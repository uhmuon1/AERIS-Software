#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include <string.h>

#define GPS_I2C i2c0
#define GPS_I2C_ADDR 0x42  // Typical I2C address for Ublox GPS module (this can vary)
#define I2C_BAUD 400000  // I2C baud rate (400 kHz)

#define GPS_BUFFER_SIZE 1024
char gps_buffer[GPS_BUFFER_SIZE];
int buffer_index = 0;

// Function to read data from the GPS via I2C
void read_gps_data() {
    uint8_t data[64]; // Buffer to store incoming data
    int length = i2c_read_blocking(GPS_I2C, GPS_I2C_ADDR, data, sizeof(data), false);
    
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
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);

    // Enable internal pullups on the SDA and SCL pins
    gpio_pull_up(0);
    gpio_pull_up(1);
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
        sleep_ms(100);  // Wait a little before reading again
    }

    return 0;
}