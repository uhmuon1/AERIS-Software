// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include <stdio.h>
// #include <string.h>

// #define GPS_I2C i2c0
// #define GPS_I2C_ADDR 0x42  // Typical I2C address for Ublox GPS module (this can vary)
// #define I2C_BAUD 100000  // I2C baud rate (400 kHz)

// #define GPS_BUFFER_SIZE 1024
// char gps_buffer[GPS_BUFFER_SIZE];
// int buffer_index = 0;

// // Thing Plus RP2040 Qwiic pins
// #define SDA_PIN 4
// #define SCL_PIN 5



// // Function to read data from the GPS via I2C
// void read_gps_data() {

        
//     // Setup
//     // printf("Initializing I2C...\n");
//     // i2c_init(GPS_I2C, 100000);  // 100kHz

//     // // Setup pins with validation prints
//     // gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     // gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     // gpio_pull_up(SDA_PIN);
//     // gpio_pull_up(SCL_PIN);

//     // printf("Pin functions set:\n");
//     // printf("SDA (GPIO4) function: %d\n", gpio_get_function(SDA_PIN));
//     // printf("SCL (GPIO5) function: %d\n", gpio_get_function(SCL_PIN));

//     // // Try direct communication with NEO-M9N
//     // uint8_t rxdata;
//     // int ret = i2c_read_blocking(GPS_I2C, 0x42, &rxdata, 1, false);
//     // printf("Direct NEO-M9N read result: %d\n", ret);

//     // // Then do full scan
//     // for (uint8_t addr = 0; addr < 128; addr++) {
//     //     ret = i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false);
//     //     if (ret >= 0) {
//     //         printf("Device responded at address: 0x%02X\n", addr);
//     //     }
//     // }

//     // printf("Attempting to read data \n");

//     // uint8_t buffer[2];
//     // // int ret;

//     // uint8_t reg = 0x01;
//     // ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, &reg, 1,true );
//     // ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, buffer, 2, false);

//     // printf("read result: %d, class: 0x%02X, ID: 0x%02X\n", ret, buffer[0], buffer[1]);

//     // for (uint8_t addr = 0; addr < 128; addr++) {
//     //     // uint8_t rxdata;
//     //     if (i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false) >= 0) {
//     //         printf("Found I2C device at address: 0x%02X\n", addr);
//     //     }
//     // }

//     // uint8_t reg = 0x01;  // Example register address
//     // int ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, &reg, 1, true);  // Write the register address
//     // if (ret < 0) {
//     //     printf("I2C Write failed: %d\n", ret);
//     //     return;  // Handle error
//     // }

//     // uint8_t rxdata;
//     // printf("Scanning I2C bus...\n");
//     // for (uint8_t addr = 0; addr < 128; addr++) {
//     //     int ret = i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false);
//     //     if (ret >= 0) {
//     //         printf("Device responded at address: 0x%02X\n", addr);
//     //     }
//     // }

//     uint8_t data[64]; // Buffer to store incoming data
//     int length = i2c_read_blocking(GPS_I2C, GPS_I2C_ADDR, data, sizeof(data), false);

//     printf("GPS i2c: %x\n", GPS_I2C);
//     printf("GPS i2c: %x\n", *GPS_I2C);
    
//     if (length > 0) {
//         // Process the received data
//         for (int i = 0; i < length; i++) {
//             char byte = data[i];

//             // Store the byte in the buffer
//             if (byte == '\n') {
//                 gps_buffer[buffer_index] = '\0';  // Null-terminate the string
//                 // Process NMEA sentence
//                 printf("Received NMEA: %s\n", gps_buffer);
//                 buffer_index = 0;  // Reset buffer for next sentence
//             } else {
//                 gps_buffer[buffer_index++] = byte;
//                 // Make sure we don't overflow the buffer
//                 if (buffer_index >= GPS_BUFFER_SIZE - 1) {
//                     buffer_index = 0;  // Reset buffer to avoid overflow
//                 }
//             }
//         }
//     }
// }

// // Setup I2C
// void setup_i2c() {
//     // Initialize I2C at the desired baud rate
//     i2c_init(GPS_I2C, I2C_BAUD);

//     // Set the I2C pins (GPIO0 for SDA, GPIO1 for SCL)
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

//     // Enable internal pullups on the SDA and SCL pins
//     gpio_pull_up(SDA_PIN);
//     gpio_pull_up(SCL_PIN);
// }

// int main() {
//     // Initialize the stdio (for debugging)
//     stdio_init_all();
    
//     // Setup I2C communication
//     setup_i2c();
    
//     printf("GPS I2C Logger Started!\n");

//     // Main loop
//     while (true) {
//         read_gps_data();
//         sleep_ms(1000);  // Wait a little before reading again
//     }

//     return 0;
// }






// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include <stdio.h>
// #include <string.h>

// #define GPS_I2C i2c0
// #define GPS_I2C_ADDR 0x42  // NEO-M9N default address
// #define I2C_BAUD 115200    // Starting with 100kHz for reliability

// // Thing Plus RP2040 Qwiic pins
// #define SDA_PIN 0
// #define SCL_PIN 1

// // UBX protocol sync chars
// #define UBX_SYNC_1 0xB5
// #define UBX_SYNC_2 0x62

