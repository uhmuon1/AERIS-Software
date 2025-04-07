#include "sdCard.h"

int successful_writes;
int failed_writes;

bool init_sd_card() {
    printf("Initializing SD card...\n");
    FRESULT fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        printf("Failed to mount SD card: %d\n", fr);
        return false;
    }
    printf("SD card mounted\n");

    successful_writes = 0;
    failed_writes = 0;
    return true;
}

bool create_data_file(const ubx_pvt_data_t *pvt_data) {
    char filename[32];
    // Create a filename based on timestamp from boot
    uint32_t timestamp = to_ms_since_boot(get_absolute_time());
    snprintf(filename, sizeof(filename), "0:/gnss_%04d-%02d-%02d_%02d%02d%02d.csv", 
             pvt_data->year, pvt_data->month, pvt_data->day, 
             pvt_data->hour, pvt_data->min, pvt_data->sec);
    
    FRESULT fr = f_open(&data_file, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        printf("Failed to create file: %d\n", fr);
        return false;
    }
    
    // Write CSV header
    UINT bw;
    const char *header = "sys_time,gnss_time,lat,lon,alt,vel_n,vel_e,vel_d,gspeed,fix,sats\n";
    fr = f_write(&data_file, header, strlen(header), &bw);
    if (fr != FR_OK || bw != strlen(header)) {
        printf("Failed to write header: %d\n", fr);
        f_close(&data_file);
        return false;
    }
    
    printf("Created file: %s\n", filename);
    return true;
}

bool reset_f_ptr(){
    f_lseek(&data_file, 0);
}

bool write_data_to_sd(const ubx_pvt_data_t *data, uint32_t system_timestamp_ms) {
    char buffer[LOG_BUFFER_SIZE];
    uint32_t sys_seconds = system_timestamp_ms / 1000;
    uint32_t sys_ms = system_timestamp_ms % 1000;
    //uint16_t gnss_ms = (data->nano /1000000) % 1000;
    
    // Calculate milliseconds (if you have a nanosecond timestamp, do something like this)
    // uint32_t milliseconds = (data->nano / 1000000) % 1000; // For example, if you had a nano field
    
    // Or if your data doesn't include nanoseconds, we can simply use the current timestamp
    uint32_t current_time_ms = to_ms_since_boot(get_absolute_time());
    uint16_t milliseconds = current_time_ms % 1000; // Extract milliseconds
    
    // Format the data as CSV with milliseconds included
    int len = snprintf(buffer, LOG_BUFFER_SIZE,
        "%02d:%02d,%02d:%02d:%02d:%03d,%.7f,%.7f,%.2f,%.2f,%.2f,%.2f,%.2f,%s,%d\n",
        sys_seconds, sys_ms,
        data->hour, data->min, data->sec, milliseconds, // Include milliseconds here
        data->lat * 1e-7, data->lon * 1e-7,
        data->hMSL / 1000.0,
        data->velN / 1000.0, data->velE / 1000.0, data->velD / 1000.0,
        data->gSpeed / 1000.0,
        gnss_get_fix_type_str(data->fixType),
        data->numSV);
    
    UINT bw;
    FRESULT fr = f_write(&data_file, buffer, len, &bw);
    
    if (fr != FR_OK || bw != len) {
        failed_writes++;
        return false;
    }
    
    // Sync every 25 samples (about once per second)
    if (successful_writes % 25 == 0) {
        fr = f_sync(&data_file);
        if (fr != FR_OK) {
            return false;
        }
    }
    
    successful_writes++;
    return true;
}


uint read_data_from_sd(uint8_t *buffer) {
    UINT br;
    FRESULT fr = f_read(&data_file, buffer, sizeof(buffer) - 1, &br);
    if (fr != FR_OK) {
        printf("Failed to read file: %d\n Closing file", fr);
        f_close(&data_file);
        return -1;
    }
    return br;
}

bool quit_sd_card(){
    f_close(&data_file);
    f_unmount("0:");
}