#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

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

// SPI Pins for Thing Plus RP2040
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_RST  20  // Reset pin
#define PIN_TX   21  // TXEN pin
#define PIN_RX   22  // RXEN pin
#define SPI_PORT spi0

// Configuration
#define FREQUENCY 433000000

// Function declarations
void lora_reset();
void lora_write_reg(uint8_t reg, uint8_t data);
uint8_t lora_read_reg(uint8_t reg);
void lora_init();
void lora_send_packet(const uint8_t *data, uint8_t len);

int pico_led_init(void) {
    printf("Initializing LED...\n");
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    printf("LED initialized using GPIO pin %d\n", PICO_DEFAULT_LED_PIN);
    return PICO_OK;
#endif
    printf("No LED initialization method found\n");
    return -1;
}

void pico_set_led(bool led_on) {
    printf("Setting LED %s\n", led_on ? "ON" : "OFF");
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#endif
}

bool check_lora_connection() {
    uint8_t version = lora_read_reg(0x42); // Version register
    printf("LoRa chip version: 0x%02X\n", version);
    
    // Most SX127x chips return 0x12
    if (version == 0x12) {
        printf("SX127x chip detected\n");
        return true;
    } else {
        printf("WARNING: Unknown or disconnected LoRa chip!\n");
        return false;
    }
}

void lora_init() {
    printf("Starting LoRa Initialization...\n");
    
    // Initialize SPI
    printf("Initializing SPI at 1MHz\n");
    spi_init(SPI_PORT, 1e6);  // 1MHz clock rate
    // Set SPI format (CPOL=0, CPHA=0)
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    
    // Initialize SPI pins (without hardware CS)
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select as output
    printf("Configuring Chip Select pin\n");
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    
    // Reset pin
    printf("Configuring Reset pin\n");
    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    // TXEN Pin set
    printf("Configuring TXEN pin\n");
    gpio_init(PIN_TX);
    gpio_set_dir(PIN_TX, GPIO_OUT);
    
    // RXEN Pin set
    printf("Configuring RXEN pin\n");
    gpio_init(PIN_RX);
    gpio_set_dir(PIN_RX, GPIO_OUT);
    gpio_put(PIN_RX, 0); // RXEN to low

    // Reset the LoRa module
    // printf("Resetting LoRa module\n");
    // lora_reset();
    
    check_lora_connection();

    // Set sleep mode
    printf("Setting sleep mode\n");
    lora_write_reg(REG_OP_MODE, SLEEP_MODE);  // Sleep mode, LoRa mode
    sleep_ms(10);
    
    // Set frequency to 433 MHz
    printf("Setting frequency to 433 MHz\n");
    uint32_t frf = ((uint32_t)FREQUENCY) / 61.035;
    lora_write_reg(REG_FR_MSB, (frf >> 16) & 0xFF);
    lora_write_reg(REG_FR_MID, (frf >> 8) & 0xFF);
    lora_write_reg(REG_FR_LSB, frf & 0xFF);
    
    // PA BOOST
    printf("Configuring PA BOOST\n");
    lora_write_reg(REG_PA_CONFIG, 0b11110011);  // 0-011-1111 PA BOOST enabled, output power = 15dBm
    lora_write_reg(REG_PA_DAC, 0x84);     // PA_HF/LF or +17dBm
    // Setting over current protection
    lora_write_reg(REG_OCP, 0b00111111); // 00-1-11111

    // Setting low noise amplifier
    lora_write_reg(REG_LNA, 0x20); // Max gain

    lora_write_reg(REG_FIFO_ADDR_PTR,0x00);
    lora_write_reg(REG_FIFO_TX_BASE_ADDR,0x00);
    lora_write_reg(REG_FIFO_RX_BASE_ADDR,0x00);
    
    // Set modem config
    printf("Configuring Modem Settings\n");
    lora_write_reg(REG_MODEM_CONFIG_1, 0b01100011);  // 0110-001-1 BW=62.5kHz, CR=4/5, implicit header
    lora_write_reg(REG_MODEM_CONFIG_2, 0b01111100);  // 0111-1-1-00 SF=7, TX CONT, crc on
    
    // Setting preamble to 8
    lora_write_reg(REG_PREAMBLE_MSB, 0x00);
    lora_write_reg(REG_PREAMBLE_LSB, 0x08);

    printf("Setting Sync Word\n");
    lora_write_reg(REG_SYNC_WORD, 0x12);
    
    // Set to standby
    printf("Setting to Standby mode\n");
    lora_write_reg(REG_OP_MODE, STDBY_MODE);  // Standby mode
    
    printf("LoRa Initialization Complete\n");

    debug_tx_parameters();
}

