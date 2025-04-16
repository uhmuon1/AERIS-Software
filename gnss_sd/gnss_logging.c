
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include "ff.h"
// #include "f_util.h"
// #include "hw_config.h"
// #include "pico/multicore.h"

// // GNSS Definitions
// #define GNSS_ADDR 0x42
// #define UBX_SYNC1 0xB5
// #define UBX_SYNC2 0x62
// #define NAV_CLASS 0x01
// #define NAV_PVT_ID 0x07
// #define MAX_UBX_LENGTH 100

// // File system objects
// static FATFS fs;
// static FIL data_file;

// // Timing and logging control
// #define LOG_INTERVAL_MS 40  // 25Hz = 40ms between samples
// #define STATUS_INTERVAL_MS 1000  // Status update every second
// #define LOG_BUFFER_SIZE 256

// void blink_core1_entry();

// // UBX-NAV-PVT Poll Request message
// const uint8_t ubx_nav_pvt_poll[] = {
//     0xB5, 0x62,     // Sync chars
//     0x01, 0x07,     // Class (NAV) + ID (PVT)
//     0x00, 0x00,     // Length (0 for poll request)
//     0x08, 0x19      // Checksum
// };

// typedef struct {
//     // Time data
//     uint16_t year;
//     uint8_t month;
//     uint8_t day;
//     uint8_t hour;
//     uint8_t min;
//     uint8_t sec;
    
//     // Position data (all in cm)
//     int32_t lon;    // Scaled by 10^-7
//     int32_t lat;    // Scaled by 10^-7
//     int32_t height; // Height above ellipsoid in mm
//     int32_t hMSL;   // Height above mean sea level in mm
    
//     // Velocity data (all in mm/s)
//     int32_t velN;   // North velocity
//     int32_t velE;   // East velocity
//     int32_t velD;   // Down velocity
//     uint32_t gSpeed; // Ground speed
    
//     // Status data
//     uint8_t numSV;  // Number of satellites used
//     uint8_t fixType;// Fix type
// } ubx_pvt_data_t;

// // Statistics counters
// uint32_t total_reads = 0;
// uint32_t successful_reads = 0;
// uint32_t successful_writes = 0;
// uint32_t failed_reads = 0;
// uint32_t failed_writes = 0;

// bool send_poll_request(i2c_inst_t *i2c) {
//     int result = i2c_write_blocking(i2c, GNSS_ADDR, ubx_nav_pvt_poll, sizeof(ubx_nav_pvt_poll), false);
//     return (result == sizeof(ubx_nav_pvt_poll));
// }

// bool read_byte(i2c_inst_t *i2c, uint8_t *byte) {
//     return (i2c_read_blocking(i2c, GNSS_ADDR, byte, 1, false) == 1);
// }

// bool wait_for_sync(i2c_inst_t *i2c) {
//     uint8_t byte;
//     int attempts = 0;
    
//     while (attempts < 100) {
//         if (!read_byte(i2c, &byte)) return false;
//         if (byte == UBX_SYNC1) {
//             if (!read_byte(i2c, &byte)) return false;
//             if (byte == UBX_SYNC2) return true;
//         }
//         attempts++;
//     }
//     return false;
// }

// bool read_ubx_message(i2c_inst_t *i2c, ubx_pvt_data_t *data) {
//     total_reads++;
    
//     // First send poll request
//     if (!send_poll_request(i2c)) {
//         failed_reads++;
//         return false;
//     }
    
//     sleep_ms(5); // Short delay for device to process request
    
//     uint8_t header[4];
//     uint8_t checksum[2];
//     uint8_t pvt_data[92];  // NAV-PVT message is 92 bytes
    
//     // Wait for sync characters
//     if (!wait_for_sync(i2c)) {
//         failed_reads++;
//         return false;
//     }
    
//     // Read message class and ID
//     if (i2c_read_blocking(i2c, GNSS_ADDR, header, 4, false) != 4) {
//         failed_reads++;
//         return false;
//     }
    
