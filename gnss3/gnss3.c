// #include <stdio.h>
// #include <string.h>
// #include "pico/stdlib.h"
// #include "hardware/i2c.h"

// #define MAX_NMEA_LENGTH 82  // Maximum length of an NMEA sentence

// // UBX Sync bytes
// #define UBX_SYNC1 0xB5
// #define UBX_SYNC2 0x62

// // UBX message types
// #define UBX_CFG_PRT 0x06  // CFG-PRT
// #define UBX_CFG_MSG 0x01  // CFG-MSG

// // I2C address of the GPS (0x42 for most UBlox devices)
// #define GPS_I2C_ADDR 0x42

// typedef struct {
//     double latitude;
//     double longitude;
//     double altitude;
//     uint8_t satellites;
//     uint8_t fix_type;
//     double velocity_east;
//     double velocity_north;
//     double velocity_up;
//     int32_t x;
//     int32_t y;
//     int32_t z;
// } gnss_data_t;

// // // Function to send UBX message via I2C
// // void send_ubx_message(i2c_inst_t *i2c, uint8_t *message, size_t length) {
// //     i2c_write_blocking(i2c, GPS_I2C_ADDR, message, length, false);
// // }


// // Function to send a UBX message
// void send_ubx_message(i2c_inst_t *i2c, uint8_t *message, size_t length) {
//     for (size_t i = 0; i < length; i++) {
//         i2c_write_blocking(i2c, 0x42, &message[i], 1, false);
//     }
// }

// // Function to configure GPS to output UBX messages
// void configure_gps() {
//     uint8_t cfg_prt_message[] = {
//         UBX_SYNC1, UBX_SYNC2,   // Sync bytes
//         0x06, 0x00,             // Message class (CFG), message ID (PRT)
//         0x14, 0x00,             // Length of payload (20 bytes)
//         0x01, 0x00, 0x00, 0x00, // Port ID (I2C)
//         0x00, 0x00,             // Reserved
//         0x00, 0x00, 0x00, 0x00  // Baud rate (9600)
//     };

//     uint8_t cfg_msg_message[] = {
//         UBX_SYNC1, UBX_SYNC2,   // Sync bytes
//         0x01, 0x01,             // Message class (CFG), message ID (MSG)
//         0x06, 0x00,             // Length of payload (6 bytes)
//         0x01, 0x00,             // Disable NMEA messages (0x01 is NAV-PVT)
//         0x01, 0x01              // Enable NAV-PVT (0x01 0x01 for NAV-PVT)
//     };

//     uint8_t cfg_msg_disable_nmea[] = {
//         UBX_SYNC1, UBX_SYNC2,       // Sync bytes
//         0x01, 0x01,                 // Message class (CFG), message ID (MSG)
//         0x06, 0x00,                 // Length of payload (6 bytes)
//         0x00, 0x00,                 // Disable all NMEA sentences
//         0x01, 0x01                  // Enable NAV-PVT UBX message
//     };

//     send_ubx_message(i2c_default, cfg_msg_disable_nmea, sizeof(cfg_msg_disable_nmea));

//     // Send CFG-PRT message to configure GPS output to I2C
//     send_ubx_message(i2c_default, cfg_prt_message, sizeof(cfg_prt_message));
//     sleep_ms(100);  // Small delay after CFG-PRT

//     // Send CFG-MSG message to enable NAV-PVT
//     send_ubx_message(i2c_default, cfg_msg_message, sizeof(cfg_msg_message));
//     sleep_ms(100);  // Small delay after CFG-MSG

//     printf("GPS configured to output UBX NAV-PVT message.\n");
// }


// // Function to parse UBX PVT (Position, Velocity, Time) message
// void parse_ubx_pvt(uint8_t *buffer, gnss_data_t *data) {
//     // Extract latitude, longitude, altitude
//     data->latitude = *((int32_t*)(&buffer[24])) / 1E7;  // Latitude in degrees
//     data->longitude = *((int32_t*)(&buffer[28])) / 1E7; // Longitude in degrees
//     data->altitude = *((int32_t*)(&buffer[32])) / 1000.0; // Altitude in meters