void debug_tx_parameters() {
    uint8_t sf_cr = lora_read_reg(REG_MODEM_CONFIG_2);
    uint8_t spreading = (sf_cr >> 4) & 0x0F;
    uint8_t bw_cr = lora_read_reg(REG_MODEM_CONFIG_1);
    uint8_t bandwidth = (bw_cr >> 4) & 0x0F;
    uint8_t coding_rate = (bw_cr >> 1) & 0x07;
    uint8_t power = lora_read_reg(REG_PA_CONFIG) & 0x0F;
    
    printf("TX Parameters:\n");
    printf("  Spreading Factor: SF%d\n", spreading);
    printf("  Bandwidth: 0x%02x\n", bandwidth);
    printf("  Coding Rate: 4/%d\n", coding_rate + 4);
    printf("  Output Power: %d\n", power);
}

void lora_reset() {
    printf("Performing LoRa module hardware reset\n");
    gpio_put(PIN_RST, 0);
    sleep_ms(10);
    gpio_put(PIN_RST, 1);
    sleep_ms(10);
    printf("LoRa module reset complete\n");
}

void lora_write_reg(uint8_t reg, uint8_t data) {
    printf("Writing to register 0x%02X: value 0x%02X\n", reg, data);
    uint8_t buf[2] = {reg | 0x80, data};  // Set MSB for write

    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, buf, 2);
    gpio_put(PIN_CS, 1);
}

uint8_t lora_read_reg(uint8_t reg) {

    uint8_t TX_buf[2] = {reg & 0x7F, 0x00};  // Clear MSB for read
    uint8_t RX_buf[2];

    gpio_put(PIN_CS, 0);
    spi_write_read_blocking(SPI_PORT, TX_buf, RX_buf, 2);
    gpio_put(PIN_CS, 1);
    
    // printf("Reading from register 0x%02X: value 0x%02X\n", reg, buf[1]);
    return RX_buf[1];
}

void lora_send_packet(const uint8_t *data, uint8_t len) {
    printf("Preparing to send packet of length %d\n", len);

    // Set to standby
    printf("Setting to Standby mode before transmission\n");
    lora_write_reg(REG_OP_MODE, STDBY_MODE);
    
    // Reset FIFO pointer
    printf("Resetting FIFO pointer\n");
    lora_write_reg(REG_FIFO_ADDR_PTR, 0x00);
    
    // Write data to FIFO
    printf("Writing data to FIFO\n");

    uint8_t msg[] = {REG_FIFO | 0x80, data};
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, msg, 1);
    gpio_put(PIN_CS, 1);
    
    // Set payload length
    printf("Setting payload length to %d\n", len);
    lora_write_reg(REG_PAYLOAD_LENGTH, len);
    
    // Start transmission
    printf("Entering TX mode\n");
    gpio_put(PIN_TX, 1); // Set TXEN Pin to high
    // TODO set TXEN to logic high
    lora_write_reg(REG_OP_MODE, TX_MODE);  // TX mode
    
    // Wait for TX done
    printf("Waiting for transmission to complete\n");
    while((lora_read_reg(REG_IRQ_FLAGS) & 0x08) == 0) {
        sleep_ms(1);
    }

    gpio_put(PIN_TX, 0); // Set TXEN Pin to low
    
    // Clear IRQ flags
    printf("Clearing IRQ flags\n");
    lora_write_reg(REG_IRQ_FLAGS, 0xFF);
    sleep_ms(10);
    printf("Packet transmission complete\n");
}

int main() {
    stdio_init_all();

    sleep_ms(5000); // Give time to pull up serial bus
    printf("Starting LoRa TX Test\n");
    
    // Initialize LED
    int led_init_result = pico_led_init();
    printf("LED Initialization Result: %d\n", led_init_result);
    
    // Initialize LoRa
    printf("Initializing LoRa Module\n");
    lora_init();
    
    // Test message
    //uint8_t message[] = "Hello from RP2040 LoRa!";
    uint8_t message[] = {0x41,0x42,0x43,0x44};
    
    printf("Starting TX Loop\n");

    while (1) {
        pico_set_led(true);
        printf("Sending packet...\n");
        
        // Print out the message contents
        printf("Message contents: ");
        for (int i = 0; i < sizeof(message) - 1; i++) {
            printf("%c", message[i]);
        }
        printf("\n");
        
        lora_send_packet(message, sizeof(message) - 1);
        printf("Packet sent successfully!\n");
        
        sleep_ms(1000);
        // Uncomment the sleep if you want to control transmission rate
        // sleep_ms(5000);  // Wait 5 seconds between transmissions
    }
    
    return 0;
}