//     // Verify this is a NAV-PVT message
//     if (header[0] != NAV_CLASS || header[1] != NAV_PVT_ID) {
//         failed_reads++;
//         return false;
//     }
    
//     uint16_t length = header[2] | (header[3] << 8);
//     if (length != 92) {
//         failed_reads++;
//         return false;
//     }
    
//     // Read PVT data
//     if (i2c_read_blocking(i2c, GNSS_ADDR, pvt_data, length, false) != length) {
//         failed_reads++;
//         return false;
//     }
    
//     // Read checksum
//     if (i2c_read_blocking(i2c, GNSS_ADDR, checksum, 2, false) != 2) {
//         failed_reads++;
//         return false;
//     }
    
//     // Parse PVT data
//     data->year = pvt_data[4] | (pvt_data[5] << 8);
//     data->month = pvt_data[6];
//     data->day = pvt_data[7];
//     data->hour = pvt_data[8];
//     data->min = pvt_data[9];
//     data->sec = pvt_data[10];
    
//     data->fixType = pvt_data[20];
//     data->numSV = pvt_data[23];
    
//     data->lon = pvt_data[24] | (pvt_data[25] << 8) | (pvt_data[26] << 16) | (pvt_data[27] << 24);
//     data->lat = pvt_data[28] | (pvt_data[29] << 8) | (pvt_data[30] << 16) | (pvt_data[31] << 24);
//     data->height = pvt_data[32] | (pvt_data[33] << 8) | (pvt_data[34] << 16) | (pvt_data[35] << 24);
//     data->hMSL = pvt_data[36] | (pvt_data[37] << 8) | (pvt_data[38] << 16) | (pvt_data[39] << 24);
    
//     data->velN = pvt_data[48] | (pvt_data[49] << 8) | (pvt_data[50] << 16) | (pvt_data[51] << 24);
//     data->velE = pvt_data[52] | (pvt_data[53] << 8) | (pvt_data[54] << 16) | (pvt_data[55] << 24);
//     data->velD = pvt_data[56] | (pvt_data[57] << 8) | (pvt_data[58] << 16) | (pvt_data[59] << 24);
//     data->gSpeed = pvt_data[60] | (pvt_data[61] << 8) | (pvt_data[62] << 16) | (pvt_data[63] << 24);
    
//     successful_reads++;
//     return true;
// }

// const char* get_fix_type_str(uint8_t fix_type) {
//     switch(fix_type) {
//         case 0: return "No fix";
//         case 1: return "DR";
//         case 2: return "2D";
//         case 3: return "3D";
//         case 4: return "DR+GNSS";
//         default: return "?";
//     }
// }

// bool init_sd_card() {
//     printf("Initializing SD card...\n");
//     FRESULT fr = f_mount(&fs, "0:", 1);
//     if (fr != FR_OK) {
//         printf("Failed to mount SD card: %d\n", fr);
//         return false;
//     }
//     printf("SD card mounted\n");
//     return true;
// }

// bool create_data_file(const ubx_pvt_data_t *pvt_data) {
//     char filename[32];
//     // Create a filename based on timestamp from boot
//     uint32_t timestamp = to_ms_since_boot(get_absolute_time());
//     snprintf(filename, sizeof(filename), "0:/gnss_%04d-%02d-%02d_%02d%02d%02d.csv", 
//              pvt_data->year, pvt_data->month, pvt_data->day, 
//              pvt_data->hour, pvt_data->min, pvt_data->sec);
    
//     FRESULT fr = f_open(&data_file, filename, FA_WRITE | FA_CREATE_ALWAYS);
//     if (fr != FR_OK) {
//         printf("Failed to create file: %d\n", fr);
//         return false;
//     }
    
