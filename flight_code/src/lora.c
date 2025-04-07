/**
 * lora.c - Implementation of LoRa module functionality
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "config.h"
#include "lora.h"

// LoRa module registers
#define REG_FIFO               0x00
#define REG_OP_MODE            0x01
#define REG_FR_MSB             0x06
#define REG_FR_MID             0x07
#define REG_FR_LSB             0x08
#define REG_PA_CONFIG          0x09
#define REG_PA_DAC             0x4D
#define REG_OCP                0x0B
#define REG_LNA                0x0C
#define REG_FIFO_ADDR_PTR      0x0D
#define REG_FIFO_TX_BASE_ADDR  0x0E
#define REG_FIFO_RX_BASE_ADDR  0x0F
#define REG_IRQ_FLAGS_MASK     0x11
#define REG_IRQ_FLAGS          0x12
#define REG_RSSI_VALUE         0x1A
#define REG_MODEM_CONFIG_1     0x1D
#define REG_MODEM_CONFIG_2     0x1E
#define REG_TIMEOUT_LSB        0x1F
#define REG_PREAMBLE_MSB       0x20
#define REG_PREAMBLE_LSB       0x21
#define REG_PAYLOAD_LENGTH     0x22
#define REG_MODEM_CONFIG_3     0x26
#define REG_SYNC_WORD          0x39
#define REG_PKT_SNR_VALUE      0x19
#define REG_PKT_RSSI_VALUE     0x1B
#define REG_DETECTION_THRESHOLD 0x37

// Modes
#define SLEEP_MODE             0x80
#define STDBY_MODE             0x81
#define TX_MODE                0x83 // b1000 0011

bool lora_init() {
    printf("Starting LoRa Initialization...\n");
    
    // Initialize SPI
    printf("Initializing SPI at 1MHz\n");
    spi_init(CONFIG_SPI_PORT, 1e6);  // 1MHz clock rate
    // Set SPI format (CPOL=0, CPHA=0)
    spi_set_format(CONFIG_SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    
    // Initialize SPI pins (without hardware CS)
    gpio_set_function(CONFIG_PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(CONFIG_PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(CONFIG_PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select as output
    printf("Configuring Chip Select pin\n");
    gpio_init(CONFIG_PIN_CS);
    gpio_set_dir(CONFIG_PIN_CS, GPIO_OUT);
    gpio_put(CONFIG_PIN_CS, 1);
    
    // Reset pin
    printf("Configuring Reset pin\n");
    gpio_init(CONFIG_PIN_RST);
    gpio_set_dir(CONFIG_PIN_RST, GPIO_OUT);

    // TXEN Pin set
    printf("Configuring TXEN pin\n");
    gpio_init(CONFIG_PIN_TX);
    gpio_set_dir(CONFIG_PIN_TX, GPIO_OUT);
    
    // RXEN Pin set
    printf("Configuring RXEN pin\n");
    gpio_init(CONFIG_PIN_RX);
    gpio_set_dir(CONFIG_PIN_RX, GPIO_OUT);
    gpio_put(CONFIG_PIN_RX, 0); // RXEN to low

    // Reset the LoRa module
    lora_reset();
    
    // Verify chip is connected
    uint8_t version = lora_read_reg(0x42); // Version register
    printf("LoRa chip version: 0x%02X\n", version);
    
    if (version != 0x12) {
        printf("WARNING: Unknown or disconnected LoRa chip!\n");
        return false;
    }
    
    // Set sleep mode
    printf("Setting sleep mode\n");
    lora_write_reg(REG_OP_MODE, SLEEP_MODE);  // Sleep mode, LoRa mode
    sleep_ms(10);
    
    // Set frequency
    printf("Setting frequency\n");
    uint32_t frf = ((uint32_t)CONFIG_LORA_FREQUENCY) / 61.035;
    lora_write_reg(REG_FR_MSB, (frf >> 16) & 0xFF);
    lora_write_reg(REG_FR_MID, (frf >> 8) & 0xFF);
    lora_write_reg(REG_FR_LSB, frf & 0xFF);
    
    // PA BOOST
    printf("Configuring PA BOOST\n");
    lora_write_reg(REG_PA_CONFIG, CONFIG_LORA_POWER);  // PA BOOST enabled
    lora_write_reg(REG_PA_DAC, 0x84);     // PA_HF/LF or +17dBm
    // Setting over current protection
    lora_write_reg(REG_OCP, 0b00111111); // 00-1-11111

    // Setting low noise amplifier
    lora_write_reg(REG_LNA, 0x20); // Max gain
    
    // Set modem config
    printf("Configuring Modem Settings\n");
    lora_write_reg(REG_MODEM_CONFIG_1, 0b10000011);  // BW=250 kHz, CR=4/5, implicit header
    lora_write_reg(REG_MODEM_CONFIG_2, 0b01110100);  // SF=7, TX single, crc on
    
    // Setting preamble to 8
    lora_write_reg(REG_PREAMBLE_MSB, 0x00);
    lora_write_reg(REG_PREAMBLE_LSB, 0x08);

    printf("Setting Sync Word\n");
    lora_write_reg(REG_SYNC_WORD, 0x12);
    
    // Set to standby
    printf("Setting to Standby mode\n");
    lora_write_reg(REG_OP_MODE, STDBY_MODE);  // Standby mode
    
    printf("LoRa Initialization Complete\n");
    lora_debug_parameters();
    
    return true;
}

void lora_reset() {
    printf("Performing LoRa module hardware reset\n");
    gpio_put(CONFIG_PIN_RST, 0);
    sleep_ms(10);
    gpio_put(CONFIG_PIN_RST, 1);
    sleep_ms(10);
    printf("LoRa module reset complete\n");
}

void lora_write_reg(uint8_t reg, uint8_t data) {
    uint8_t buf[2] = {reg | 0x80, data};  // Set MSB for write

    gpio_put(CONFIG_PIN_CS, 0);
    spi_write_blocking(CONFIG_SPI_PORT, buf, 2);
    gpio_put(CONFIG_PIN_CS, 1);
}

uint8_t lora_read_reg(uint8_t reg) {
    uint8_t TX_buf[2] = {reg & 0x7F, 0x00};  // Clear MSB for read
    uint8_t RX_buf[2];

    gpio_put(CONFIG_PIN_CS, 0);
    spi_write_read_blocking(CONFIG_SPI_PORT, TX_buf, RX_buf, 2);
    gpio_put(CONFIG_PIN_CS, 1);
    
    return RX_buf[1];
}

bool lora_send_packet(const uint8_t *data, uint8_t len) {
    if (len > LORA_MAX_PACKET_SIZE) {
        printf("Packet too large (max %d bytes)\n", LORA_MAX_PACKET_SIZE);
        return false;
    }
    
    printf("Preparing to send LoRa packet of length %d\n", len);

    // Set to standby
    lora_write_reg(REG_OP_MODE, STDBY_MODE);
    
    // Reset FIFO pointer
    uint8_t tx_base_add = lora_read_reg(REG_FIFO_TX_BASE_ADDR);
    lora_write_reg(REG_FIFO_ADDR_PTR, tx_base_add);
    
    // Write data to FIFO
    printf("Writing data to FIFO\n");

    uint8_t header = REG_FIFO | 0x80;
    gpio_put(CONFIG_PIN_CS, 0);
    spi_write_blocking(CONFIG_SPI_PORT, &header, 1);
    spi_write_blocking(CONFIG_SPI_PORT, data, len);
    gpio_put(CONFIG_PIN_CS, 1);
    
    // Set payload length
    lora_write_reg(REG_PAYLOAD_LENGTH, len);
    
    // Start transmission
    printf("Starting LoRa transmission\n");
    gpio_put(CONFIG_PIN_TX, 1); // Set TXEN Pin to high
    lora_write_reg(REG_OP_MODE, TX_MODE);
    
    // Wait for TX done with timeout
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while ((lora_read_reg(REG_IRQ_FLAGS) & 0x08) == 0) {
        sleep_ms(10);
        
        // Timeout after 2 seconds
        if (to_ms_since_boot(get_absolute_time()) - start_time > 2000) {
            printf("LoRa transmission timeout\n");
            gpio_put(CONFIG_PIN_TX, 0);
            return false;
        }
    }

    gpio_put(CONFIG_PIN_TX, 0); // Set TXEN Pin to low
    
    // Clear IRQ flags
    lora_write_reg(REG_IRQ_FLAGS, 0xFF);
    lora_write_reg(REG_OP_MODE, STDBY_MODE);

    printf("LoRa packet transmission complete\n");
    return true;
}

void lora_debug_parameters() {
    uint8_t sf_cr = lora_read_reg(REG_MODEM_CONFIG_2);
    uint8_t spreading = (sf_cr >> 4) & 0x0F;
    uint8_t bw_cr = lora_read_reg(REG_MODEM_CONFIG_1);
    uint8_t bandwidth = (bw_cr >> 4) & 0x0F;
    uint8_t coding_rate = (bw_cr >> 1) & 0x07;
    uint8_t power = lora_read_reg(REG_PA_CONFIG) & 0x0F;
    
    printf("LoRa TX Parameters:\n");
    printf("  Spreading Factor: SF%d\n", spreading);
    printf("  Bandwidth: 0x%02x\n", bandwidth);
    printf("  Coding Rate: 4/%d\n", coding_rate + 4);
    printf("  Output Power: %d\n", power);
}
