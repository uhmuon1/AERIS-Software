/**
 * gnss.h - GNSS module interface
 */
#ifndef GNSS_H
#define GNSS_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/i2c.h"

// GNSS Definitions
#define GNSS_ADDR 0x42
#define UBX_SYNC1 0xB5
#define UBX_SYNC2 0x62
#define NAV_CLASS 0x01
#define NAV_PVT_ID 0x07
#define MAX_UBX_LENGTH 100

// File system objects
static FATFS fs;
static FIL data_file;

// Timing and logging control
#define LOG_INTERVAL_MS 40  // 25Hz = 40ms between samples
#define STATUS_INTERVAL_MS 1000  // Status update every second

void blink_core1_entry();

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
    
    // Velocity data (all in mm/s)
    int32_t velN;   // North velocity
    int32_t velE;   // East velocity
    int32_t velD;   // Down velocity
    uint32_t gSpeed; // Ground speed
    
    // Status data
    uint8_t numSV;  // Number of satellites used
    uint8_t fixType;// Fix type
} ubx_pvt_data_t;

// Function prototypes
bool gnss_init(i2c_inst_t *i2c);
bool gnss_read_location(i2c_inst_t *i2c, ubx_pvt_data_t *data);
void gnss_print_status(const ubx_pvt_data_t *data);
const char* gnss_get_fix_type_str(uint8_t fix_type);

#endif // GNSS_H
