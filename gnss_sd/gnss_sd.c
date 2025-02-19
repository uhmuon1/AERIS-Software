#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ff.h"
#include "f_util.h"
#include "hw_config.h"

#define GNSS_ADDR 0x42
#define UBX_SYNC1 0xB5
#define UBX_SYNC2 0x62
#define NAV_CLASS 0x01
#define NAV_PVT_ID 0x07
#define MAX_UBX_LENGTH 100
#define FILENAME "gps_data.csv"

// [Previous UBX message structures and functions remain the same]

// Function to write CSV header
bool write_csv_header(FIL* fil) {
    UINT bw;
    const char* header = "Timestamp,Date,Time,Latitude,Longitude,Height,MSL_Height,Velocity_N,Velocity_E,Velocity_D,Ground_Speed,Fix_Type,Satellites\n";
    FRESULT fr = f_write(fil, header, strlen(header), &bw);
    return (fr == FR_OK);
}

// Function to write data to CSV
bool write_csv_data(FIL* fil, ubx_pvt_data_t* data) {
    char buffer[256];
    uint32_t timestamp = to_ms_since_boot(get_absolute_time());
    
    snprintf(buffer, sizeof(buffer), 
             "%lu,%02d/%02d/%04d,%02d:%02d:%02d,%.7f,%.7f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d\n",
             timestamp,
             data->day, data->month, data->year,
             data->hour, data->min, data->sec,
             data->lat * 1e-7,
             data->lon * 1e-7,
             data->height / 1000.0,
             data->hMSL / 1000.0,
             data->velN / 1000.0,
             data->velE / 1000.0,
             data->velD / 1000.0,
             data->gSpeed / 1000.0,
             data->fixType,
             data->numSV);
    
    UINT bw;
    FRESULT fr = f_write(fil, buffer, strlen(buffer), &bw);
    if (fr != FR_OK) {
        printf("Failed to write to CSV\n");
        return false;
    }
    
    // Flush the data to ensure it's written to the card
    f_sync(fil);
    return true;
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    
    printf("\nStarting UBX Message Reader with CSV Logging\n");
    printf("------------------------------------------\n");
    
    // Initialize I2C
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    printf("I2C Initialized\n");
    
    // Initialize SD card
    sd_card_t *pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (fr != FR_OK) {
        printf("Failed to mount SD card\n");
        return 1;
    }
    printf("SD card mounted\n");
    
    // Open CSV file
    FIL fil;
    fr = f_open(&fil, FILENAME, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        printf("Failed to open file\n");
        return 1;
    }
    printf("CSV file opened\n");
    
    // Write CSV header
    if (!write_csv_header(&fil)) {
        printf("Failed to write CSV header\n");
        f_close(&fil);
        return 1;
    }
    printf("CSV header written\n");
    
    ubx_pvt_data_t pvt_data;
    uint32_t record_count = 0;
    
    while (true) {
        if (read_ubx_message(i2c_default, &pvt_data)) {
            printf("\033[2J\033[H");  // Clear screen
            
            // Display data as before
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
            
            // Write to CSV
            if (write_csv_data(&fil, &pvt_data)) {
                record_count++;
                printf("\nRecords written: %lu\n", record_count);
            }
        } else {
            printf(".");
            fflush(stdout);
        }
        
        sleep_ms(100);  // Poll rate limit
    }
    
    f_close(&fil);
    f_unmount(pSD->pcName);
    
    return 0;
}