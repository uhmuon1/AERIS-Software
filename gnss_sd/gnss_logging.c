
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

bool send_poll_request(i2c_inst_t *i2c) {
    int result = i2c_write_blocking(i2c, GNSS_ADDR, ubx_nav_pvt_poll, sizeof(ubx_nav_pvt_poll), false);
    return (result == sizeof(ubx_nav_pvt_poll));
}

bool read_byte(i2c_inst_t *i2c, uint8_t *byte) {
    return (i2c_read_blocking(i2c, GNSS_ADDR, byte, 1, false) == 1);
}

bool wait_for_sync(i2c_inst_t *i2c) {
    uint8_t byte;
    int attempts = 0;
    
    while (attempts < 100) {
        if (!read_byte(i2c, &byte)) return false;
        if (byte == UBX_SYNC1) {
            if (!read_byte(i2c, &byte)) return false;
            if (byte == UBX_SYNC2) return true;
        }
        attempts++;
    }
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
    if (i2c_read_blocking(i2c, GNSS_ADDR, header, 4, false) != 4) {
        failed_reads++;
        return false;
    }
    
    // Verify this is a NAV-PVT message
    if (header[0] != NAV_CLASS || header[1] != NAV_PVT_ID) {
        failed_reads++;
        return false;
    }
    
    uint16_t length = header[2] | (header[3] << 8);
    if (length != 92) {
        failed_reads++;
        return false;
    }
    
    // Read PVT data
    if (i2c_read_blocking(i2c, GNSS_ADDR, pvt_data, length, false) != length) {
        failed_reads++;
        return false;
    }
    
    // Read checksum
    if (i2c_read_blocking(i2c, GNSS_ADDR, checksum, 2, false) != 2) {
        failed_reads++;
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
        get_fix_type_str(data->fixType),
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

void print_status_line(const ubx_pvt_data_t *data) {
    // Use simple, single-line status updates instead of clearing the screen
    printf("Time: %02d:%02d:%02d | Pos: %.5f, %.5f | Fix: %s | Sats: %d | Log: %lu/%lu\r",
        data->hour, data->min, data->sec,
        data->lat * 1e-7, data->lon * 1e-7,
        get_fix_type_str(data->fixType),
        data->numSV,
        successful_writes, total_reads);
    
    // Very important: don't use fflush() excessively as it can cause issues with some
    // serial terminals. Let the terminal handle buffering.
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

int main() {
    stdio_init_all();
    
    // Important: Give enough time for UART to initialize
    sleep_ms(20000);
    
    printf("\nGNSS Data Logger - 25Hz\n");
    printf("Starting in 2 seconds...\n");
    sleep_ms(2000);
    
    // Initialize I2C for GNSS module
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    printf("I2C Initialized\n");

    pico_led_init();

    // int i2cfreq = 50;
    int waitfreq = 1000;
    int fixedfreq = 25;
    int logfreq = 1000;
    int donefreq = 1000;

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
        while (1) tight_loop_contents();
    }
    
    // Variables for timing
    ubx_pvt_data_t pvt_data;
    uint32_t next_sample_time = 0;
    uint32_t next_status_time = 0;
    
    // Set the recording time limit (80 seconds)
    uint32_t start_time = 0;
    uint32_t time_limit = 80 * 1000;  // 80 seconds in milliseconds
    bool time_limit_started = false;  // Flag to indicate if the 3D fix has been obtained

    printf("Logging started\n");
    
    while (true) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // Collect a sample
        if (current_time >= next_sample_time) {
            next_sample_time = current_time + LOG_INTERVAL_MS;
            // blink_freq = 1000;
            if (read_ubx_message(i2c_default, &pvt_data)) {
                // Only log if we have a valid position
                if (pvt_data.fixType >= 3) {  // 3D fix is fixType 3 or higher
                    if (!time_limit_started) {
                        // Start the timer once we get the first 3D fix
                        
                        time_limit_started = true;
                        start_time = current_time;  // Start counting time from here
                        blink_freq = 50;
                        // Create the file with a date-time based filename
                        if (!create_data_file(&pvt_data)) {
                            printf("File creation failed\n");
                            while (1) tight_loop_contents();
                        }
                    }
                    // blink_freq = fixedfreq;
                    write_data_to_sd(&pvt_data, current_time);
                }
                
                // Display status update (throttled to reduce serial traffic)
                if (current_time >= next_status_time) {
                    print_status_line(&pvt_data);
                    next_status_time = current_time + STATUS_INTERVAL_MS;
                }
            }
        }
        
        // Check if we've exceeded the time limit after the first 3D fix
        if (time_limit_started && current_time - start_time >= time_limit) {
            blink_freq = 3000;
            printf("Time limit reached, stopping logging...\n");
            break;  // Exit the loop after 80 seconds
        }

        // Small sleep to prevent tight loop when waiting for next sample time
        sleep_us(500);
    }
    
    // Close the data file and unmount the SD card
    f_close(&data_file);
    f_unmount("0:");
    return 0;
}

void blink_core1_entry() {
    // Get the parameters from core 0
    int *freq = (int*)multicore_fifo_pop_blocking();
    bool *on = (bool*)multicore_fifo_pop_blocking();
    
    // Run the blink task
    blink_task(freq, on);
}