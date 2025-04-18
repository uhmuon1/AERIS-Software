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

bool create_data_file() {
    char filename[32];
    // Create a filename based on timestamp from boot
    uint32_t timestamp = to_ms_since_boot(get_absolute_time());
    // snprintf(filename, sizeof(filename), "0:/gnss_%04d-%02d-%02d_%02d%02d%02d.csv", 
    //          pvt_data->year, pvt_data->month, pvt_data->day, 
    //          pvt_data->hour, pvt_data->min, pvt_data->sec);
    
    FRESULT fr = f_open(&data_file, FILE_NAME, FA_WRITE | FA_CREATE_ALWAYS);
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

bool file_read_setup(){
    f_close(&data_file);
    f_open(&data_file, FILE_NAME, FA_READ);
}

bool write_data_to_sd(const ubx_pvt_data_t *data, uint32_t system_timestamp_ms) {
    char buffer[LOG_BUFFER_SIZE];
    uint32_t sys_seconds = system_timestamp_ms / 1000;
    uint32_t sys_ms = system_timestamp_ms % 1000;
    
    // Calculate milliseconds for timestamp
    uint32_t current_time_ms = to_ms_since_boot(get_absolute_time());
    uint16_t milliseconds = current_time_ms % 1000;
    
    // Format the data with maximum precision for velocities and altitudes
    // int len = snprintf(buffer, LOG_BUFFER_SIZE,
    //     "%02d:%02d,%02d:%02d:%02d:%03d,%.9f,%.9f,%.6f,%.6f,%.9f,%.9f,%.9f,%.9f,%s,%d\n",
    //     sys_seconds, sys_ms,
    //     data->hour, data->min, data->sec, milliseconds,
    //     data->lat * 1e-7, data->lon * 1e-7,
    //     data->height / 1000.0,   // Increased to 6 decimal places for altitude
    //     data->hMSL / 1000.0,     // Increased to 6 decimal places for altitude
    //     data->velN / 1000.0,     // Increased to 9 decimal places for velocities
    //     data->velE / 1000.0,     // Increased to 9 decimal places for velocities
    //     data->velD / 1000.0,     // Increased to 9 decimal places for velocities
    //     data->gSpeed / 1000.0,   // Increased to 9 decimal places for ground speed
    //     gnss_get_fix_type_str(data->fixType),
    //     data->numSV);
    int len = snprintf(buffer, LOG_BUFFER_SIZE,
        "%05d:%03d,%02d:%02d:%02d:%03d,%.9f,%.9f,%.9f,%.9f,%.9f,%.9f,%.9f,%.9f,%s,%d\n",
        sys_seconds, sys_ms,
        data->hour, data->min, data->sec, milliseconds,
        data->lat,      // Already in degrees
        data->lon,      // Already in degrees
        data->height,   // Already in meters
        data->hMSL,     // Already in meters
        data->velN,     // Already in m/s
        data->velE,     // Already in m/s
        data->velD,     // Already in m/s
        data->gSpeed,   // Already in m/s
        gnss_get_fix_type_str(data->fixType),
        data->numSV);
    
    UINT bw;
    FRESULT fr = f_write(&data_file, buffer, len, &bw);
    
    if (fr != FR_OK || bw != len) {
        failed_writes++;
        return false;
    }

    f_sync(&data_file);
    successful_writes++;
    return true;
}


// uint read_data_from_sd(uint8_t *buffer) {
//     UINT br;
//     FRESULT fr = f_read(&data_file, buffer, sizeof(buffer) - 1, &br);
//     if (fr != FR_OK) {
//         printf("Failed to read file: %d\n Closing file", fr);
//         f_close(&data_file);
//         return -1;
//     }
//     return br;
// }

uint32_t read_data_from_sd(uint8_t *lora_packet) {
    uint8_t buffer[256];  // Temporary buffer for raw SD card data
    UINT br;
    
    // Read one line from SD card
    FRESULT fr = f_read(&data_file, buffer, sizeof(buffer) - 1, &br);
    if (fr != FR_OK || br == 0) {
        printf("Failed to read file or EOF: %d\n", fr);
        return 0;
    }
    
    buffer[br] = '\0';  // Null-terminate for string operations
    
    // Parse the CSV line
    char *token;
    char *rest = (char*)buffer;
    
    // Skip system time (column 1)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    
    // Get GNSS time (column 2)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    printf("GNSS time: %s\n",token);
    
    // Parse HH:MM:SS:MS
    uint8_t hour, min, sec;
    uint16_t ms;
    sscanf(token, "%hhu:%hhu:%hhu:%hu", &hour, &min, &sec, &ms);
    
    // Parse remaining values
    // Lat (column 3)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    double lat = atof(token);
    printf("Latitude: %s\n",token);
    
    // Lon (column 4)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    double lon = atof(token);
    printf("Longitude: %s\n",token);
    
    // Height (column 5)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    double height = atof(token);
    printf("Height: %s\n",token);
    
    // hMSL (column 6)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    double hMSL = atof(token);
    printf("hMSL: %s\n",token);
    
    // velN (column 7)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    double velN = atof(token);
    printf("velN: %s\n",token);
    
    // velE (column 8)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    double velE = atof(token);
    printf("velE: %s\n",token);
    
    // velD (column 9)
    token = strtok_r(rest, ",", &rest);
    if (token == NULL) return 0;
    double velD = atof(token);
    printf("velD: %s\n",token);
    
    // Now pack into binary format for LoRa
    uint32_t pos = 0;
    
    // Pack time values
    lora_packet[pos++] = hour;
    lora_packet[pos++] = min;
    lora_packet[pos++] = sec;
    lora_packet[pos++] = ms & 0xFF;
    lora_packet[pos++] = (ms >> 8) & 0xFF;
    
    // Pack position, height and velocity values
    memcpy(lora_packet + pos, &lat, sizeof(double));
    pos += sizeof(double);
    
    memcpy(lora_packet + pos, &lon, sizeof(double));
    pos += sizeof(double);
    
    memcpy(lora_packet + pos, &height, sizeof(double));
    pos += sizeof(double);
    
    memcpy(lora_packet + pos, &hMSL, sizeof(double));
    pos += sizeof(double);
    
    memcpy(lora_packet + pos, &velN, sizeof(double));
    pos += sizeof(double);
    
    memcpy(lora_packet + pos, &velE, sizeof(double));
    pos += sizeof(double);
    
    memcpy(lora_packet + pos, &velD, sizeof(double));
    pos += sizeof(double);

    printf("Raw data (hex): ");
    for (int i = 0; i < pos; i++) {
        printf("%02X ", lora_packet[i]);
    }
    printf("\n\n");
    
    return pos;  // Return number of bytes packed
}

bool quit_sd_card(){
    f_close(&data_file);
    f_unmount("0:");
}