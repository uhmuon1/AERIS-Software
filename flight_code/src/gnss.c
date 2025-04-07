/**
 * gnss.c - Implementation of GNSS module functionality
 */
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "gnss.h"

// GNSS Definitions
#define GNSS_ADDR 0x42
#define UBX_SYNC1 0xB5
#define UBX_SYNC2 0x62
#define NAV_CLASS 0x01
#define NAV_PVT_ID 0x07
#define MAX_UBX_LENGTH 100

// UBX-NAV-PVT Poll Request message
const uint8_t ubx_nav_pvt_poll[] = {
    0xB5, 0x62,     // Sync chars
    0x01, 0x07,     // Class (NAV) + ID (PVT)
    0x00, 0x00,     // Length (0 for poll request)
    0x08, 0x19      // Checksum
};

// Statistics counters
static uint32_t total_reads = 0;
static uint32_t successful_reads = 0;
static uint32_t failed_reads = 0;

bool gnss_init(i2c_inst_t *i2c) {
    printf("Initializing GNSS module...\n");
    
    // Poll the module to check if it's responding
    if (!send_poll_request(i2c)) {
        printf("GNSS module not responding\n");
        return false;
    }
    
    sleep_ms(100);
    
    // Try to read a message to verify communications
    gnss_data_t dummy_data;
    if (!gnss_read_location(i2c, &dummy_data)) {
        printf("Failed to read initial GNSS data\n");
        // We'll continue anyway as the module might need time to start up
    }
    
    printf("GNSS module initialized\n");
    return true;
}

bool send_poll_request(i2c_inst_t *i2c) {
    int result = i2c_write_blocking(i2c, GNSS_ADDR, ubx_nav_pvt_poll, sizeof(ubx_nav_pvt_poll), false);
    return (result == sizeof(ubx_nav_pvt_poll));
}

bool read_byte(i2c_inst_t *i2c, uint8_t *byte) {
    return (i2c_read_blocking(i2c, GNSS_ADDR, byte, 1, false) == 1);
}

bool wait_for_sync(i2c_inst_t *i2c) {
    uint8_t byte;
    int attempts = 0;
    
    while (attempts < 100) {
        if (!read_byte(i2c, &byte)) return false;
        if (byte == UBX_SYNC1) {
            if (!read_byte(i2c, &byte)) return false;
            if (byte == UBX_SYNC2) return true;
        }
        attempts++;
    }
    return false;
}

bool gnss_read_location(i2c_inst_t *i2c, gnss_data_t *data) {
    total_reads++;
    
    // First send poll request
    if (!send_poll_request(i2c)) {
        failed_reads++;
        return false;
    }
    
    sleep_ms(5); // Short delay for device to process request
    
    uint8_t header[4];
    uint8_t checksum[2];
    uint8_t pvt_data[92];  // NAV-PVT message is 92 bytes
    
    // Wait for sync characters
    if (!wait_for_sync(i2c)) {
        failed_reads++;
        return false;
    }
    
    // Read message class and ID
    if (i2c_read_blocking(i2c, GNSS_ADDR, header, 4, false) != 4) {
        failed_reads++;
        return false;
    }
    
    // Verify this is a NAV-PVT message
    if (header[0] != NAV_CLASS || header[1] != NAV_PVT_ID) {
        failed_reads++;
        return false;
    }
    
    uint16_t length = header[2] | (header[3] << 8);
    if (length != 92) {
        failed_reads++;
        return false;
    }
    
    // Read PVT data
    if (i2c_read_blocking(i2c, GNSS_ADDR, pvt_data, length, false) != length) {
        failed_reads++;
        return false;
    }
    
    // Read checksum
    if (i2c_read_blocking(i2c, GNSS_ADDR, checksum, 2, false) != 2) {
        failed_reads++;
        return false;
    }
    
    // Parse PVT data
    data->year = pvt_data[4] | (pvt_data[5] << 8);
    data->month = pvt_data[6];
    data->day = pvt_data[7];
    data->hour = pvt_data[8];
    data->min = pvt_data[9];
    data->sec = pvt_data[10];
    
    data->fix_type = pvt_data[20];
    data->satellites = pvt_data[23];
    
    data->lon = pvt_data[24] | (pvt_data[25] << 8) | (pvt_data[26] << 16) | (pvt_data[27] << 24);
    data->lat = pvt_data[28] | (pvt_data[29] << 8) | (pvt_data[30] << 16) | (pvt_data[31] << 24);
    data->altitude = pvt_data[36] | (pvt_data[37] << 8) | (pvt_data[38] << 16) | (pvt_data[39] << 24);
    
    data->vel_north = pvt_data[48] | (pvt_data[49] << 8) | (pvt_data[50] << 16) | (pvt_data[51] << 24);
    data->vel_east = pvt_data[52] | (pvt_data[53] << 8) | (pvt_data[54] << 16) | (pvt_data[55] << 24);
    data->vel_down = pvt_data[56] | (pvt_data[57] << 8) | (pvt_data[58] << 16) | (pvt_data[59] << 24);
    data->ground_speed = pvt_data[60] | (pvt_data[61] << 8) | (pvt_data[62] << 16) | (pvt_data[63] << 24);
    
    successful_reads++;
    return true;
}

const char* gnss_get_fix_type_str(uint8_t fix_type) {
    switch(fix_type) {
        case 0: return "No fix";
        case 1: return "DR";
        case 2: return "2D";
        case 3: return "3D";
        case 4: return "DR+GNSS";
        default: return "?";
    }
}

void gnss_print_status(const gnss_data_t *data) {
    // Simple, single-line status update
    printf("Time: %02d:%02d:%02d | Pos: %.5f, %.5f | Alt: %.1fm | Fix: %s | Sats: %d | Stats: %lu/%lu\r",
        data->hour, data->min, data->sec,
        data->lat * 1e-7, data->lon * 1e-7,
        data->altitude / 1000.0,
        gnss_get_fix_type_str(data->fix_type),
        data->satellites,
        successful_reads, total_reads);
}