//     // Velocity in mm/s, convert to m/s
//     data->velocity_east = *((int32_t*)(&buffer[44])) / 1000.0; // Velocity east
//     data->velocity_north = *((int32_t*)(&buffer[48])) / 1000.0; // Velocity north
//     data->velocity_up = *((int32_t*)(&buffer[52])) / 1000.0; // Velocity up

//     // ECF coordinates (x, y, z) in meters
//     data->x = *((int32_t*)(&buffer[56])); // ECF X coordinate
//     data->y = *((int32_t*)(&buffer[60])); // ECF Y coordinate
//     data->z = *((int32_t*)(&buffer[64])); // ECF Z coordinate
// }

// // Function to read a UBX message
// bool read_ubx_message(i2c_inst_t *i2c, uint8_t *buffer, size_t buffer_size) {
//     size_t index = 0;
//     uint8_t byte;

//     // Read the UBX message byte by byte
//     while (index < buffer_size) {
//         if (i2c_read_blocking(i2c, GPS_I2C_ADDR, &byte, 1, false) != 1) {
//             return false;
//         }

//         // Print the byte being read for debugging
//         printf("Read byte: 0x%02X\n", byte);

//         buffer[index++] = byte;

//         // Check for sync bytes (0xB5 0x62)
//         if (index > 1 && buffer[index - 2] == 0xB5 && buffer[index - 1] == 0x62) {
//             // We found the start of the UBX message, now get the length of the message
//             printf("Sync bytes found!\n");

//             if (index >= 4) {
//                 uint16_t len = buffer[2] | (buffer[3] << 8);  // Length of the UBX message (little-endian)
//                 // Check if we have the complete message based on length
//                 if (index >= 4 + len) {
//                     printf("Complete UBX message received!\n");
//                     return true;
//                 }
//             }
//         }
//     }

//     // If we exit the loop, no complete UBX message was received
//     return false;
// }

// int main() {
//     stdio_init_all();

//     // Initialize I2C (adjust the I2C instance, pins, and baud rate as needed)
//     i2c_init(i2c_default, 100 * 1000); // 100kHz baud rate
//     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
//     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

//     gnss_data_t gnss_data;
//     uint8_t ubx_buffer[128]; // Buffer for UBX messages

//     // Configure GPS to output UBX messages
//     configure_gps();

//     while (true) {
//         // Read the UBX message
//         if (read_ubx_message(i2c_default, ubx_buffer, sizeof(ubx_buffer))) {
//             printf("Received UBX message: ");
//             for (int i = 0; i < 10; i++) {
//                 printf("0x%02X ", ubx_buffer[i]);
//             }
//             printf("\n");

//             // Parse the UBX message (specifically NAV-PVT)
//             parse_ubx_pvt(ubx_buffer, &gnss_data);

//             // Print extracted GNSS data
//             printf("GNSS Data:\n");
//             printf("Latitude: %f\n", gnss_data.latitude);
//             printf("Longitude: %f\n", gnss_data.longitude);
//             printf("Altitude: %f m\n", gnss_data.altitude);
//             printf("Velocity East: %f m/s\n", gnss_data.velocity_east);
//             printf("Velocity North: %f m/s\n", gnss_data.velocity_north);
//             printf("Velocity Up: %f m/s\n", gnss_data.velocity_up);
//             printf("ECF X: %d\n", gnss_data.x);
//             printf("ECF Y: %d\n", gnss_data.y);
//             printf("ECF Z: %d\n", gnss_data.z);
//         } else {
//             printf("Failed to read UBX message\n");
//         }

//         sleep_ms(1000); // Delay before reading the next message
//     }

//     return 0;
// }

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define GNSS_ADDR 0x42

