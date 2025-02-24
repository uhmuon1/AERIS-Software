#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define BUF_LEN 0x100

#ifndef SPARKFUN_THING_PLUS_SPI_SCK_PIN
#define SPARKFUN_THING_PLUS_SPI_SCK_PIN 18
#endif

#ifndef SPARKFUN_THING_PLUS_SPI_MOSI_PIN
#define SPARKFUN_THING_PLUS_SPI_MOSI_PIN 19
#endif

#ifndef SPARKFUN_THING_PLUS_SPI_MISO_PIN
#define SPARKFUN_THING_PLUS_SPI_MISO_PIN 16
#endif

#ifndef SPARKFUN_THING_PLUS_SPI_CS_PIN
#define SPARKFUN_THING_PLUS_SPI_CS_PIN 17
#endif

void printbuf(uint8_t buf[], size_t len) {
    size_t i;
    for (i = 0; i < len; ++i) {
        if (i % 16 == 15)
            printf("%02x\n", buf[i]);
        else
            printf("%02x ", buf[i]);
    }

    // append trailing newline if there isn't one
    if (i % 16) {
        putchar('\n');
    }
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

int main() {
    // Enable UART so we can print
    stdio_init_all();
    pico_led_init();
    //TODO Need to change to the sparkfun thing plus board things
#if !defined(spi_default) || !defined(SPARKFUN_THING_PLUS_SPI_SCK_PIN) || !defined(SPARKFUN_THING_PLUS_SPI_MOSI_PIN) || !defined(SPARKFUN_THING_PLUS_SPI_MISO_PIN) || !defined(SPARKFUN_THING_PLUS_SPI_CS_PIN)
#warning spi/spi_master example requires a board with SPI pins
    puts("Default SPI pins were not defined");
#else
    pico_set_led(true);
    printf("SPI master example\n");
    // Enable SPI 0 at 1 MHz and connect to GPIOs
    spi_init(spi_default, 1000 * 1000);
    gpio_set_function(SPARKFUN_THING_PLUS_SPI_MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPARKFUN_THING_PLUS_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPARKFUN_THING_PLUS_SPI_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPARKFUN_THING_PLUS_SPI_CS_PIN, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_4pins_with_func(SPARKFUN_THING_PLUS_SPI_MISO_PIN, SPARKFUN_THING_PLUS_SPI_MOSI_PIN, SPARKFUN_THING_PLUS_SPI_SCK_PIN, SPARKFUN_THING_PLUS_SPI_CS_PIN, GPIO_FUNC_SPI));

    uint8_t out_buf[BUF_LEN], in_buf[BUF_LEN];

    // Initialize output buffer
    for (size_t i = 0; i < BUF_LEN; ++i) {
        out_buf[i] = i;
    }
    while(true){
        printf("SPI master says: The following buffer will be written to MOSI endlessly:\n");
        printbuf(out_buf, BUF_LEN);
    
        for (size_t i = 0; ; ++i) {
            // Write the output buffer to MOSI, and at the same time read from MISO.
            spi_write_read_blocking(spi_default, out_buf, in_buf, BUF_LEN);
    
            // Write to stdio whatever came in on the MISO line.
            printf("SPI master says: read page %d from the MISO line:\n", i);
            printbuf(in_buf, BUF_LEN);
    
            // Sleep for ten seconds so you get a chance to read the output.
            sleep_ms(10 * 1000);
        }
        sleep_ms(10*1000);
    }
#endif
}
