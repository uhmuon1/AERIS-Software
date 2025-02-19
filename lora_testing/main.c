#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

// LoRa module registers
#define REG_FIFO                 0x00
#define REG_OP_MODE             0x01
#define REG_FR_MSB              0x06
#define REG_FR_MID              0x07
#define REG_FR_LSB              0x08
#define REG_PA_CONFIG           0x09
#define REG_PA_DAC              0x4D
#define REG_LNA                 0x0C
#define REG_FIFO_ADDR_PTR      0x0D
#define REG_FIFO_TX_BASE_ADDR  0x0E
#define REG_IRQ_FLAGS          0x12
#define REG_TX_POWER           0x09
#define REG_MODEM_CONFIG_1     0x1D
#define REG_MODEM_CONFIG_2     0x1E
#define REG_PAYLOAD_LENGTH     0x22
#define REG_IRQ_FLAGS_MASK     0x11

// SPI Pins for Thing Plus RP2040
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_RST  20  // Reset pin
#define SPI_PORT spi0

// Function declarations
void lora_reset();
void lora_write_reg(uint8_t reg, uint8_t data);
uint8_t lora_read_reg(uint8_t reg);
void lora_init();
void lora_send_packet(const uint8_t *data, uint8_t len);

// Initialize LoRa module
void lora_init() {
    // Initialize SPI
    spi_init(SPI_PORT, 1000000);  // 1MHz clock rate
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select as output
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    
    // Reset pin
    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);
    
    // Reset the LoRa module
    lora_reset();
    
    // Set sleep mode
    lora_write_reg(REG_OP_MODE, 0x80);  // Sleep mode, LoRa mode
    sleep_ms(10);
    
    // Set frequency to 915MHz
    uint32_t frf = ((uint32_t)915000000) / 61.035;
    lora_write_reg(REG_FR_MSB, (frf >> 16) & 0xFF);
    lora_write_reg(REG_FR_MID, (frf >> 8) & 0xFF);
    lora_write_reg(REG_FR_LSB, frf & 0xFF);
    
    // PA BOOST
    lora_write_reg(REG_PA_CONFIG, 0x8F);  // PA BOOST enabled, output power = 15dBm
    lora_write_reg(REG_PA_DAC, 0x87);     // PA DAC enabled
    
    // Set modem config
    lora_write_reg(REG_MODEM_CONFIG_1, 0x72);  // BW=125kHz, CR=4/5, explicit header
    lora_write_reg(REG_MODEM_CONFIG_2, 0x70);  // SF=7, normal mode
    
    // Set to standby
    lora_write_reg(REG_OP_MODE, 0x81);  // Standby mode
}

void lora_reset() {
    gpio_put(PIN_RST, 0);
    sleep_ms(10);
    gpio_put(PIN_RST, 1);
    sleep_ms(10);
}

void lora_write_reg(uint8_t reg, uint8_t data) {
    gpio_put(PIN_CS, 0);
    uint8_t buf[2] = {reg | 0x80, data};  // Set MSB for write
    spi_write_blocking(SPI_PORT, buf, 2);
    gpio_put(PIN_CS, 1);
}

uint8_t lora_read_reg(uint8_t reg) {
    gpio_put(PIN_CS, 0);
    uint8_t buf[2] = {reg & 0x7F, 0x00};  // Clear MSB for read
    spi_write_blocking(SPI_PORT, &buf[0], 1);
    spi_read_blocking(SPI_PORT, 0, &buf[1], 1);
    gpio_put(PIN_CS, 1);
    return buf[1];
}

void lora_send_packet(const uint8_t *data, uint8_t len) {
    // Set to standby
    lora_write_reg(REG_OP_MODE, 0x81);
    
    // Reset FIFO pointer
    lora_write_reg(REG_FIFO_ADDR_PTR, 0x00);
    
    // Write data to FIFO
    gpio_put(PIN_CS, 0);
    uint8_t reg = REG_FIFO | 0x80;
    spi_write_blocking(SPI_PORT, &reg, 1);
    spi_write_blocking(SPI_PORT, data, len);
    gpio_put(PIN_CS, 1);
    
    // Set payload length
    lora_write_reg(REG_PAYLOAD_LENGTH, len);
    
    // Start transmission
    lora_write_reg(REG_OP_MODE, 0x83);  // TX mode
    
    // Wait for TX done
    while((lora_read_reg(REG_IRQ_FLAGS) & 0x08) == 0) {
        sleep_ms(1);
    }
    
    // Clear IRQ flags
    lora_write_reg(REG_IRQ_FLAGS, 0xFF);
}

int main() {
    stdio_init_all();
    
    // Initialize LoRa
    lora_init();
    
    // Test message
    uint8_t message[] = "Hello from RP2040 LoRa!";
    
    while (1) {
        printf("Sending packet...\n");
        lora_send_packet(message, sizeof(message) - 1);
        printf("Packet sent!\n");
        sleep_ms(5000);  // Wait 5 seconds between transmissions
    }
    
    return 0;
}