//     // Write CSV header
//     UINT bw;
//     const char *header = "sys_time,gnss_time,lat,lon,alt,vel_n,vel_e,vel_d,gspeed,fix,sats\n";
//     fr = f_write(&data_file, header, strlen(header), &bw);
//     if (fr != FR_OK || bw != strlen(header)) {
//         printf("Failed to write header: %d\n", fr);
//         f_close(&data_file);
//         return false;
//     }
    
//     printf("Created file: %s\n", filename);
//     return true;
// }

// bool write_data_to_sd(const ubx_pvt_data_t *data, uint32_t system_timestamp_ms) {
//     char buffer[LOG_BUFFER_SIZE];
//     uint32_t sys_seconds = system_timestamp_ms / 1000;
//     uint32_t sys_ms = system_timestamp_ms % 1000;
//     //uint16_t gnss_ms = (data->nano /1000000) % 1000;
    
//     // Calculate milliseconds (if you have a nanosecond timestamp, do something like this)
//     // uint32_t milliseconds = (data->nano / 1000000) % 1000; // For example, if you had a nano field
    
//     // Or if your data doesn't include nanoseconds, we can simply use the current timestamp
//     uint32_t current_time_ms = to_ms_since_boot(get_absolute_time());
//     uint16_t milliseconds = current_time_ms % 1000; // Extract milliseconds
    
//     // Format the data as CSV with milliseconds included
//     int len = snprintf(buffer, LOG_BUFFER_SIZE,
//         "%02d:%02d,%02d:%02d:%02d:%03d,%.7f,%.7f,%.2f,%.2f,%.2f,%.2f,%.2f,%s,%d\n",
//         sys_seconds, sys_ms,
//         data->hour, data->min, data->sec, milliseconds, // Include milliseconds here
//         data->lat * 1e-7, data->lon * 1e-7,
//         data->hMSL / 1000.0,
//         data->velN / 1000.0, data->velE / 1000.0, data->velD / 1000.0,
//         data->gSpeed / 1000.0,
//         get_fix_type_str(data->fixType),
//         data->numSV);
    
//     UINT bw;
//     FRESULT fr = f_write(&data_file, buffer, len, &bw);
    
//     if (fr != FR_OK || bw != len) {
//         failed_writes++;
//         return false;
//     }
    
//     // Sync every 25 samples (about once per second)
//     if (successful_writes % 25 == 0) {
//         fr = f_sync(&data_file);
//         if (fr != FR_OK) {
//             return false;
//         }
//     }
    
//     successful_writes++;
//     return true;
// }

// void print_status_line(const ubx_pvt_data_t *data) {
//     // Use simple, single-line status updates instead of clearing the screen
//     printf("Time: %02d:%02d:%02d | Pos: %.5f, %.5f | Fix: %s | Sats: %d | Log: %lu/%lu\r",
//         data->hour, data->min, data->sec,
//         data->lat * 1e-7, data->lon * 1e-7,
//         get_fix_type_str(data->fixType),
//         data->numSV,
//         successful_writes, total_reads);
    
//     // Very important: don't use fflush() excessively as it can cause issues with some
//     // serial terminals. Let the terminal handle buffering.
// }


// int pico_led_init(void) {
// #if defined(PICO_DEFAULT_LED_PIN)
//     // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
//     // so we can use normal GPIO functionality to turn the led on and off
//     gpio_init(PICO_DEFAULT_LED_PIN);
//     gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
//     return PICO_OK;
// #elif defined(CYW43_WL_GPIO_LED_PIN)
//     // For Pico W devices we need to initialise the driver etc
//     return cyw43_arch_init();
// #endif
// }

// // Turn the led on or off
// void pico_set_led(bool led_on) {
// #if defined(PICO_DEFAULT_LED_PIN)
//     // Just set the GPIO on or off
//     gpio_put(PICO_DEFAULT_LED_PIN, led_on);
// #elif defined(CYW43_WL_GPIO_LED_PIN)
//     // Ask the wifi "driver" to set the GPIO on or off
//     cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
// #endif
// }

