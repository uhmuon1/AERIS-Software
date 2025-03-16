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
#define REG_TX_POWER           0x09
#define REG_MODEM_CONFIG_1     0x1D
#define REG_MODEM_CONFIG_2     0x1E
#define REG_MODEM_CONFIG_3     0x26
#define REG_PREAMBLE_MSB       0x20
#define REG_PREAMBLE_LSB       0x21
#define REG_PAYLOAD_LENGTH     0x22
#define REG_SYNC_WORD          0x39
#define REG_RSSI_VALUE         0x1A
#define REG_PKT_SNR_VALUE      0x19
#define REG_PKT_RSSI_VALUE     0x1B
#define REG_DETECTION_THRESHOLD 0x37

#define REG_TIMEOUT_LSB        0x15

// Modes
#define SLEEP_MODE             0x80 // b1000 0000
#define STDBY_MODE             0x81 // b1000 0001
#define TX_MODE                0x83 // b1000 0011
#define RXCONT_MODE            0x85 // b1000 0101

// SPI Pins for Thing Plus RP2040
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
#define PIN_RST  20  // Reset pin
#define PIN_TX   21
#define PIN_RX   22
#define SPI_PORT spi0

// Configuration
#define FREQUENCY 433000000

// Function declarations
void lora_reset();
void lora_write_reg(uint8_t reg, uint8_t data);
uint8_t lora_read_reg(uint8_t reg);
void lora_init();
void lora_receive_packet(uint8_t *buffer, uint8_t *len);

int pico_led_init(void) {
    printf("Initializing LED...\n");
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    printf("LED initialized using GPIO pin %d\n", PICO_DEFAULT_LED_PIN);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    int result = cyw43_arch_init();
    printf("LED initialization result: %d\n", result);
    return result;
#endif
    printf("No LED initialization method found\n");
    return -1;
}

