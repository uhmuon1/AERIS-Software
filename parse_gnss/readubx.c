#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

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

typedef struct {
    // Time data
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    
    // Position data (all in cm)
    int32_t lon;    // Scaled by 10^-7
    int32_t lat;    // Scaled by 10^-7
    int32_t height; // Height above ellipsoid in mm
    int32_t hMSL;   // Height above mean sea level in mm
    
    // Velocity data (all in cm/s)
    int32_t velN;   // North velocity
    int32_t velE;   // East velocity
    int32_t velD;   // Down velocity
    uint32_t gSpeed; // Ground speed
    
    // Status data
    uint8_t numSV;  // Number of satellites used
    uint8_t fixType;// Fix type
} ubx_pvt_data_t;

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

bool read_ubx_message(i2c_inst_t *i2c, ubx_pvt_data_t *data) {
    // First send poll request
    if (!send_poll_request(i2c)) {
        printf("Failed to send poll request\n");
        return false;
    }
    
    sleep_ms(5); // Short delay for device to process request
    
    uint8_t header[4];
    uint8_t checksum[2];
    uint8_t pvt_data[92];  // NAV-PVT message is 92 bytes
    
    // Wait for sync characters
    if (!wait_for_sync(i2c)) {
        return false;
    }
    
    // Read message class and ID
    if (i2c_read_blocking(i2c, GNSS_ADDR, header, 4, false) != 4) {
        return false;
    }
    
    // Verify this is a NAV-PVT message
    if (header[0] != NAV_CLASS || header[1] != NAV_PVT_ID) {
        return false;
    }
    
    uint16_t length = header[2] | (header[3] << 8);
    if (length != 92) {
        return false;
    }
    
    // Read PVT data
    if (i2c_read_blocking(i2c, GNSS_ADDR, pvt_data, length, false) != length) {
        return false;
    }
    
    // Read checksum
    if (i2c_read_blocking(i2c, GNSS_ADDR, checksum, 2, false) != 2) {
        return false;
    }
    
    // Parse PVT data
    data->year = pvt_data[4] | (pvt_data[5] << 8);
    data->month = pvt_data[6];
    data->day = pvt_data[7];
    data->hour = pvt_data[8];
    data->min = pvt_data[9];
    data->sec = pvt_data[10];
    
    data->fixType = pvt_data[20];
    data->numSV = pvt_data[23];
    
    data->lon = pvt_data[24] | (pvt_data[25] << 8) | (pvt_data[26] << 16) | (pvt_data[27] << 24);
    data->lat = pvt_data[28] | (pvt_data[29] << 8) | (pvt_data[30] << 16) | (pvt_data[31] << 24);
    data->height = pvt_data[32] | (pvt_data[33] << 8) | (pvt_data[34] << 16) | (pvt_data[35] << 24);
    data->hMSL = pvt_data[36] | (pvt_data[37] << 8) | (pvt_data[38] << 16) | (pvt_data[39] << 24);
    
    data->velN = pvt_data[48] | (pvt_data[49] << 8) | (pvt_data[50] << 16) | (pvt_data[51] << 24);
    data->velE = pvt_data[52] | (pvt_data[53] << 8) | (pvt_data[54] << 16) | (pvt_data[55] << 24);
    data->velD = pvt_data[56] | (pvt_data[57] << 8) | (pvt_data[58] << 16) | (pvt_data[59] << 24);
    data->gSpeed = pvt_data[60] | (pvt_data[61] << 8) | (pvt_data[62] << 16) | (pvt_data[63] << 24);
    
    return true;
}

const char* get_fix_type_str(uint8_t fix_type) {
    switch(fix_type) {
        case 0: return "No fix";
        case 1: return "Dead reckoning";
        case 2: return "2D fix";
        case 3: return "3D fix";
        case 4: return "GNSS + Dead reckoning";
        default: return "Unknown";
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    printf("\nStarting UBX Message Reader with Poll Request\n");
    printf("----------------------------------------\n");
    
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    printf("I2C Initialized\n\n");
    
    ubx_pvt_data_t pvt_data;
    
    while (true) {
        if (read_ubx_message(i2c_default, &pvt_data)) {
            printf("\033[2J\033[H");  // Clear screen
            
            printf("Time: %02d:%02d:%02d %02d/%02d/%04d\n", 
                   pvt_data.hour, pvt_data.min, pvt_data.sec,
                   pvt_data.day, pvt_data.month, pvt_data.year);
            
            printf("\nPosition:\n");
            printf("  Latitude:  %.7f degrees\n", pvt_data.lat * 1e-7);
            printf("  Longitude: %.7f degrees\n", pvt_data.lon * 1e-7);
            printf("  Height:    %.2f m (above ellipsoid)\n", pvt_data.height / 1000.0);
            printf("  Altitude:  %.2f m (above sea level)\n", pvt_data.hMSL / 1000.0);
            
            printf("\nVelocity:\n");
            printf("  North:     %.2f m/s\n", pvt_data.velN / 1000.0);
            printf("  East:      %.2f m/s\n", pvt_data.velE / 1000.0);
            printf("  Down:      %.2f m/s\n", pvt_data.velD / 1000.0);
            printf("  Ground:    %.2f m/s\n", pvt_data.gSpeed / 1000.0);
            
            printf("\nStatus:\n");
            printf("  Fix type:  %s\n", get_fix_type_str(pvt_data.fixType));
            printf("  Satellites: %d\n", pvt_data.numSV);
        } else {
            printf(".");  // Show activity while waiting
            fflush(stdout);
        }
        
        sleep_ms(100);  // Poll rate limit
    }
    
    return 0;
}