#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

#define LED_PIN 25
#define SDA_PIN 4
#define SCL_PIN 5
#define GPS_I2C i2c0
#define GPS_ADDR 0x42

// NEO-M9N registers
#define DATA_STREAM_REG 0xFF
#define BYTES_AVAILABLE_REG 0xFD

void read_gps_data() {
    uint8_t bytes_available[2];
    uint8_t reg = BYTES_AVAILABLE_REG;
    
    // First read how many bytes are available
    int ret = i2c_write_blocking(GPS_I2C, GPS_ADDR, &reg, 1, true);
    if (ret < 0) {
        printf("Failed to write bytes available register command\n");
        return;
    }
    
    ret = i2c_read_blocking(GPS_I2C, GPS_ADDR, bytes_available, 2, false);
    if (ret < 0) {
        printf("Failed to read bytes available\n");
        return;
    }
    
    uint16_t num_bytes = (bytes_available[0] << 8) | bytes_available[1];
    if (num_bytes > 0) {
        printf("Bytes available: %d\n", num_bytes);
        
        // Now read the actual data
        reg = DATA_STREAM_REG;
        ret = i2c_write_blocking(GPS_I2C, GPS_ADDR, &reg, 1, true);
        if (ret < 0) {
            printf("Failed to write data stream register command\n");
            return;
        }
        
        // Read available bytes (up to 32 at a time to not overflow buffer)
        uint8_t data[32];
        uint16_t bytes_to_read = (num_bytes > 32) ? 32 : num_bytes;
        
        ret = i2c_read_blocking(GPS_I2C, GPS_ADDR, data, bytes_to_read, false);
        if (ret < 0) {
            printf("Failed to read GPS data\n");
            return;
        }
        
        printf("Read GPS data: ");
        for(int i = 0; i < bytes_to_read; i++) {
            printf("%c", data[i]);
        }
        printf("\n");
    }
}

int main() {
    stdio_init_all();
    stdio_usb_init();
    sleep_ms(3000);
    
    // Initialize LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    // Initialize I2C
    printf("Initializing I2C...\n");
    i2c_init(GPS_I2C, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    
    printf("Starting GPS read loop...\n");
    
    while (true) {
        // Flash LED to show we're alive
        gpio_put(LED_PIN, 1);
        read_gps_data();
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
    }
    
    return 0;
}
