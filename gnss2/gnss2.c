// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "pico/binary_info.h"
// #include "hardware/i2c.h"

// // NEO-M9N I2C address (default)
// #define GNSS_ADDR 0x42

// // UBX protocol sync chars
// #define UBX_SYNC_1 0xB5
// #define UBX_SYNC_2 0x62

// // UBX message classes
// #define UBX_CLASS_NAV 0x01
// #define UBX_CLASS_CFG 0x06

// // UBX message IDs
// #define UBX_NAV_PVT 0x07    // Navigation Position Velocity Time Solution

// // Buffer sizes
// #define MAX_PAYLOAD_SIZE 100

// typedef struct {
//     double latitude;
//     double longitude;
//     double altitude;
//     uint8_t satellites;
//     uint8_t fix_type;
// } gnss_data_t;

// int pico_led_init(void) {
//     #if defined(PICO_DEFAULT_LED_PIN)
//         // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
//         // so we can use normal GPIO functionality to turn the led on and off
//         gpio_init(PICO_DEFAULT_LED_PIN);
//         gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
//         return PICO_OK;
//     #elif defined(CYW43_WL_GPIO_LED_PIN)
//         // For Pico W devices we need to initialise the driver etc
//         return cyw43_arch_init();
//     #endif
// }
    
//     // Turn the led on or off
// void pico_set_led(bool led_on) {
//     #if defined(PICO_DEFAULT_LED_PIN)
//         // Just set the GPIO on or off
//         gpio_put(PICO_DEFAULT_LED_PIN, led_on);
//     #elif defined(CYW43_WL_GPIO_LED_PIN)
//         // Ask the wifi "driver" to set the GPIO on or off
//         cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
//     #endif
// }

// // Calculate Fletcher checksum
// void calculate_checksum(uint8_t *buffer, size_t length, uint8_t *ck_a, uint8_t *ck_b) {
//     *ck_a = 0;
//     *ck_b = 0;
    
//     for(size_t i = 0; i < length; i++) {
//         *ck_a += buffer[i];
//         *ck_b += *ck_a;
//     }
// }

// // Send UBX message
// bool send_ubx_message(i2c_inst_t *i2c, uint8_t msg_class, uint8_t msg_id, 
//                      uint8_t *payload, uint16_t length) {
//     uint8_t header[6];
//     uint8_t checksum[2];
    
//     // Header
//     header[0] = UBX_SYNC_1;
//     header[1] = UBX_SYNC_2;
//     header[2] = msg_class;
//     header[3] = msg_id;
//     header[4] = length & 0xFF;
//     header[5] = (length >> 8) & 0xFF;
    
//     // Calculate checksum
//     calculate_checksum(&header[2], 4 + length, &checksum[0], &checksum[1]);
    
//     // Send header
//     if (i2c_write_blocking(i2c, GNSS_ADDR, header, 6, false) != 6) {
//         return false;
//     }
    
//     // Send payload if any
//     if (length > 0 && payload != NULL) {
//         if (i2c_write_blocking(i2c, GNSS_ADDR, payload, length, false) != length) {
//             return false;
//         }
//     }
    
//     // Send checksum
//     if (i2c_write_blocking(i2c, GNSS_ADDR, checksum, 2, false) != 2) {
//         return false;
//     }
    
//     return true;
// }

// // Read GNSS data
// bool read_gnss_data(i2c_inst_t *i2c, gnss_data_t *data) {
//     uint8_t buffer[MAX_PAYLOAD_SIZE];
//     uint16_t length;
    
//     // Request PVT data
//     if (!send_ubx_message(i2c, UBX_CLASS_NAV, UBX_NAV_PVT, NULL, 0)) {
//         printf("Failed to send message\n");
//         return false;
//     }
    
//     // Wait for response
//     // sleep_ms(100);
    
//     // Read response and print first byte
//     if (i2c_read_blocking(i2c, GNSS_ADDR, buffer, 1, false) != 1) {
//         printf("Failed to read first byte\n");
//         return false;
//     }
//     printf("First byte: 0x%02X\n", buffer[0]);

//     uint8_t header[6];
//     if (i2c_read_blocking(i2c, GNSS_ADDR, header, 6, false) != 6) {
//         printf("header error\n");
//         return false;
//     }
//     //length = header[4] | (header[5] << 8);
//     length = buffer[0];
//     printf("Reported length: %d\n", length);
//     if (length > MAX_PAYLOAD_SIZE) {
//         printf("Length too large\n");
//         return false;
//     }
    
//     // Read rest of data and print first few bytes
//     if (i2c_read_blocking(i2c, GNSS_ADDR, buffer, length, false) != length) {
//         printf("Failed to read full message\n");
//         return false;
//     }
    
//     printf("Raw Data:\n");
//     for (int i = 0; i < length; i++) {
//         printf("0x%02X ", buffer[i]);
//     }
//     printf("\n");

    
//     // Parse PVT data according to UBX-NAV-PVT message structure
//     // Offsets based on u-blox M9 interface description
//     data->latitude = ((double)((int32_t)((buffer[27] << 24) | (buffer[26] << 16) | 
//                     (buffer[25] << 8) | buffer[24]))) * 1e-7;
//     data->longitude = ((double)((int32_t)((buffer[31] << 24) | (buffer[30] << 16) | 
//                      (buffer[29] << 8) | buffer[28]))) * 1e-7;
//     data->altitude = ((double)((int32_t)((buffer[35] << 24) | (buffer[34] << 16) | 
//                     (buffer[33] << 8) | buffer[32]))) * 1e-3;
//     data->satellites = buffer[23];
//     data->fix_type = buffer[20];
    
