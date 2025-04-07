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

bool init_sd_card();
bool create_data_file(const gnss_data_t *pvt_data);

#endif