// Configuration message to switch to UBX-03 protocol
const uint8_t ubx_cfg_prt[] = {
    0xB5, 0x62,           // Sync chars
    0x06, 0x00,           // CFG-PRT message
    0x14, 0x00,           // Length (20 bytes)
    0x00,                 // Port ID (I2C)
    0x00,                 // Reserved
    0x00, 0x00,           // TX ready
    0x84, 0x00, 0x00, 0x00, // Mode (8N1)
    0x00, 0x00, 0x00, 0x00, // Baudrate (not used for I2C)
    0x03, 0x00,           // inProtoMask (UBX-03 protocol)
    0x03, 0x00,           // outProtoMask (UBX-03 protocol)
    0x00, 0x00,           // Flags
    0x00, 0x00,           // Reserved
    0x86, 0x8C            // Checksum
};

// Enable NAV-PVT messages (for position, velocity, time)
const uint8_t ubx_cfg_msg[] = {
    0xB5, 0x62,           // Sync chars
    0x06, 0x01,           // CFG-MSG
    0x08, 0x00,           // Length
    0x01,                 // Class
    0x07,                 // ID (NAV-PVT)
    0x01,                 // Rate on I2C
    0x00,                 // Rate on UART1
    0x00,                 // Rate on UART2
    0x00,                 // Rate on USB
    0x00,                 // Rate on SPI
    0x00,                 // Rate on reserved
    0x1D, 0xE5            // Checksum
};

// Save configuration to flash
const uint8_t ubx_cfg_save[] = {
    0xB5, 0x62,           // Sync chars
    0x06, 0x09,           // CFG-CFG
    0x0D, 0x00,           // Length
    0x00, 0x00, 0x00, 0x00, // Clear mask
    0xFF, 0xFF, 0x00, 0x00, // Save mask
    0x00, 0x00, 0x00, 0x00, // Load mask
    0x01,                 // Device mask (BBR)
    0x1B, 0xA5            // Checksum
};

bool send_config_message(i2c_inst_t *i2c, const uint8_t *message, size_t length) {
    printf("Sending message (%d bytes): ", length);
    for(size_t i = 0; i < length; i++) {
        printf("%02X ", message[i]);
    }
    printf("\n");
    
    int bytes_written = i2c_write_blocking(i2c, GNSS_ADDR, message, length, false);
    bool success = (bytes_written == length);
    printf("%s (wrote %d/%d bytes)\n", success ? "Success" : "Failed", bytes_written, length);
    
    return success;
}

int main() {
    stdio_init_all();
    sleep_ms(20000);  // Wait for serial to initialize
    
    printf("\nStarting NEO-M9N UBX-03 Configuration\n");
    printf("-------------------------------------\n");
    
    // Initialize I2C
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    printf("I2C Initialized\n");
    
    // Step 1: Configure for UBX-03 protocol
    printf("\n1. Configuring for UBX-03 protocol...\n");
    if (!send_config_message(i2c_default, ubx_cfg_prt, sizeof(ubx_cfg_prt))) {
        printf("Failed to configure UBX-03 protocol\n");
        return 1;
    }
    sleep_ms(100);
    
    // Step 2: Enable NAV-PVT messages
    printf("\n2. Enabling NAV-PVT messages...\n");
    if (!send_config_message(i2c_default, ubx_cfg_msg, sizeof(ubx_cfg_msg))) {
        printf("Failed to enable NAV-PVT messages\n");
        return 1;
    }
    sleep_ms(100);
    
    // Step 3: Save configuration
    printf("\n3. Saving configuration to flash...\n");
    if (!send_config_message(i2c_default, ubx_cfg_save, sizeof(ubx_cfg_save))) {
        printf("Failed to save configuration\n");
        return 1;
    }
    
    printf("\nConfiguration complete!\n");
    printf("Please reset the device for changes to take effect.\n");
    printf("After reset, the device should output UBX-03 protocol messages.\n");
    
    // Monitor for acknowledgment
    uint8_t response[100];
    printf("\nMonitoring for response...\n");
    while(1) {
        int bytes_read = i2c_read_blocking(i2c_default, GNSS_ADDR, response, 1, false);
        if (bytes_read > 0) {
            printf("Received: 0x%02X\n", response[0]);
        }
        sleep_ms(100);
    }
    
    return 0;
}
