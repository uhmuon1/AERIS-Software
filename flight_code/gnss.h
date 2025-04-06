/**
 * gnss.h - GNSS module interface
 */
#ifndef GNSS_H
#define GNSS_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/i2c.h"

// GNSS data structure
typedef struct {
    // Time data
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    
    // Position data
    int32_t lon;          // Scaled by 10^-7
    int32_t lat;          // Scaled by 10^-7
    int32_t altitude;     // Height above mean sea level in mm
    
    // Velocity data
    int32_t vel_north;    // North velocity in mm/s
    int32_t vel_east;     // East velocity in mm/s
    int32_t vel_down;     // Down velocity in mm/s
    uint32_t ground_speed;// Ground speed in mm/s
    
    // Status data
    uint8_t satellites;   // Number of satellites used
    uint8_t fix_type;     // Fix type
    
    // System time when data was collected
    uint32_t system_time_ms;
} gnss_data_t;

// Function prototypes
bool gnss_init(i2c_inst_t *i2c);
bool gnss_read_location(i2c_inst_t *i2c, gnss_data_t *data);
void gnss_print_status(const gnss_data_t *data);
const char* gnss_get_fix_type_str(uint8_t fix_type);

#endif // GNSS_H