void pico_set_led(bool led_on) {
    printf("Setting LED %s\n", led_on ? "ON" : "OFF");
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

void lora_init() {
    printf("Starting LoRa Initialization...\n");

    // Initialize SPI
    printf("Initializing SPI at 1MHz\n");
    spi_init(SPI_PORT, 1e6);  // 1MHz clock rate
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
    gpio_put(PIN_TX, 0);
    
    // RXEN Pin set
    printf("Configuring RXEN pin\n");
    gpio_init(PIN_RX);
    gpio_set_dir(PIN_RX, GPIO_OUT);

    // Reset the LoRa module
    printf("Resetting LoRa module\n");
    lora_reset();

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
    // lora_write_reg(REG_PA_CONFIG, 0xFF);  // PA BOOST enabled, output power = 15dBm
    // lora_write_reg(REG_PA_DAC, 0x87);     // PA DAC enabled
    lora_write_reg(REG_OCP, 0x3F);

    // low noise amplifier
    lora_write_reg(REG_LNA, 0xB0);

    // Reset FIFO buffer pointer
    lora_write_reg(REG_FIFO_ADDR_PTR,0x00);
    lora_write_reg(REG_FIFO_TX_BASE_ADDR,0x00);
    lora_write_reg(REG_FIFO_RX_BASE_ADDR,0x00);

    // Set modem config
    printf("Configuring Modem Settings\n");
    lora_write_reg(REG_MODEM_CONFIG_1, 0x63);  // 0110-001-1 BW=62.5kHz, CR=4/5, explicit header
    lora_write_reg(REG_MODEM_CONFIG_2, 0x77);  // 0111-0-1-11 SF=7, normal mode
    lora_write_reg(REG_MODEM_CONFIG_3, 0x04);  // 0000-0-1-00 LNA gain set by the internal AGC loop
    lora_write_reg(REG_TIMEOUT_LSB, 0xF0);   // Set timeout to max

    lora_write_reg(REG_PREAMBLE_MSB,0x00);
    lora_write_reg(REG_PREAMBLE_LSB,0x08);

    // Set Sync Word (added for debugging)
    printf("Setting Sync Word\n");
    lora_write_reg(REG_SYNC_WORD, 0x00);  // Example sync word

    // Set to standby
    printf("Setting to RX Continuous mode\n");
    lora_write_reg(REG_OP_MODE, RXCONT_MODE);

    printf("LoRa Initialization Complete\n");
}

// Function to read and display signal quality
void read_signal_quality() {
    // Read RSSI (current)
    int8_t rssi_value = lora_read_reg(REG_RSSI_VALUE);
    float rssi = -157 + rssi_value;  // Adjust formula based on frequency band
    
    // Read SNR (from last packet)
    int8_t snr_raw = lora_read_reg(REG_PKT_SNR_VALUE);
    float snr = snr_raw * 0.25;
    
    // Read packet RSSI (from last packet)
    int8_t pkt_rssi_value = lora_read_reg(REG_PKT_RSSI_VALUE);
    float pkt_rssi = -157 + pkt_rssi_value;  // Adjust formula based on frequency band
    
    printf("Signal Quality:\n");
    printf("  Current RSSI: %.1f dBm\n", rssi);
    printf("  Last Packet RSSI: %.1f dBm\n", pkt_rssi);
    printf("  Last Packet SNR: %.1f dB\n", snr);
    
    // Signal strength assessment
    if (pkt_rssi > -80) {
        printf("  Signal strength: EXCELLENT\n");
    } else if (pkt_rssi > -100) {
        printf("  Signal strength: GOOD\n");
    } else if (pkt_rssi > -120) {
        printf("  Signal strength: FAIR\n");
    } else {
        printf("  Signal strength: POOR\n");
    }
    
    // SNR assessment
    if (snr > 5) {
        printf("  Signal quality: EXCELLENT\n");
    } else if (snr > 0) {
        printf("  Signal quality: GOOD\n");
    } else if (snr > -5) {
        printf("  Signal quality: FAIR\n");
    } else {
        printf("  Signal quality: POOR\n");
    }
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

    //printf("Reading from register 0x%02X: value 0x%02X\n", reg, buf[1]);
    return buf[1];
}

void lora_receive_packet(uint8_t *buffer, uint8_t *len) {
    
    
    lora_write_reg(REG_FIFO_ADDR_PTR,0x00);

    // Set to RX mode
    printf("Entering RX mode\n");
    gpio_put(PIN_RX,1);

    // Wait for RX done
    printf("Waiting for received packet\n");
    while ((lora_read_reg(REG_IRQ_FLAGS) & 0x40) == 0 || (lora_read_reg(REG_IRQ_FLAGS) & 0x80) == 0) {  // RX done flag or timeout
        read_signal_quality();
        sleep_ms(1);
    }

    if (lora_read_reg(REG_IRQ_FLAGS) & 0x20) {
        printf("CRC Error\n"); 
        sleep_ms(1);
    }

    gpio_put(PIN_RX,0);

    // Clear IRQ flags
    printf("Clearing IRQ flags\n");
    lora_write_reg(REG_IRQ_FLAGS, 0xFF);

    // Read payload length
    *len = lora_read_reg(REG_PAYLOAD_LENGTH);
    printf("Payload length: %d\n", len);

    // Read data from FIFO
    printf("Reading data from FIFO\n");
    gpio_put(PIN_CS, 0);
    uint8_t reg = REG_FIFO & 0x7F;  // Read mode
    spi_write_blocking(SPI_PORT, &reg, 1);
    spi_read_blocking(SPI_PORT, 0, buffer, *len);
    gpio_put(PIN_CS, 1);
}

int main() {
    stdio_init_all();

    // Initialize LED
    pico_led_init();

    sleep_ms(5000);
    printf("Starting LoRa RX Test\n");

    // Initialize LoRa
    printf("Initializing LoRa Module\n");
    lora_init();

    uint8_t received_data[256];
    uint8_t received_len = 0;

    printf("Starting RX Loop\n");

    while (1) {
        pico_set_led(true);
        printf("Waiting for a packet...\n");

        // Wait and receive a packet
        lora_receive_packet(received_data, &received_len);

        printf("Received packet of length %d\n", received_len);

        // Print out the received message contents
        printf("Received message: ");
        for (int i = 0; i < received_len; i++) {
            printf("%c", received_data[i]);
        }
        printf("\n");

        pico_set_led(false);
    }

    return 0;
}
