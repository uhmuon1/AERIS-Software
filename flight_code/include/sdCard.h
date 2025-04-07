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

extern int successful_writes;
extern int failed_writes;

bool init_sd_card();
bool create_data_file(const gnss_data_t *pvt_data);
bool write_data_to_sd(const gnss_data_t *data, uint32_t system_timestamp_ms);
bool read_data_from_sd(const gnss_data_t *buffer);
bool quit_sd_card();

#endif