// void blink_task(int *freq, bool *on) {
//     while (*on) {
//         pico_set_led(true);
//         sleep_ms(*freq);
//         pico_set_led(false);
//         sleep_ms(*freq);
//     }
//     pico_set_led(false);
// }

// int main() {
//     stdio_init_all();
    
//     // Important: Give enough time for UART to initialize
//     sleep_ms(20000);
    
//     printf("\nGNSS Data Logger - 25Hz\n");
//     printf("Starting in 2 seconds...\n");
//     sleep_ms(2000);
    
//     // Initialize I2C for GNSS module
//     i2c_init(i2c_default, 100 * 1000);
//     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
//     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
//     printf("I2C Initialized\n");

//     pico_led_init();

//     // int i2cfreq = 50;
//     int waitfreq = 1000;
//     int fixedfreq = 25;
//     int logfreq = 1000;
//     int donefreq = 1000;

//     int blink_freq = 1000; 
//     bool blinking = true;
    
//     // Create a second core task for blinking
//     multicore_launch_core1(blink_core1_entry);
    
//     // Pass the frequency and on/off variables to core 1
//     multicore_fifo_push_blocking((uint32_t)&blink_freq);
//     multicore_fifo_push_blocking((uint32_t)&blinking);

//     // Initialize SD card
//     if (!init_sd_card()) {
//         printf("SD card failed\n");
//         while (1) tight_loop_contents();
//     }
    
//     // Variables for timing
//     ubx_pvt_data_t pvt_data;
//     uint32_t next_sample_time = 0;
//     uint32_t next_status_time = 0;
    
//     // Set the recording time limit (80 seconds)
//     uint32_t start_time = 0;
//     uint32_t time_limit = 80 * 1000;  // 80 seconds in milliseconds
//     bool time_limit_started = false;  // Flag to indicate if the 3D fix has been obtained

//     printf("Logging started\n");
    
//     while (true) {
//         uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
//         // Collect a sample
//         if (current_time >= next_sample_time) {
//             next_sample_time = current_time + LOG_INTERVAL_MS;
//             // blink_freq = 1000;
//             if (read_ubx_message(i2c_default, &pvt_data)) {
//                 // Only log if we have a valid position
//                 if (pvt_data.fixType >= 3) {  // 3D fix is fixType 3 or higher
//                     if (!time_limit_started) {
//                         // Start the timer once we get the first 3D fix
                        
//                         time_limit_started = true;
//                         start_time = current_time;  // Start counting time from here
//                         blink_freq = 50;
//                         // Create the file with a date-time based filename
//                         if (!create_data_file(&pvt_data)) {
//                             printf("File creation failed\n");
//                             while (1) tight_loop_contents();
//                         }
//                     }
//                     // blink_freq = fixedfreq;
//                     write_data_to_sd(&pvt_data, current_time);
//                 }
                
//                 // Display status update (throttled to reduce serial traffic)
//                 if (current_time >= next_status_time) {
//                     print_status_line(&pvt_data);
//                     next_status_time = current_time + STATUS_INTERVAL_MS;
//                 }
//             }
//         }
        
//         // Check if we've exceeded the time limit after the first 3D fix
//         if (time_limit_started && current_time - start_time >= time_limit) {
//             blink_freq = 3000;
//             printf("Time limit reached, stopping logging...\n");
//             break;  // Exit the loop after 80 seconds
//         }

//         // Small sleep to prevent tight loop when waiting for next sample time
//         sleep_us(500);
//     }
    
//     // Close the data file and unmount the SD card
//     f_close(&data_file);
//     f_unmount("0:");
//     return 0;
// }

// void blink_core1_entry() {
//     // Get the parameters from core 0
//     int *freq = (int*)multicore_fifo_pop_blocking();
//     bool *on = (bool*)multicore_fifo_pop_blocking();
    
//     // Run the blink task
//     blink_task(freq, on);
// }

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ff.h"
#include "f_util.h"
#include "hw_config.h"
#include "pico/multicore.h"

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
#define LOG_BUFFER_SIZE 256

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

