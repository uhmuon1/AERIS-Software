#include <string.h>
#include "pico/stdlib.h"
#include "hw_config.h"

// SPI Configurations
static spi_t spis[] = {
    {
        .hw_inst = spi1,  // SPI component
        .miso_gpio = 12,  // GPIO number (not pin number)
        .mosi_gpio = 15,
        .sck_gpio = 14,
        .baud_rate = 12500 * 1000,  // 12.5 MHz
    }
};

// SD Cards
static sd_card_t sd_cards[] = {
    {
        .pcName = "0:",   // Name used to mount device
        .spi = &spis[0],  // Pointer to the SPI driving this card
        .ss_gpio = 13,    // The SPI slave select GPIO for this SD card
        .use_card_detect = false,
    }
};

/* ********************************************************************** */
size_t spi_get_num() {
    return count_of(spis);
}

/* ********************************************************************** */
spi_t *spi_get_by_num(size_t num) {
    if (num >= count_of(spis)) {
        return NULL;
    }
    return &spis[num];
}

/* ********************************************************************** */
size_t sd_get_num() {
    return count_of(sd_cards);
}

/* ********************************************************************** */
sd_card_t *sd_get_by_num(size_t num) {
    if (num >= count_of(sd_cards)) {
        return NULL;
    }
    return &sd_cards[num];
}