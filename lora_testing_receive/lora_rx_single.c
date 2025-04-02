#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "pico/time.h"  // Include for timing functions

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
#define SLEEP_MODE             0x80 // b1000 0000
#define STDBY_MODE             0x81 // b1000 0001
#define TX_MODE                0x83 // b1000 0011
#define RXCONT_MODE            0x85 // b1000 0101
#define RXSINGLE_MODE          0x86 // b1000 0110

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
    // printf("Setting LED %s\n", led_on ? "ON" : "OFF");
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}


// Timing variables
absolute_time_t start_time;
absolute_time_t checkpoint_time;

// Function to start timing
void start_timer() {
    start_time = get_absolute_time();
    checkpoint_time = start_time;
    printf("Timer started\n");
}

// Function to print elapsed time since last checkpoint
void checkpoint_timer(const char* checkpoint_name) {
    absolute_time_t current_time = get_absolute_time();
    uint32_t elapsed_since_last = absolute_time_diff_us(checkpoint_time, current_time) / 1000;
    uint32_t elapsed_since_start = absolute_time_diff_us(start_time, current_time) / 1000;
    
    printf("TIMING: %s - %lu ms since last checkpoint, %lu ms since start\n", 
           checkpoint_name, elapsed_since_last, elapsed_since_start);
    
    checkpoint_time = current_time;  // Update checkpoint time
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

// TODO Chip select line is oscillating
// Should not be
// Messages should be MSB first
// MSB of first byte should be 1 for write, 0 for read
// next byte is data to send

void lora_init() {
    printf("Starting LoRa Initialization...\n");

    // Initialize SPI port at 1 MHz
    printf("Initializing SPI at 1MHz\n");// Set SPI format (CPOL=0, CPHA=0)
    spi_init(SPI_PORT, 1e6);
    
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
    
    // Reset pin
    printf("Configuring Reset pin\n");
    gpio_init(PIN_RST);
    gpio_set_dir(PIN_RST, GPIO_OUT);

    // TXEN Pin set
    printf("Configuring TXEN pin\n");
    gpio_init(PIN_TX);
    gpio_set_dir(PIN_TX, GPIO_OUT);
    gpio_put(PIN_TX, 0); // Will remain low
    
    // RXEN Pin set
    printf("Configuring RXEN pin\n");
    gpio_init(PIN_RX);
    gpio_set_dir(PIN_RX, GPIO_OUT);
    gpio_put(PIN_RX, 0); // Initially low (inactive)

    // Reset the LoRa module
    // printf("Resetting LoRa module\n");
    // lora_reset();

    // Allow for reset
    // sleep_ms(100);

    // Check Lora connection
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

    // Set OCP
    printf("Configuring Over Currnet Protection\n");
    lora_write_reg(REG_OCP, 0x3F);

    // low noise amplifier
    // printf("Setting Low Noise Amplifier\n");
    // lora_write_reg(REG_LNA, 0b10100000); // 101-00-0-00  G5 gain

    // Reset FIFO buffer pointer
    printf("Reset FIFO\n");
    lora_write_reg(REG_FIFO_ADDR_PTR,0x00);
    lora_write_reg(REG_FIFO_TX_BASE_ADDR,0x00);
    lora_write_reg(REG_FIFO_RX_BASE_ADDR,0x00);

    // printf("Setting IRQ Mask\n");
    // lora_write_reg(REG_IRQ_FLAGS_MASK,0b01000000); // 01000000

    // Set modem config
    printf("Configuring Modem Settings\n");
    lora_write_reg(REG_MODEM_CONFIG_1, 0b01100011);  // 0110-001-1 BW=62.5kHz, CR=4/5, implicit header
    lora_write_reg(REG_MODEM_CONFIG_2, 0b01110111);  // 0111-0-1-11 SF=7, crc rx on, 
    lora_write_reg(REG_MODEM_CONFIG_3, 0b00000100);  // 0000-0-1-00 LNA gain set by the internal AGC loop
    lora_write_reg(REG_TIMEOUT_LSB, 0xFF);   // Set timeout to max

    printf("Configuring Preamble\n");
    lora_write_reg(REG_PREAMBLE_MSB,0x00);
    lora_write_reg(REG_PREAMBLE_LSB,0x08);

    // Set Sync Word (added for debugging)
    printf("Setting Sync Word\n");
    lora_write_reg(REG_SYNC_WORD, 0x12);  // Example sync word

    // Set to standby
    printf("Setting to RX Single mode\n");
    lora_write_reg(REG_OP_MODE, RXSINGLE_MODE);
    gpio_put(PIN_RX,1);

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

    int8_t detect_thres_value = lora_read_reg(REG_DETECTION_THRESHOLD);
    
    printf("Signal Quality:\n");
    printf("  Current RSSI: %.1f dBm\n", rssi);
    printf("  Last Packet RSSI: %.1f dBm\n", pkt_rssi);
    printf("  Last Packet SNR: %.1f dB\n", snr);
    printf("  Detection Threshold: 0x%02x \n", detect_thres_value);
    
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
    // printf("Performing LoRa module hardware reset\n");
    gpio_put(PIN_RST, 0);
    sleep_ms(10);
    gpio_put(PIN_RST, 1);
    sleep_ms(10);
    // printf("LoRa module reset complete\n");
}

void print_binary(uint8_t num) {
    for (int i = 7; i >= 0; i--) {
        printf("%c", (num & (1 << i)) ? '1' : '0');
    }
}

void lora_write_reg(uint8_t reg, uint8_t data) {
    // printf("Writing to register 0x%02X: value 0x%02X\n", reg, data);
    
    uint8_t buf[2] = {reg | 0x80, data};  // Set MSB for write
    gpio_put(PIN_CS, 0);
    int len = spi_write_blocking(SPI_PORT, buf, 2);
    gpio_put(PIN_CS, 1);
}

uint8_t lora_read_reg(uint8_t reg) {

    // printf("Reading from register 0x%02X ", reg);

    uint8_t TX_buf[2] = {reg & 0x7F, 0x00};  // Clear MSB for read
    uint8_t RX_buf[2];

    gpio_put(PIN_CS, 0);
    spi_write_read_blocking(SPI_PORT, TX_buf, RX_buf, 2);
    gpio_put(PIN_CS, 1);

    // printf("value: 0x%02X 0x%02X\n", RX_buf[0], RX_buf[1]);

    return RX_buf[1];
}

void lora_receive_packet(uint8_t *buffer, uint8_t *len) {
    // Set RXEN pin high if needed
    gpio_put(PIN_RX, 1);
    
    // Clear any pending IRQ flags first
    lora_write_reg(REG_IRQ_FLAGS, 0xFF);
    
    // Switch to single receive mode
    lora_write_reg(REG_OP_MODE, RXSINGLE_MODE);
    
    printf("Waiting for received packet\n");
    
    // Wait for RX done flag
    uint32_t start_time = time_us_32();
    bool timeout = false;
    
    while ((lora_read_reg(REG_IRQ_FLAGS) & 0x40) == 0) {  // RX done flag
        // Check for timeout (5 seconds)
        if (time_us_32() - start_time > 5000000) {
            printf("Receive timeout\n");
            timeout = true;
            break;
        }
        // Check less frequently to reduce console spam
        sleep_ms(1);
    }
    
    // Only read signal quality after receiving or timeout
    read_signal_quality();

    printf("IRQ Flags: 0x%02X\n", lora_read_reg(REG_IRQ_FLAGS));
    
    if (!timeout) {
        // Read payload length
        *len = lora_read_reg(REG_PAYLOAD_LENGTH);
        printf("Payload length: %d\n", *len);
        
        // Validate payload length
        if (*len > 0 && *len < 256) {
            // Read data from FIFO
            printf("Reading data from FIFO\n");
            gpio_put(PIN_CS, 0);
            uint8_t reg = REG_FIFO & 0x7F;  // Read mode
            spi_write_blocking(SPI_PORT, &reg, 1);
            spi_read_blocking(SPI_PORT, 0, buffer, *len);
            gpio_put(PIN_CS, 1);
        } else {
            printf("Invalid payload length: %d\n", *len);
            *len = 0;
        }
    } else {
        *len = 0;  // No data received due to timeout
    }
    
    // Set RXEN pin low
    gpio_put(PIN_RX, 0);
    
    // Clear IRQ flags
    lora_write_reg(REG_IRQ_FLAGS, 0xFF);
    
    // Reset FIFO pointer
    lora_write_reg(REG_FIFO_ADDR_PTR, 0x00);
}

int main() {
    stdio_init_all();

    // Initialize LED
    pico_led_init();

    sleep_ms(5000);
    printf("Starting LoRa RX Test\n");

    start_timer();

    // Initialize LoRa
    printf("Initializing LoRa Module\n");
    lora_init();

    uint8_t received_data[256];
    uint8_t received_len = 0;

    printf("Starting RX Loop\n");
    checkpoint_timer("RX Loop\n");

    while (1) {
        pico_set_led(true);
        printf("Waiting for a packet...\n");
        
        // Wait and receive a packet
        lora_receive_packet(received_data, &received_len);
        
        // Only process if we actually received data
        if (received_len > 0) {
            printf("Received packet of length %d\n", received_len);
            
            // Print out the received message contents
            printf("Received message: ");
            for (int i = 0; i < received_len; i++) {
                // Consider removing the flip_endian function unless specifically needed
                printf("%c", received_data[i]);
            }
            printf("\n");
            
            // Print hex values for debugging
            printf("Raw data (hex): ");
            for (int i = 0; i < received_len; i++) {
                printf("%02X ", received_data[i]);
            }
            printf("\n\n\n");
        }
        
        pico_set_led(false);

        // Add a small delay before trying to receive again
        sleep_ms(100);
    }

    return 0;
}
