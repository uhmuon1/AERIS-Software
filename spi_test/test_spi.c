#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// Define SPI pins (using SPI0 default pins on Thing Plus RP2040)
#define SPI_PORT        spi0
#define PIN_MISO        16  // GPIO16
#define PIN_CS          17  // GPIO17
#define PIN_SCK         18  // GPIO18
#define PIN_MOSI        19  // GPIO19

// Example data to transmit
uint8_t tx_data[4] = {0x01, 0x02, 0x03, 0x04};
uint8_t rx_data[4] = {0};

int main() {
    // Initialize stdio
    stdio_init_all();
    printf("SPI Example with Manual CS on SparkFun Thing Plus RP2040\n");
    
    // Initialize SPI port at 1 MHz
    spi_init(SPI_PORT, 1000 * 1000);
    
    // Set SPI format (CPOL=0, CPHA=0)
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    
    // Initialize SPI pins (without hardware CS)
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Initialize CS as a regular GPIO output pin (active low)
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);  // Initially high (inactive)
    
    // Main loop
    while (1) {
        // Pull CS low to begin transaction (active low)
        gpio_put(PIN_CS, 0);
        
        // Write/read data as a single transaction
        spi_write_read_blocking(SPI_PORT, tx_data, rx_data, 4);
        
        // Pull CS high to end transaction
        gpio_put(PIN_CS, 1);
        
        // Print received data
        printf("Received: ");
        for (int i = 0; i < 4; i++) {
            printf("0x%02X ", rx_data[i]);
        }
        printf("\n");
        
        // Wait a moment before next transfer
        sleep_ms(1000);
    }
    
    return 0;
}