// // Function to scan I2C bus and print found devices
// void scan_i2c_bus() {
//     printf("\nScanning I2C bus...\n");
//     for (uint8_t addr = 0; addr < 128; addr++) {
//         uint8_t rxdata;
//         int ret = i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false);
//         if (ret >= 0) {
//             printf("* Device found at address: 0x%02X (decimal: %d)\n", addr, addr);
//             // If we found the NEO-M9N
//             if (addr == GPS_I2C_ADDR) {
//                 printf("  --> This appears to be the NEO-M9N!\n");
//             }
//         }
//     }
//     printf("I2C scan complete\n\n");
// }

// // Try to read the NEO-M9N version info
// bool test_gps_communication() {
//     uint8_t reg = 0xFD; // Register containing bytes available
//     uint8_t bytes_available[2];
    
//     printf("Testing NEO-M9N communication...\n");
    
//     // Try to read the bytes available register
//     int ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, &reg, 1, true);
//     if (ret < 0) {
//         printf("Failed to write to NEO-M9N\n");
//         return false;
//     }
    
//     ret = i2c_read_blocking(GPS_I2C, GPS_I2C_ADDR, bytes_available, 2, false);
//     if (ret < 0) {
//         printf("Failed to read from NEO-M9N\n");
//         return false;
//     }
    
//     uint16_t available = (bytes_available[0] << 8) | bytes_available[1];
//     printf("Bytes available in GPS: %d\n", available);
    
//     return true;
// }

// // Setup I2C with validation
// bool setup_i2c() {
//     printf("Starting I2C setup...\n");
    
//     // Initialize I2C
//     i2c_init(GPS_I2C, I2C_BAUD);
//     printf("I2C initialized at %d Hz\n", I2C_BAUD);

//     // Configure pins
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(SDA_PIN);
//     gpio_pull_up(SCL_PIN);
    
//     // Verify pin configuration
//     printf("Pin configuration:\n");
//     printf("* SDA (GPIO%d) function: %d (should be 3 for I2C)\n", 
//            SDA_PIN, gpio_get_function(SDA_PIN));
//     printf("* SCL (GPIO%d) function: %d (should be 3 for I2C)\n", 
//            SCL_PIN, gpio_get_function(SCL_PIN));
    
//     // Validate pin functions
//     if (gpio_get_function(SDA_PIN) != GPIO_FUNC_I2C || 
//         gpio_get_function(SCL_PIN) != GPIO_FUNC_I2C) {
//         printf("ERROR: Pins not properly configured for I2C!\n");
//         return false;
//     }
    
//     printf("I2C setup complete\n");
//     return true;
// }

// int main() {
//     // Initialize stdio for debugging output
//     stdio_init_all();
    
//     // Wait a moment for serial to connect
//     sleep_ms(2000);
//     printf("\n\nNEO-M9N I2C Debug Program Starting...\n");
    
//     // Setup I2C
//     if (!setup_i2c()) {
//         printf("Failed to setup I2C. Halting.\n");
//         while(1) { sleep_ms(1000); }
//     }
    
//     // Main loop
//     while (true) {
//         gpio_init(25);
//         gpio_set_dir(25, GPIO_OUT);
//         gpio_put(25,1);
//         sleep_ms(500);
//         gpio_put(25,0);
//         sleep_ms(500);

//         scan_i2c_bus();
        
//         if (test_gps_communication()) {
//             printf("Successfully communicated with NEO-M9N!\n");
//         } else {
//             printf("Failed to communicate with NEO-M9N\n");
//         }
        
//         printf("\nWaiting 5 seconds before next attempt...\n");
//         sleep_ms(5000);
//     }

//     return 0;
// }




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
    
    printf("Attempting I2C write to address 0x%02X with register 0x%02X\n", GPS_ADDR, reg);
    
    // Check if bus is busy first
    if (i2c_get_write_available(GPS_I2C) == 0) {
        printf("I2C bus is busy or not ready\n");
        return;
    }
    
    int ret = i2c_write_blocking(GPS_I2C, GPS_ADDR, &reg, 1, true);
    printf("Write returned: %d\n", ret);
    
    if (ret < 0) {
        // Try to reset the I2C peripheral
        i2c_deinit(GPS_I2C);
        sleep_ms(100);
        i2c_init(GPS_I2C, 100000);
        printf("Attempted I2C reset after error\n");
        return;
    }
    // ... rest of your function
}


int main() {
    stdio_init_all();
    stdio_usb_init();
    sleep_ms(3000);
    
    // Initialize LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    // First set up the pins
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    
    // Give the pull-ups time to stabilize
    sleep_ms(250);
    
    // Now initialize I2C with a slower clock for testing
    printf("Initializing I2C...\n");
    i2c_init(GPS_I2C, 50000);  // Try 50kHz
    
    // Check if I2C initialized successfully
    if (!i2c_get_write_available(GPS_I2C)) {
        printf("I2C failed to initialize properly\n");
        return -1;
    }
    
    printf("I2C initialized successfully\n");
    printf("Starting GPS read loop...\n");
    
    while (true) {
        gpio_put(LED_PIN, 1);
        read_gps_data();
        sleep_ms(1000);  // Longer delay for testing
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
    }
    
    return 0;
}