// Statistics counters
uint32_t total_reads = 0;
uint32_t successful_reads = 0;
uint32_t successful_writes = 0;
uint32_t failed_reads = 0;
uint32_t failed_writes = 0;

// Debug counters
uint32_t i2c_errors = 0;
uint32_t sync_errors = 0;
uint32_t message_errors = 0;
uint32_t fix_attempts = 0;

bool send_poll_request(i2c_inst_t *i2c) {
    int result = i2c_write_blocking(i2c, GNSS_ADDR, ubx_nav_pvt_poll, sizeof(ubx_nav_pvt_poll), false);
    if (result != sizeof(ubx_nav_pvt_poll)) {
        i2c_errors++;
        printf("I2C write error: %d bytes written, expected %d\n", result, sizeof(ubx_nav_pvt_poll));
        return false;
    }
    return true;
}

bool read_byte(i2c_inst_t *i2c, uint8_t *byte) {
    int result = i2c_read_blocking(i2c, GNSS_ADDR, byte, 1, false);
    if (result != 1) {
        i2c_errors++;
        return false;
    }
    return true;
}

bool wait_for_sync(i2c_inst_t *i2c) {
    uint8_t byte;
    int attempts = 0;
    
    while (attempts < 100) {
        if (!read_byte(i2c, &byte)) {
            printf("Failed to read byte during sync (attempt %d)\n", attempts);
            return false;
        }
        if (byte == UBX_SYNC1) {
            if (!read_byte(i2c, &byte)) {
                printf("Failed to read second sync byte\n");
                return false;
            }
            if (byte == UBX_SYNC2) return true;
        }
        attempts++;
    }
    sync_errors++;
    printf("Sync not found after %d attempts\n", attempts);
    return false;
}

