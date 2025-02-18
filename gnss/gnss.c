#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

// NEO-M9N I2C address (default)
#define GNSS_ADDR 0x42

// UBX protocol sync chars
#define UBX_SYNC_1 0xB5
#define UBX_SYNC_2 0x62

// UBX message classes
#define UBX_CLASS_NAV 0x01
#define UBX_CLASS_CFG 0x06

// UBX message IDs
#define UBX_NAV_PVT 0x07    // Navigation Position Velocity Time Solution

// Buffer sizes
#define MAX_PAYLOAD_SIZE 92

typedef struct {
    double latitude;
    double longitude;
    double altitude;
    uint8_t satellites;
    uint8_t fix_type;
} gnss_data_t;

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

// Calculate Fletcher checksum
void calculate_checksum(uint8_t *buffer, size_t length, uint8_t *ck_a, uint8_t *ck_b) {
    *ck_a = 0;
    *ck_b = 0;
    
    for(size_t i = 0; i < length; i++) {
        *ck_a += buffer[i];
        *ck_b += *ck_a;
    }
}

// Send UBX message
bool send_ubx_message(i2c_inst_t *i2c, uint8_t msg_class, uint8_t msg_id, 
                     uint8_t *payload, uint16_t length) {
    uint8_t header[6];
    uint8_t checksum[2];
    
    // Header
    header[0] = UBX_SYNC_1;
    header[1] = UBX_SYNC_2;
    header[2] = msg_class;
    header[3] = msg_id;
    header[4] = length & 0xFF;
    header[5] = (length >> 8) & 0xFF;
    
    // Calculate checksum
    calculate_checksum(&header[2], 4 + length, &checksum[0], &checksum[1]);
    
    // Send header
    if (i2c_write_blocking(i2c, GNSS_ADDR, header, 6, false) != 6) {
        return false;
    }
    
    // Send payload if any
    if (length > 0 && payload != NULL) {
        if (i2c_write_blocking(i2c, GNSS_ADDR, payload, length, false) != length) {
            return false;
        }
    }
    
    // Send checksum
    if (i2c_write_blocking(i2c, GNSS_ADDR, checksum, 2, false) != 2) {
        return false;
    }
    
    return true;
}

// Read GNSS data
bool read_gnss_data(i2c_inst_t *i2c, gnss_data_t *data) {
    uint8_t buffer[MAX_PAYLOAD_SIZE];
    uint8_t length = 0;
    
    // Request PVT data
    if (!send_ubx_message(i2c, UBX_CLASS_NAV, UBX_NAV_PVT, NULL, 0)) {
        return false;
    }
    
    // Wait for response
    sleep_ms(100);
    
    // Read response
    if (i2c_read_blocking(i2c, GNSS_ADDR, buffer, 1, false) != 1) {
        return false;
    }
    
    length = buffer[0];
    if (length > MAX_PAYLOAD_SIZE) {
        return false;
    }
    
    if (i2c_read_blocking(i2c, GNSS_ADDR, buffer, length, false) != length) {
        return false;
    }
    
    // Parse PVT data (simplified example)
    // Note: In real implementation, you'd need to properly parse the UBX-NAV-PVT message
    data->latitude = ((double)((int32_t)((buffer[9] << 24) | (buffer[8] << 16) | 
                    (buffer[7] << 8) | buffer[6]))) * 1e-7;
    data->longitude = ((double)((int32_t)((buffer[13] << 24) | (buffer[12] << 16) | 
                     (buffer[11] << 8) | buffer[10]))) * 1e-7;
    data->altitude = ((double)((int32_t)((buffer[17] << 24) | (buffer[16] << 16) | 
                    (buffer[15] << 8) | buffer[14]))) * 1e-3;
    data->satellites = buffer[23];
    data->fix_type = buffer[20];
    
    return true;
}

int main() {
    stdio_init_all();
    
    // Initialize I2C
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    // Initialize LED
    pico_led_init();
    
    gnss_data_t gnss_data;
    
    while(true) {
        if (read_gnss_data(i2c_default, &gnss_data)) {
            printf("\nGNSS Data:\n");
            printf("Latitude: %f\n", gnss_data.latitude);
            printf("Longitude: %f\n", gnss_data.longitude);
            printf("Altitude: %f m\n", gnss_data.altitude);
            printf("Satellites: %d\n", gnss_data.satellites);
            printf("Fix Type: %d\n", gnss_data.fix_type);
            
            // Blink LED on successful read
            pico_set_led(true);
            sleep_ms(100);
            pico_set_led(false);
        } else {
            printf("Failed to read GNSS data\n");
        }
        
        sleep_ms(1000);
    }
    
    return 0;
}