//     return true;
// }

// int main() {
//     stdio_init_all();
    
//     // Initialize I2C
//     i2c_init(i2c_default, 100 * 1000);
//     gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
//     gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
//     // Initialize LED
//     pico_led_init();

//     sleep_ms(10*1000);
    
//     gnss_data_t gnss_data;
    
//     while(true) {
//         if (read_gnss_data(i2c_default, &gnss_data)) {
            
//             printf("\nGNSS Data:\n");
//             printf("Latitude: %.6f\n", gnss_data.latitude);
//             printf("Longitude: %.6f\n", gnss_data.longitude);
//             printf("Altitude: %.2f m\n", gnss_data.altitude);
//             printf("Satellites: %d\n", gnss_data.satellites);
//             printf("Fix Type: %d\n", gnss_data.fix_type);
            
//             // Add fix type interpretation
//             printf("Fix Status: ");
//             switch(gnss_data.fix_type) {
//                 case 0: printf("No fix\n"); break;
//                 case 1: printf("Dead reckoning only\n"); break;
//                 case 2: printf("2D fix\n"); break;
//                 case 3: printf("3D fix\n"); break;
//                 case 4: printf("GNSS + dead reckoning\n"); break;
//                 default: printf("Unknown\n");
//             }
//         }
//         pico_set_led(true);
//         sleep_ms(500);
//         pico_set_led(false);
//     }
    
//     return 0;
// }

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define MAX_NMEA_LENGTH 82  // Maximum length of an NMEA sentence

typedef struct {
    double latitude;
    double longitude;
    double altitude;
    uint8_t satellites;
    uint8_t fix_type;
} gnss_data_t;

void parse_nmea_sentence(const char *sentence, gnss_data_t *data) {
    // Check if the sentence starts with "$" (NMEA sentences)
    if (sentence[0] != '$') {
        printf("Invalid NMEA sentence: %s\n", sentence);
        return;
    }
    
    // Identify sentence type and extract relevant information
    if (strncmp(sentence, "$GNGGA", 6) == 0) {
        // $GNGGA - Global Positioning System Fix Data
        char *token = strtok((char *)sentence, ",");
        int field = 0;
        
        while (token != NULL) {
            field++;
            if (field == 2) {
                // Latitude
                data->latitude = atof(token);  // Simplified parsing for now
            } else if (field == 4) {
                // Longitude
                data->longitude = atof(token);
            } else if (field == 9) {
                // Satellites
                data->satellites = atoi(token);
            } else if (field == 11) {
                // Altitude
                data->altitude = atof(token);
            }
            token = strtok(NULL, ",");
        }
    } else if (strncmp(sentence, "$GNGSA", 6) == 0) {
        // $GNGSA - GNSS DOP and active satellites
        printf("Received $GNGSA sentence: %s\n", sentence);
        // Parse additional fields here if needed
    } else if (strncmp(sentence, "$GPGSV", 6) == 0 ||
               strncmp(sentence, "$GLGSV", 6) == 0 ||
               strncmp(sentence, "$GAGSV", 6) == 0 ||
               strncmp(sentence, "$GBGSV", 6) == 0) {
        // $GPGSV, $GLGSV, $GAGSV, $GBGSV - Satellite in view information
        printf("Received Satellite in view information: %s\n", sentence);
        // Parse satellite info here if needed
    } else if (strncmp(sentence, "$GNGLL", 6) == 0) {
        // $GNGLL - Geographic position, latitude and longitude
        printf("Received $GNGLL sentence: %s\n", sentence);
        // Parse latitude and longitude from this sentence
    } else if (strncmp(sentence, "$GNRMC", 6) == 0) {
        // $GNRMC - Recommended Minimum Navigation Information
        printf("Received $GNRMC sentence: %s\n", sentence);
        // Parse relevant fields from this sentence
    } else {
        // Unsupported sentence
        printf("Unsupported NMEA sentence: %s\n", sentence);
    }
}

bool read_nmea_sentence(i2c_inst_t *i2c, char *buffer, size_t buffer_size) {
    size_t index = 0;
    uint8_t byte;
    
    // Read characters from the GPS until we get a complete sentence (ending with \r\n)
    while (index < buffer_size - 1) {
        if (i2c_read_blocking(i2c, 0x42, &byte, 1, false) != 1) {
            return false;
        }
        
        if (byte == '$' && index == 0) {
            buffer[index++] = byte;
        } else if (index > 0) {
            buffer[index++] = byte;
            if (byte == '\n' && buffer[index - 2] == '\r') {
                buffer[index] = '\0';
                return true;
            }
        }
    }
    
    return false;
}

int main() {
    stdio_init_all();
    
    // Initialize I2C (adjust the I2C instance, pins, and baud rate as needed)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    gnss_data_t gnss_data;
    char nmea_buffer[MAX_NMEA_LENGTH];

    while (true) {
        if (read_nmea_sentence(i2c_default, nmea_buffer, MAX_NMEA_LENGTH)) {
            printf("Received NMEA sentence: %s\n", nmea_buffer);
            parse_nmea_sentence(nmea_buffer, &gnss_data);

            // Print extracted data if applicable
            if (gnss_data.latitude != 0.0 && gnss_data.longitude != 0.0) {
                printf("GNSS Data:\n");
                printf("Latitude: %f\n", gnss_data.latitude);
                printf("Longitude: %f\n", gnss_data.longitude);
                printf("Altitude: %f m\n", gnss_data.altitude);
                printf("Satellites: %d\n", gnss_data.satellites);
            }
        } else {
            printf("Failed to read NMEA sentence\n");
        }

        sleep_ms(1000);
    }

    return 0;
}
