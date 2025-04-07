/**
 * motor.h - SD Card Logging
 */

#ifndef SD_H
#define SD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "ff.h"
#include "f_util.h"
#include "hw_config.h"
#include "gnss.h"

// File system objects
static FATFS fs;
static FIL data_file;
static uint LOG_BUFFER_SIZE = 256;

bool init_sd_card();
bool create_data_file(const ubx_pvt_data_t *pvt_data);
bool reset_f_ptr();
bool write_data_to_sd(const ubx_pvt_data_t *data, uint32_t system_timestamp_ms);
uint read_data_from_sd(uint8_t *buffer);
bool quit_sd_card();

#endif // end SD_H