/**
 * config.h - Global configuration settings
 */
#ifndef CONFIG_H
#define CONFIG_H

// GNSS configuration
#define CONFIG_GNSS_SAMPLE_INTERVAL_MS  40    // 25Hz sampling rate
#define CONFIG_STATUS_INTERVAL_MS       1000  // Status update interval

// LoRa configuration
#define CONFIG_LORA_FREQUENCY           433000000  // 433 MHz
#define CONFIG_LORA_TX_INTERVAL_MS      5000       // 5 seconds between transmissions
#define CONFIG_LORA_POWER               0xff       // Maximum power

// Hardware pins - SPI for LoRa
#define CONFIG_PIN_MISO                 16
#define CONFIG_PIN_CS                   17
#define CONFIG_PIN_SCK                  18
#define CONFIG_PIN_MOSI                 19
#define CONFIG_PIN_RST                  20
#define CONFIG_PIN_TX                   21
#define CONFIG_PIN_RX                   22
#define CONFIG_SPI_PORT                 spi0

// Misc settings
#define CONFIG_MAX_RUNTIME_MS           (10 * 60 * 1000)  // 10 minutes max runtime

#endif // CONFIG_H