bool read_ubx_message(i2c_inst_t *i2c, ubx_pvt_data_t *data) {
    total_reads++;
    
    // First send poll request
    if (!send_poll_request(i2c)) {
        failed_reads++;
        return false;
    }
    
    sleep_ms(5); // Short delay for device to process request
    
    uint8_t header[4];
    uint8_t checksum[2];
    uint8_t pvt_data[92];  // NAV-PVT message is 92 bytes
    
    // Wait for sync characters
    if (!wait_for_sync(i2c)) {
        failed_reads++;
        return false;
    }
    
    // Read message class and ID
    int read_result = i2c_read_blocking(i2c, GNSS_ADDR, header, 4, false);
    if (read_result != 4) {
        failed_reads++;
        message_errors++;
        printf("Failed to read header: %d bytes read\n", read_result);
        return false;
    }
    
    // Verify this is a NAV-PVT message
    if (header[0] != NAV_CLASS || header[1] != NAV_PVT_ID) {
        failed_reads++;
        message_errors++;
        printf("Wrong message: class=0x%02X, ID=0x%02X (expected 0x%02X, 0x%02X)\n", 
               header[0], header[1], NAV_CLASS, NAV_PVT_ID);
        return false;
    }
    
    uint16_t length = header[2] | (header[3] << 8);
    if (length != 92) {
        failed_reads++;
        message_errors++;
        printf("Wrong length: %d (expected 92)\n", length);
        return false;
    }
    
    // Read PVT data
    read_result = i2c_read_blocking(i2c, GNSS_ADDR, pvt_data, length, false);
    if (read_result != length) {
        failed_reads++;
        message_errors++;
        printf("Failed to read PVT data: %d/%d bytes\n", read_result, length);
        return false;
    }
    
    // Read checksum
    read_result = i2c_read_blocking(i2c, GNSS_ADDR, checksum, 2, false);
    if (read_result != 2) {
        failed_reads++;
        message_errors++;
        printf("Failed to read checksum: %d/2 bytes\n", read_result);
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
    
    // Log fix type information
    if (data->fixType < 3) {
        fix_attempts++;
        if (fix_attempts % 10 == 0) {
            printf("Still waiting for fix: type=%d (%s), sats=%d (attempts=%lu)\n", 
                   data->fixType, get_fix_type_str(data->fixType), data->numSV, fix_attempts);
        }
    } else {
        // If we get a fix, reset the counter
        fix_attempts = 0;
    }
    
    data->lon = pvt_data[24] | (pvt_data[25] << 8) | (pvt_data[26] << 16) | (pvt_data[27] << 24);
    data->lat = pvt_data[28] | (pvt_data[29] << 8) | (pvt_data[30] << 16) | (pvt_data[31] << 24);
    data->height = pvt_data[32] | (pvt_data[33] << 8) | (pvt_data[34] << 16) | (pvt_data[35] << 24);
    data->hMSL = pvt_data[36] | (pvt_data[37] << 8) | (pvt_data[38] << 16) | (pvt_data[39] << 24);
    
    data->velN = pvt_data[48] | (pvt_data[49] << 8) | (pvt_data[50] << 16) | (pvt_data[51] << 24);
    data->velE = pvt_data[52] | (pvt_data[53] << 8) | (pvt_data[54] << 16) | (pvt_data[55] << 24);
    data->velD = pvt_data[56] | (pvt_data[57] << 8) | (pvt_data[58] << 16) | (pvt_data[59] << 24);
    data->gSpeed = pvt_data[60] | (pvt_data[61] << 8) | (pvt_data[62] << 16) | (pvt_data[63] << 24);
    
    successful_reads++;
    return true;
}

const char* get_fix_type_str(uint8_t fix_type) {
    switch(fix_type) {
        case 0: return "No fix";
        case 1: return "DR";
        case 2: return "2D";
        case 3: return "3D";
        case 4: return "DR+GNSS";
        default: return "?";
    }
}

bool init_sd_card() {
    printf("Initializing SD card...\n");
    FRESULT fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        printf("Failed to mount SD card: %d\n", fr);
        return false;
    }
    printf("SD card mounted\n");
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

bool write_data_to_sd(const ubx_pvt_data_t *data, uint32_t system_timestamp_ms) {
    char buffer[LOG_BUFFER_SIZE];
    uint32_t sys_seconds = system_timestamp_ms / 1000;
    uint32_t sys_ms = system_timestamp_ms % 1000;
    
    // Calculate milliseconds
    uint32_t current_time_ms = to_ms_since_boot(get_absolute_time());
    uint16_t milliseconds = current_time_ms % 1000;
    
    // Format the data as CSV with milliseconds included
    int len = snprintf(buffer, LOG_BUFFER_SIZE,
        "%02d:%02d,%02d:%02d:%02d:%03d,%.7f,%.7f,%.2f,%.2f,%.2f,%.2f,%.2f,%s,%d\n",
        sys_seconds, sys_ms,
        data->hour, data->min, data->sec, milliseconds,
        data->lat * 1e-7, data->lon * 1e-7,
        data->hMSL / 1000.0,
        data->velN / 1000.0, data->velE / 1000.0, data->velD / 1000.0,
        data->gSpeed / 1000.0,
        get_fix_type_str(data->fixType),
        data->numSV);
    
    UINT bw;
    FRESULT fr = f_write(&data_file, buffer, len, &bw);
    
    if (fr != FR_OK || bw != len) {
        failed_writes++;
        printf("Failed to write data: error=%d, written=%d, expected=%d\n", fr, bw, len);
        return false;
    }
    
    // Sync every 25 samples (about once per second)
    if (successful_writes % 25 == 0) {
        fr = f_sync(&data_file);
        if (fr != FR_OK) {
            printf("Failed to sync data file: %d\n", fr);
            return false;
        }
    }
    
    successful_writes++;
    return true;
}

void print_status_line(const ubx_pvt_data_t *data) {
    // Enhanced status line with more debug info
    printf("Time: %02d:%02d:%02d | Pos: %.5f, %.5f | Fix: %s | Sats: %d | Log: %lu/%lu | Errors: I2C=%lu Sync=%lu Msg=%lu\r",
        data->hour, data->min, data->sec,
        data->lat * 1e-7, data->lon * 1e-7,
        get_fix_type_str(data->fixType),
        data->numSV,
        successful_writes, total_reads,
        i2c_errors, sync_errors, message_errors);
}

void print_debug_info(const ubx_pvt_data_t *data) {
    // Dump detailed GNSS info
    printf("\n--- GNSS Debug Info ---\n");
    printf("Date/Time: %04d-%02d-%02d %02d:%02d:%02d\n", 
           data->year, data->month, data->day, data->hour, data->min, data->sec);
    printf("Position: Lat=%.7f, Lon=%.7f, Alt=%.2fm\n", 
           data->lat * 1e-7, data->lon * 1e-7, data->hMSL / 1000.0);
    printf("Velocity: N=%.2f, E=%.2f, D=%.2f, GSpeed=%.2f m/s\n",
           data->velN / 1000.0, data->velE / 1000.0, data->velD / 1000.0, data->gSpeed / 1000.0);
    printf("Fix: %s, Satellites: %d\n", get_fix_type_str(data->fixType), data->numSV);
    printf("Raw values: lat=%ld, lon=%ld, height=%ld, hMSL=%ld\n",
           data->lat, data->lon, data->height, data->hMSL);
    printf("Stats: Reads=%lu/%lu, Writes=%lu/%lu\n",
           successful_reads, total_reads, successful_writes, failed_writes);
    printf("Errors: I2C=%lu, Sync=%lu, Msg=%lu, Fix attempts=%lu\n",
           i2c_errors, sync_errors, message_errors, fix_attempts);
    printf("-----------------------\n");
}

int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

void blink_task(int *freq, bool *on) {
    while (*on) {
        pico_set_led(true);
        sleep_ms(*freq);
        pico_set_led(false);
        sleep_ms(*freq);
    }
    pico_set_led(false);
}

void blink_core1_entry() {
    // Get the parameters from core 0
    int *freq = (int*)multicore_fifo_pop_blocking();
    bool *on = (bool*)multicore_fifo_pop_blocking();
    
    // Run the blink task
    blink_task(freq, on);
}

int main() {
    stdio_init_all();
    
    // Important: Give enough time for UART to initialize
    sleep_ms(5000); // Reduced from 20000 to 5000, still plenty of time
    
    printf("\nGNSS Data Logger - 25Hz with diagnostics\n");
    printf("Starting in 2 seconds...\n");
    sleep_ms(2000);
    
    // Initialize I2C for GNSS module
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    printf("I2C Initialized\n");

    // Test I2C communication with the GNSS module
    printf("Testing GNSS module presence...\n");
    uint8_t dummy_reg = 0;
    int result = i2c_read_blocking(i2c_default, GNSS_ADDR, &dummy_reg, 1, false);
    if (result < 0) {
        printf("GNSS module not found on I2C bus (error %d)!\n", result);
        printf("Possible issues:\n");
        printf("- Incorrect wiring\n");
        printf("- Module not powered\n");
        printf("- Wrong I2C address (using 0x%02X)\n", GNSS_ADDR);
        // Blink LED rapidly to indicate error
        while (1) {
            pico_set_led(true);
            sleep_ms(100);
            pico_set_led(false);
            sleep_ms(100);
        }
    } else {
        printf("GNSS module detected on I2C bus\n");
    }

    pico_led_init();

    int blink_freq = 1000; 
    bool blinking = true;
    
    // Create a second core task for blinking
    multicore_launch_core1(blink_core1_entry);
    
    // Pass the frequency and on/off variables to core 1
    multicore_fifo_push_blocking((uint32_t)&blink_freq);
    multicore_fifo_push_blocking((uint32_t)&blinking);

    // Initialize SD card
    if (!init_sd_card()) {
        printf("SD card failed\n");
        // Indicate error with different blink pattern
        blink_freq = 250;
        while (1) tight_loop_contents();
    }
    
    // Variables for timing
    ubx_pvt_data_t pvt_data;
    uint32_t next_sample_time = 0;
    uint32_t next_status_time = 0;
    uint32_t next_debug_time = 0;
    
    // Set the recording time limit (80 seconds)
    uint32_t start_time = 0;
    uint32_t time_limit = 80 * 1000;  // 80 seconds in milliseconds
    bool time_limit_started = false;  // Flag to indicate if the 3D fix has been obtained

    printf("Logging started - waiting for GNSS fix\n");
    
    uint32_t last_attempt_time = 0;
    
    while (true) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // Collect a sample - implement rate limiting
        if (current_time >= next_sample_time) {
            next_sample_time = current_time + LOG_INTERVAL_MS;
            
            // Track how long it takes to read a message
            uint32_t read_start_time = to_ms_since_boot(get_absolute_time());
            bool read_success = read_ubx_message(i2c_default, &pvt_data);
            uint32_t read_end_time = to_ms_since_boot(get_absolute_time());
            uint32_t read_duration = read_end_time - read_start_time;
            
            // Log if reading takes too long
            if (read_duration > 30) {
                printf("Warning: GNSS read took %lums\n", read_duration);
            }
            
            if (read_success) {
                // Print detailed debug info periodically (every 5 seconds)
                if (current_time >= next_debug_time) {
                    print_debug_info(&pvt_data);
                    next_debug_time = current_time + 5000;
                }
                
                // Only log if we have a valid position
                if (pvt_data.fixType >= 3) {  // 3D fix is fixType 3 or higher
                    if (!time_limit_started) {
                        // Start the timer once we get the first 3D fix
                        time_limit_started = true;
                        start_time = current_time;  // Start counting time from here
                        blink_freq = 50;
                        printf("3D fix obtained! Starting logging...\n");
                        
                        // Create the file with a date-time based filename
                        if (!create_data_file(&pvt_data)) {
                            printf("File creation failed\n");
                            blink_freq = 150;  // Indicate error
                            while (1) tight_loop_contents();
                        }
                    }
                    
                    // Log data to SD card
                    if (time_limit_started && !write_data_to_sd(&pvt_data, current_time)) {
                        printf("Failed to write data to SD card\n");
                        // Continue trying rather than stopping
                    }
                } else if (current_time - last_attempt_time > 1000) {
                    // If we don't have a fix, report status less frequently
                    last_attempt_time = current_time;
                    printf("Waiting for 3D fix, current fix: %s, satellites: %d\n", 
                           get_fix_type_str(pvt_data.fixType), pvt_data.numSV);
                }
                
                // Display status update (throttled to reduce serial traffic)
                if (current_time >= next_status_time) {
                    print_status_line(&pvt_data);
                    next_status_time = current_time + STATUS_INTERVAL_MS;
                }
            } else {
                if (current_time - last_attempt_time > 1000) {
                    last_attempt_time = current_time;
                    printf("Failed to read GNSS data - retrying...\n");
                }
            }
        }
        
        // Check if we've exceeded the time limit after the first 3D fix
        if (time_limit_started && current_time - start_time >= time_limit) {
            blink_freq = 500;
            printf("\nTime limit reached, stopping logging...\n");
            printf("Session summary: Reads=%lu/%lu, Writes=%lu/%lu\n",
                   successful_reads, total_reads, successful_writes, failed_writes);
            printf("Errors: I2C=%lu, Sync=%lu, Msg=%lu\n",
                   i2c_errors, sync_errors, message_errors);
            break;  // Exit the loop after 80 seconds
        }

        // Small sleep to prevent tight loop when waiting for next sample time
        sleep_us(500);
    }
    
    // Close the data file and unmount the SD card
    f_close(&data_file);
    f_unmount("0:");
    
    // Final blink pattern to indicate completion
    blink_freq = 1000;
    sleep_ms(3000);
    blinking = false;
    
    return 0;
}