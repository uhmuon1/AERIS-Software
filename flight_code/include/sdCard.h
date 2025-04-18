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
static char* FILE_NAME = "0:/gnss_2025-04-16_203400.csv";

bool init_sd_card();
bool create_data_file();
bool file_read_setup();
bool write_data_to_sd(const ubx_pvt_data_t *data, uint32_t system_timestamp_ms);
uint32_t read_data_from_sd(uint8_t *buffer);
bool quit_sd_card();

#endif // end SD_H