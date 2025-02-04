#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <string.h>

#define GPS_I2C i2c0
#define GPS_I2C_ADDR 0x42  // Typical I2C address for Ublox GPS module (this can vary)
#define I2C_BAUD 100000  // I2C baud rate (400 kHz)

#define GPS_BUFFER_SIZE 1024
char gps_buffer[GPS_BUFFER_SIZE];
int buffer_index = 0;

// Thing Plus RP2040 Qwiic pins
#define SDA_PIN 4
#define SCL_PIN 5



// Function to read data from the GPS via I2C
void read_gps_data() {

        
    // Setup
    // printf("Initializing I2C...\n");
    // i2c_init(GPS_I2C, 100000);  // 100kHz

    // // Setup pins with validation prints
    // gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    // gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    // gpio_pull_up(SDA_PIN);
    // gpio_pull_up(SCL_PIN);

    // printf("Pin functions set:\n");
    // printf("SDA (GPIO4) function: %d\n", gpio_get_function(SDA_PIN));
    // printf("SCL (GPIO5) function: %d\n", gpio_get_function(SCL_PIN));

    // // Try direct communication with NEO-M9N
    // uint8_t rxdata;
    // int ret = i2c_read_blocking(GPS_I2C, 0x42, &rxdata, 1, false);
    // printf("Direct NEO-M9N read result: %d\n", ret);

    // // Then do full scan
    // for (uint8_t addr = 0; addr < 128; addr++) {
    //     ret = i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false);
    //     if (ret >= 0) {
    //         printf("Device responded at address: 0x%02X\n", addr);
    //     }
    // }

    // printf("Attempting to read data \n");

    // uint8_t buffer[2];
    // // int ret;

    // uint8_t reg = 0x01;
    // ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, &reg, 1,true );
    // ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, buffer, 2, false);

    // printf("read result: %d, class: 0x%02X, ID: 0x%02X\n", ret, buffer[0], buffer[1]);

    // for (uint8_t addr = 0; addr < 128; addr++) {
    //     // uint8_t rxdata;
    //     if (i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false) >= 0) {
    //         printf("Found I2C device at address: 0x%02X\n", addr);
    //     }
    // }

    // uint8_t reg = 0x01;  // Example register address
    // int ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, &reg, 1, true);  // Write the register address
    // if (ret < 0) {
    //     printf("I2C Write failed: %d\n", ret);
    //     return;  // Handle error
    // }

    uint8_t rxdata;
    printf("Scanning I2C bus...\n");
    for (uint8_t addr = 0; addr < 128; addr++) {
        int ret = i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false);
        if (ret >= 0) {
            printf("Device responded at address: 0x%02X\n", addr);
        }
    }

    uint8_t data[64]; // Buffer to store incoming data
    int length = i2c_read_blocking(GPS_I2C, GPS_I2C_ADDR, data, sizeof(data), false);

    printf("GPS i2c: %x\n", GPS_I2C);
    printf("GPS i2c: %x\n", *GPS_I2C);
    
    if (length > 0) {
        // Process the received data
        for (int i = 0; i < length; i++) {
            char byte = data[i];

            // Store the byte in the buffer
            if (byte == '\n') {
                gps_buffer[buffer_index] = '\0';  // Null-terminate the string
                // Process NMEA sentence
                printf("Received NMEA: %s\n", gps_buffer);
                buffer_index = 0;  // Reset buffer for next sentence
            } else {
                gps_buffer[buffer_index++] = byte;
                // Make sure we don't overflow the buffer
                if (buffer_index >= GPS_BUFFER_SIZE - 1) {
                    buffer_index = 0;  // Reset buffer to avoid overflow
                }
            }
        }
    }
}

// Setup I2C
void setup_i2c() {
    // Initialize I2C at the desired baud rate
    i2c_init(GPS_I2C, I2C_BAUD);

    // Set the I2C pins (GPIO0 for SDA, GPIO1 for SCL)
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // Enable internal pullups on the SDA and SCL pins
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
}

int main() {
    // Initialize the stdio (for debugging)
    stdio_init_all();
    
    // Setup I2C communication
    setup_i2c();
    
    printf("GPS I2C Logger Started!\n");

    // Main loop
    while (true) {
        read_gps_data();
        sleep_ms(1000);  // Wait a little before reading again
    }

    return 0;
}