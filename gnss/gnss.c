// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include <stdio.h>
// #include <string.h>

// #define GPS_I2C i2c0
// #define GPS_I2C_ADDR 0x42  // Typical I2C address for Ublox GPS module (this can vary)
// #define I2C_BAUD 100000  // I2C baud rate (400 kHz)

// #define GPS_BUFFER_SIZE 1024
// char gps_buffer[GPS_BUFFER_SIZE];
// int buffer_index = 0;

// // Thing Plus RP2040 Qwiic pins
// #define SDA_PIN 4
// #define SCL_PIN 5



// // Function to read data from the GPS via I2C
// void read_gps_data() {

        
//     // Setup
//     // printf("Initializing I2C...\n");
//     // i2c_init(GPS_I2C, 100000);  // 100kHz

//     // // Setup pins with validation prints
//     // gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     // gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     // gpio_pull_up(SDA_PIN);
//     // gpio_pull_up(SCL_PIN);

//     // printf("Pin functions set:\n");
//     // printf("SDA (GPIO4) function: %d\n", gpio_get_function(SDA_PIN));
//     // printf("SCL (GPIO5) function: %d\n", gpio_get_function(SCL_PIN));

//     // // Try direct communication with NEO-M9N
//     // uint8_t rxdata;
//     // int ret = i2c_read_blocking(GPS_I2C, 0x42, &rxdata, 1, false);
//     // printf("Direct NEO-M9N read result: %d\n", ret);

//     // // Then do full scan
//     // for (uint8_t addr = 0; addr < 128; addr++) {
//     //     ret = i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false);
//     //     if (ret >= 0) {
//     //         printf("Device responded at address: 0x%02X\n", addr);
//     //     }
//     // }

//     // printf("Attempting to read data \n");

//     // uint8_t buffer[2];
//     // // int ret;

//     // uint8_t reg = 0x01;
//     // ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, &reg, 1,true );
//     // ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, buffer, 2, false);

//     // printf("read result: %d, class: 0x%02X, ID: 0x%02X\n", ret, buffer[0], buffer[1]);

//     // for (uint8_t addr = 0; addr < 128; addr++) {
//     //     // uint8_t rxdata;
//     //     if (i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false) >= 0) {
//     //         printf("Found I2C device at address: 0x%02X\n", addr);
//     //     }
//     // }

//     // uint8_t reg = 0x01;  // Example register address
//     // int ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, &reg, 1, true);  // Write the register address
//     // if (ret < 0) {
//     //     printf("I2C Write failed: %d\n", ret);
//     //     return;  // Handle error
//     // }

//     // uint8_t rxdata;
//     // printf("Scanning I2C bus...\n");
//     // for (uint8_t addr = 0; addr < 128; addr++) {
//     //     int ret = i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false);
//     //     if (ret >= 0) {
//     //         printf("Device responded at address: 0x%02X\n", addr);
//     //     }
//     // }

//     uint8_t data[64]; // Buffer to store incoming data
//     int length = i2c_read_blocking(GPS_I2C, GPS_I2C_ADDR, data, sizeof(data), false);

//     printf("GPS i2c: %x\n", GPS_I2C);
//     printf("GPS i2c: %x\n", *GPS_I2C);
    
//     if (length > 0) {
//         // Process the received data
//         for (int i = 0; i < length; i++) {
//             char byte = data[i];

//             // Store the byte in the buffer
//             if (byte == '\n') {
//                 gps_buffer[buffer_index] = '\0';  // Null-terminate the string
//                 // Process NMEA sentence
//                 printf("Received NMEA: %s\n", gps_buffer);
//                 buffer_index = 0;  // Reset buffer for next sentence
//             } else {
//                 gps_buffer[buffer_index++] = byte;
//                 // Make sure we don't overflow the buffer
//                 if (buffer_index >= GPS_BUFFER_SIZE - 1) {
//                     buffer_index = 0;  // Reset buffer to avoid overflow
//                 }
//             }
//         }
//     }
// }

// // Setup I2C
// void setup_i2c() {
//     // Initialize I2C at the desired baud rate
//     i2c_init(GPS_I2C, I2C_BAUD);

//     // Set the I2C pins (GPIO0 for SDA, GPIO1 for SCL)
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

//     // Enable internal pullups on the SDA and SCL pins
//     gpio_pull_up(SDA_PIN);
//     gpio_pull_up(SCL_PIN);
// }

// int main() {
//     // Initialize the stdio (for debugging)
//     stdio_init_all();
    
//     // Setup I2C communication
//     setup_i2c();
    
//     printf("GPS I2C Logger Started!\n");

//     // Main loop
//     while (true) {
//         read_gps_data();
//         sleep_ms(1000);  // Wait a little before reading again
//     }

//     return 0;
// }






// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include <stdio.h>
// #include <string.h>

// #define GPS_I2C i2c0
// #define GPS_I2C_ADDR 0x42  // NEO-M9N default address
// #define I2C_BAUD 115200    // Starting with 100kHz for reliability

// // Thing Plus RP2040 Qwiic pins
// #define SDA_PIN 0
// #define SCL_PIN 1

// // UBX protocol sync chars
// #define UBX_SYNC_1 0xB5
// #define UBX_SYNC_2 0x62

// // Function to scan I2C bus and print found devices
// void scan_i2c_bus() {
//     printf("\nScanning I2C bus...\n");
//     for (uint8_t addr = 0; addr < 128; addr++) {
//         uint8_t rxdata;
//         int ret = i2c_read_blocking(GPS_I2C, addr, &rxdata, 1, false);
//         if (ret >= 0) {
//             printf("* Device found at address: 0x%02X (decimal: %d)\n", addr, addr);
//             // If we found the NEO-M9N
//             if (addr == GPS_I2C_ADDR) {
//                 printf("  --> This appears to be the NEO-M9N!\n");
//             }
//         }
//     }
//     printf("I2C scan complete\n\n");
// }

// // Try to read the NEO-M9N version info
// bool test_gps_communication() {
//     uint8_t reg = 0xFD; // Register containing bytes available
//     uint8_t bytes_available[2];
    
//     printf("Testing NEO-M9N communication...\n");
    
//     // Try to read the bytes available register
//     int ret = i2c_write_blocking(GPS_I2C, GPS_I2C_ADDR, &reg, 1, true);
//     if (ret < 0) {
//         printf("Failed to write to NEO-M9N\n");
//         return false;
//     }
    
//     ret = i2c_read_blocking(GPS_I2C, GPS_I2C_ADDR, bytes_available, 2, false);
//     if (ret < 0) {
//         printf("Failed to read from NEO-M9N\n");
//         return false;
//     }
    
//     uint16_t available = (bytes_available[0] << 8) | bytes_available[1];
//     printf("Bytes available in GPS: %d\n", available);
    
//     return true;
// }

// // Setup I2C with validation
// bool setup_i2c() {
//     printf("Starting I2C setup...\n");
    
//     // Initialize I2C
//     i2c_init(GPS_I2C, I2C_BAUD);
//     printf("I2C initialized at %d Hz\n", I2C_BAUD);

//     // Configure pins
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(SDA_PIN);
//     gpio_pull_up(SCL_PIN);
    
//     // Verify pin configuration
//     printf("Pin configuration:\n");
//     printf("* SDA (GPIO%d) function: %d (should be 3 for I2C)\n", 
//            SDA_PIN, gpio_get_function(SDA_PIN));
//     printf("* SCL (GPIO%d) function: %d (should be 3 for I2C)\n", 
//            SCL_PIN, gpio_get_function(SCL_PIN));
    
//     // Validate pin functions
//     if (gpio_get_function(SDA_PIN) != GPIO_FUNC_I2C || 
//         gpio_get_function(SCL_PIN) != GPIO_FUNC_I2C) {
//         printf("ERROR: Pins not properly configured for I2C!\n");
//         return false;
//     }
    
//     printf("I2C setup complete\n");
//     return true;
// }

// int main() {
//     // Initialize stdio for debugging output
//     stdio_init_all();
    
//     // Wait a moment for serial to connect
//     sleep_ms(2000);
//     printf("\n\nNEO-M9N I2C Debug Program Starting...\n");
    
//     // Setup I2C
//     if (!setup_i2c()) {
//         printf("Failed to setup I2C. Halting.\n");
//         while(1) { sleep_ms(1000); }
//     }
    
//     // Main loop
//     while (true) {
//         gpio_init(25);
//         gpio_set_dir(25, GPIO_OUT);
//         gpio_put(25,1);
//         sleep_ms(500);
//         gpio_put(25,0);
//         sleep_ms(500);

//         scan_i2c_bus();
        
//         if (test_gps_communication()) {
//             printf("Successfully communicated with NEO-M9N!\n");
//         } else {
//             printf("Failed to communicate with NEO-M9N\n");
//         }
        
//         printf("\nWaiting 5 seconds before next attempt...\n");
//         sleep_ms(5000);
//     }

//     return 0;
// }




// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include <stdio.h>

// #define LED_PIN 25
// #define SDA_PIN 4
// #define SCL_PIN 5
// #define GPS_I2C i2c0
// #define GPS_ADDR 0x42

// // NEO-M9N registers
// #define DATA_STREAM_REG 0xFF
// #define BYTES_AVAILABLE_REG 0xFD

// void read_gps_data() {
//     uint8_t bytes_available[2];
//     uint8_t reg = BYTES_AVAILABLE_REG;
    
//     printf("Attempting I2C write to address 0x%02X with register 0x%02X\n", GPS_ADDR, reg);
    
//     // Check if bus is busy first
//     if (i2c_get_write_available(GPS_I2C) == 0) {
//         printf("I2C bus is busy or not ready\n");
//         return;
//     }
    
//     int ret = i2c_write_blocking(GPS_I2C, GPS_ADDR, &reg, 1, true);
//     printf("Write returned: %d\n", ret);
    
//     if (ret < 0) {
//         // Try to reset the I2C peripheral
//         i2c_deinit(GPS_I2C);
//         sleep_ms(100);
//         i2c_init(GPS_I2C, 100000);
//         printf("Attempted I2C reset after error\n");
//         return;
//     }
//     // ... rest of your function
// }


// int main() {
//     stdio_init_all();
//     stdio_usb_init();
//     sleep_ms(3000);
    
//     // Initialize LED
//     gpio_init(LED_PIN);
//     gpio_set_dir(LED_PIN, GPIO_OUT);
    
//     // First set up the pins
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(SDA_PIN);
//     gpio_pull_up(SCL_PIN);
    
//     // Give the pull-ups time to stabilize
//     sleep_ms(250);
    
//     // Now initialize I2C with a slower clock for testing
//     printf("Initializing I2C...\n");
//     i2c_init(GPS_I2C, 50000);  // Try 50kHz
    
//     // Check if I2C initialized successfully
//     if (!i2c_get_write_available(GPS_I2C)) {
//         printf("I2C failed to initialize properly\n");
//         return -1;
//     }
    
//     printf("I2C initialized successfully\n");
//     printf("Starting GPS read loop...\n");
    
//     while (true) {
//         gpio_put(LED_PIN, 1);
//         read_gps_data();
//         sleep_ms(1000);  // Longer delay for testing
//         gpio_put(LED_PIN, 0);
//         sleep_ms(1000);
//     }
    
//     return 0;
// }





// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include "hardware/gpio.h"

// // I2C defines
// #define I2C_PORT i2c0
// #define I2C_SDA 0  // GPIO4 for SDA
// #define I2C_SCL 1  // GPIO5 for SCL
// #define I2C_FREQ 115200  // 100kHz standard mode

// // Function to initialize I2C
// void init_i2c() {
//     // Initialize I2C port
//     i2c_init(I2C_PORT, I2C_FREQ);
    
//     // Set up I2C pins
//     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
//     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    
//     // Enable internal pull-ups
//     gpio_pull_up(I2C_SDA);
//     gpio_pull_up(I2C_SCL);
// }

// static inline void i2c_program_init(PIO pio, uint sm, uint offset, uint pin_sda, uint pin_scl) {
//     assert(pin_scl == pin_sda + 1);
//     pio_sm_config c = i2c_program_get_default_config(offset);

//     // IO mapping
//     sm_config_set_out_pins(&c, pin_sda, 1);
//     sm_config_set_set_pins(&c, pin_sda, 1);
//     sm_config_set_in_pins(&c, pin_sda);
//     sm_config_set_sideset_pins(&c, pin_scl);
//     sm_config_set_jmp_pin(&c, pin_sda);

//     sm_config_set_out_shift(&c, false, true, 16);
//     sm_config_set_in_shift(&c, false, true, 8);

//     float div = (float)clock_get_hz(clk_sys) / (32 * 100000);
//     sm_config_set_clkdiv(&c, div);

//     // Try to avoid glitching the bus while connecting the IOs. Get things set
//     // up so that pin is driven down when PIO asserts OE low, and pulled up
//     // otherwise.
//     gpio_pull_up(pin_scl);
//     gpio_pull_up(pin_sda);
//     uint32_t both_pins = (1u << pin_sda) | (1u << pin_scl);
//     pio_sm_set_pins_with_mask(pio, sm, both_pins, both_pins);
//     pio_sm_set_pindirs_with_mask(pio, sm, both_pins, both_pins);
//     pio_gpio_init(pio, pin_sda);
//     gpio_set_oeover(pin_sda, GPIO_OVERRIDE_INVERT);
//     pio_gpio_init(pio, pin_scl);
//     gpio_set_oeover(pin_scl, GPIO_OVERRIDE_INVERT);
//     pio_sm_set_pins_with_mask(pio, sm, 0, both_pins);

//     // Clear IRQ flag before starting, and make sure flag doesn't actually
//     // assert a system-level interrupt (we're using it as a status flag)
//     pio_set_irq0_source_enabled(pio, (enum pio_interrupt_source)((uint)pis_interrupt0 + sm), false);
//     pio_set_irq1_source_enabled(pio, (enum pio_interrupt_source)((uint)pis_interrupt0 + sm), false);
//     pio_interrupt_clear(pio, sm);

//     // Configure and start SM
//     pio_sm_init(pio, sm, offset + i2c_offset_entry_point, &c);
//     pio_sm_set_enabled(pio, sm, true);
// }

// // Function to scan I2C bus
// void scan_i2c() {
//     printf("\nI2C Bus Scan\n");
//     printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

//     for (int addr = 0; addr < 128; addr++) {
//         if (addr % 16 == 0) {
//             printf("%02x ", addr);
//         }

//         // Try to read from the current address
//         uint8_t rx_data;
//         int ret = i2c_read_blocking(I2C_PORT, addr, &rx_data, 1, false);
        
//         if (ret >= 0) {
//             printf("*  "); // Device found
//         } else {
//             printf(".  "); // No device
//         }

//         if (addr % 16 == 15) {
//             printf("\n");
//         }
//     }
// }

// int main() {
//     // Initialize USB serial for output
//     stdio_init_all();
    
//     // Wait for USB serial to be ready
//     sleep_ms(2000);
    
//     printf("RP2040 I2C Scanner Starting...\n");
    
//     // Initialize I2C
//     init_i2c();
    
//     // Main loop
//     while (1) {
//         scan_i2c();
//         printf("\nScan complete. Waiting 5 seconds before next scan...\n");
//         sleep_ms(5000);
//     }

//     return 0;
// }

// #include "pico/stdlib.h"
// #include "hardware/pio.h"
// #include "hardware/clocks.h"
// #include "hardware/gpio.h"
// #include "hardware/i2c.h"
// #include <stdio.h>

// // Pin definitions
// #define PIN_SDA 4
// #define PIN_SCL 5  // Must be PIN_SDA + 1

// // PIO program declaration - this needs to be provided from your .pio file
// extern const PIO_program i2c_program;

// // Constants for the set_scl_sda instructions
// enum {
//     I2C_SC0_SD0,
//     I2C_SC1_SD0,
//     I2C_SC1_SD1,
//     COUNT_OF_SET_SCL_SDA_INSTRUCTIONS
// };

// static const uint16_t set_scl_sda_program_instructions[] = {
//     0x0000, // 00 SCL=0 SDA=0
//     0x0002, // 01 SCL=1 SDA=0
//     0x0003  // 11 SCL=1 SDA=1
// };

// #define PIO_I2C_ICOUNT_LSB 10

// // Function to check if address is reserved
// static bool reserved_addr(uint8_t addr) {
//     return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
// }

// // Helper function for PIO FIFO writes
// static void pio_i2c_put_or_err(PIO pio, uint sm, uint16_t data) {
//     if (!pio_sm_is_tx_fifo_full(pio, sm)) {
//         pio_sm_put(pio, sm, data);
//     } else {
//         pio_interrupt_set(pio, sm);
//     }
// }

// // Generate I2C start condition
// void pio_i2c_start(PIO pio, uint sm) {
//     pio_i2c_put_or_err(pio, sm, 1u << PIO_I2C_ICOUNT_LSB);  // Escape code for 2 instruction sequence
//     pio_i2c_put_or_err(pio, sm, set_scl_sda_program_instructions[I2C_SC1_SD0]);  // We are already in idle state, just pull SDA low
//     pio_i2c_put_or_err(pio, sm, set_scl_sda_program_instructions[I2C_SC0_SD0]);  // Also pull clock low so we can present data
// }

// // Check for I2C errors
// bool pio_i2c_check_error(PIO pio, uint sm) {
//     return pio_interrupt_get(pio, sm);
// }

// // Resume I2C after an error
// void pio_i2c_resume_after_error(PIO pio, uint sm) {
//     pio_sm_drain_tx_fifo(pio, sm);
//     pio_sm_exec(pio, sm, (pio->sm[sm].execctrl & PIO_SM0_EXECCTRL_WRAP_BOTTOM_BITS) >> PIO_SM0_EXECCTRL_WRAP_BOTTOM_LSB);
//     pio_interrupt_clear(pio, sm);
// }

// // Initialize I2C with PIO
// static inline void i2c_program_init(PIO pio, uint sm, uint offset, uint pin_sda, uint pin_scl) {
//     assert(pin_scl == pin_sda + 1);
//     pio_sm_config c = i2c_program_get_default_config(offset);

//     // IO mapping
//     sm_config_set_out_pins(&c, pin_sda, 1);
//     sm_config_set_set_pins(&c, pin_sda, 1);
//     sm_config_set_in_pins(&c, pin_sda);
//     sm_config_set_sideset_pins(&c, pin_scl);
//     sm_config_set_jmp_pin(&c, pin_sda);

//     sm_config_set_out_shift(&c, false, true, 16);
//     sm_config_set_in_shift(&c, false, true, 8);

//     float div = (float)clock_get_hz(clk_sys) / (32 * 100000);
//     sm_config_set_clkdiv(&c, div);

//     // Try to avoid glitching the bus while connecting the IOs
//     gpio_pull_up(pin_scl);
//     gpio_pull_up(pin_sda);
//     uint32_t both_pins = (1u << pin_sda) | (1u << pin_scl);
//     pio_sm_set_pins_with_mask(pio, sm, both_pins, both_pins);
//     pio_sm_set_pindirs_with_mask(pio, sm, both_pins, both_pins);
//     pio_gpio_init(pio, pin_sda);
//     gpio_set_oeover(pin_sda, GPIO_OVERRIDE_INVERT);
//     pio_gpio_init(pio, pin_scl);
//     gpio_set_oeover(pin_scl, GPIO_OVERRIDE_INVERT);
//     pio_sm_set_pins_with_mask(pio, sm, 0, both_pins);

//     // Clear IRQ flag before starting
//     pio_set_irq0_source_enabled(pio, (enum pio_interrupt_source)((uint)pis_interrupt0 + sm), false);
//     pio_set_irq1_source_enabled(pio, (enum pio_interrupt_source)((uint)pis_interrupt0 + sm), false);
//     pio_interrupt_clear(pio, sm);

//     // Configure and start SM
//     pio_sm_init(pio, sm, offset + i2c_offset_entry_point, &c);
//     pio_sm_set_enabled(pio, sm, true);
// }

// // Main function
// int main() {
//     stdio_init_all();

//     PIO pio = pio0;
//     uint sm = 0;
//     uint offset = pio_add_program(pio, &i2c_program);
//     i2c_program_init(pio, sm, offset, PIN_SDA, PIN_SCL);

//     printf("\nPIO I2C Bus Scan\n");
//     printf(" 0 1 2 3 4 5 6 7 8 9 A B C D E F\n");

//     for (int addr = 0; addr < (1 << 7); ++addr) {
//         if (addr % 16 == 0) {
//             printf("%02x ", addr);
//         }
//         // Perform a 0-byte read from the probe address
//         int result;
//         if (reserved_addr(addr))
//             result = -1;
//         else
//             result = i2c_read_blocking(pio, sm, addr, NULL, 0);

//         printf(result < 0 ? "." : "@");
//         printf(addr % 16 == 15 ? "\n" : " ");
//     }
//     printf("Done.\n");
//     return 0;
// }

// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include "hardware/gpio.h"
// #include "pico/binary_info.h"
// #include <stdio.h>

// #define I2C_PORT i2c0            // Using I2C0 interface
// #define SDA_PIN  4               // SDA Pin (can be any valid GPIO pin)
// #define SCL_PIN  5               // SCL Pin (can be any valid GPIO pin)
// #define NEO_M9N_ADDR 0x42        // Default I2C address for NEO-M9N, check your datasheet

// void i2c_init_custom() {
//     // Initialize the I2C interface with the desired frequency
//     i2c_init(I2C_PORT, 100000); // Set the baud rate to 100kHz (standard for many devices)
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(SDA_PIN); // Enable internal pull-up resistors
//     gpio_pull_up(SCL_PIN); // Enable internal pull-up resistors
//     bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C));
// }

// void write_data(uint8_t reg, uint8_t value) {
//    uint8_t data[2] = {reg, value};
//    i2c_write_blocking(I2C_PORT, NEO_M9N_ADDR, data, 2, false);
// }

// uint8_t read_data(uint8_t reg) {
//     uint8_t value;
//     i2c_write_blocking(I2C_PORT, NEO_M9N_ADDR, &reg, 1, true); // Send the register address
//     i2c_read_blocking(I2C_PORT, NEO_M9N_ADDR, &value, 1, false); // Read the value from that register
//     return value;
// }

// void setup_neo_m9n() {
//     // Example of initializing the NEO-M9N: (this may change depending on your requirements)
//     // Send some configuration commands to set the NEO-M9N to I2C mode or adjust settings
//     // Example: setting baud rate or setting operational mode (check the NEO-M9N manual for specific commands)
    
//     write_data(0xFD, 0x01); // Example command (register and value - modify based on NEO-M9N documentation)
    
//     // Wait for a while to allow the device to initialize
//     sleep_ms(1000);
// }

// void read_position() {
//     // Example of reading some GNSS data from the NEO-M9N
//     // Read position data (depending on the NEO-M9N's I2C response format)
    
//     uint8_t data = read_data(0xFF); // Example: Reading data from register 0x10 (change according to your need)
//     printf("Data from NEO-M9N: %d\n", data); // Print data to console
// }

// int main() {
//     // Initialize standard input/output
//     stdio_init_all();
    
//     // Initialize I2C interface
//     i2c_init_custom();
    
//     // Setup NEO-M9N receiver
//     setup_neo_m9n();
    
//     // Main loop: Read GNSS position data
//     while (true) {
//         read_position();
//         sleep_ms(1000); // Delay between reads
//     }

//     return 0;
// }

// #include "pico/stdlib.h"
// #include "hardware/i2c.h"
// #include "pico/binary_info.h"
// #include <stdio.h>

// // Pin Definitions
// #define LED_PIN 25
// #define SDA_PIN 4
// #define SCL_PIN 5
// #define GPS_I2C i2c0
// #define GPS_ADDR 0x42

// // NEO-M9N registers
// #define DATA_STREAM_REG 0xFF
// #define BYTES_AVAILABLE_REG 0xFD

// // Debug function prototypes
// void check_i2c_state(void);
// void test_i2c_pins(void);
// bool verify_i2c_device(void);
// void debug_i2c_bus(void);
// void scan_i2c_addresses(void);

// void scan_i2c_addresses(void) {
//     printf("\n=== Scanning I2C Addresses ===\n");
//     uint8_t reg = BYTES_AVAILABLE_REG;
    
//     // Try common GPS addresses
//     uint8_t addresses[] = {0x42, 0x84, 0x77};  // Common GPS addresses
    
//     for (int i = 0; i < sizeof(addresses); i++) {
//         printf("Testing address 0x%02X: ", addresses[i]);
        
//         // Try to write to the device
//         int ret = i2c_write_blocking(GPS_I2C, addresses[i], &reg, 1, false);
        
//         if (ret >= 0) {
//             printf("DEVICE FOUND!\n");
//             printf("Working address is: 0x%02X\n", addresses[i]);
//             return;
//         } else {
//             printf("No response\n");
//         }
//         sleep_ms(100);
//     }
    
//     printf("\nNo GPS device found at common addresses.\n");
//     printf("Performing full bus scan...\n");
    
//     // Full bus scan
//     for (int addr = 0x08; addr < 0x77; addr++) {
//         printf("Scanning address 0x%02X: ", addr);
        
//         int ret = i2c_write_blocking(GPS_I2C, addr, &reg, 1, false);
        
//         if (ret >= 0) {
//             printf("DEVICE FOUND!\n");
//             return;
//         } else {
//             printf(".");
//         }
        
//         if (addr % 16 == 15) printf("\n");
//         sleep_ms(10);
//     }
//     printf("\nScan complete.\n");
// }


// // Function to check I2C line states
// void check_i2c_state(void) {
//     printf("\n=== I2C Line States ===\n");
//     printf("SDA Current State: %d\n", gpio_get(SDA_PIN));
//     printf("SCL Current State: %d\n", gpio_get(SCL_PIN));
//     printf("SDA Function Mode: %d (Should be 3 for I2C)\n", gpio_get_function(SDA_PIN));
//     printf("SCL Function Mode: %d (Should be 3 for I2C)\n", gpio_get_function(SCL_PIN));
// }

// // Function to test I2C pins over time
// void test_i2c_pins(void) {
//     printf("\n=== Testing I2C Lines for 1 second ===\n");
//     int sda_low_count = 0;
//     int scl_low_count = 0;
    
//     for(int i = 0; i < 10; i++) {
//         if(gpio_get(SDA_PIN) == 0) sda_low_count++;
//         if(gpio_get(SCL_PIN) == 0) scl_low_count++;
//         sleep_ms(100);
//     }
    
//     printf("Times SDA was LOW: %d/10\n", sda_low_count);
//     printf("Times SCL was LOW: %d/10\n", scl_low_count);
    
//     if(sda_low_count > 8) printf("WARNING: SDA might be stuck LOW\n");
//     if(scl_low_count > 8) printf("WARNING: SCL might be stuck LOW\n");
// }

// // Function to verify I2C device presence
// bool verify_i2c_device(void) {
//     printf("\n=== Checking for GPS Device ===\n");
//     uint8_t rxdata;
//     int ret;
    
//     // Try to read one byte from the device
//     ret = i2c_read_blocking(GPS_I2C, GPS_ADDR, &rxdata, 1, false);
    
//     if (ret < 0) {
//         printf("Error: Could not communicate with GPS device\n");
//         printf("Return code: %d\n", ret);
//         return false;
//     }
    
//     printf("Successfully communicated with GPS device!\n");
//     return true;
// }

// // Function to debug I2C bus
// void debug_i2c_bus(void) {
//     printf("\n=== I2C Bus Status ===\n");
//     printf("Write available: %d\n", i2c_get_write_available(GPS_I2C));
//     printf("Read available: %d\n", i2c_get_read_available(GPS_I2C));
// }

// // Main GPS data reading function
// void read_gps_data(void) {
//     uint8_t bytes_available[2];
//     uint8_t reg = BYTES_AVAILABLE_REG;
    
//     printf("\n=== Attempting GPS Read ===\n");
    
//     // Check bus state before attempting read
//     debug_i2c_bus();
    
//     int ret = i2c_write_blocking(GPS_I2C, GPS_ADDR, &reg, 1, true);
//     printf("Write command return code: %d\n", ret);
    
//     if (ret < 0) {
//         printf("Error writing to GPS. Attempting reset...\n");
//         i2c_deinit(GPS_I2C);
//         sleep_ms(100);
//         i2c_init(GPS_I2C, 100000);
//         return;
//     }
    
//     ret = i2c_read_blocking(GPS_I2C, GPS_ADDR, bytes_available, 2, false);
//     printf("Read command return code: %d\n", ret);
    
//     if (ret >= 0) {
//         printf("Bytes available: %d\n", (bytes_available[0] << 8) | bytes_available[1]);
//     }
// }

// int main() {
//     // Initialize USB serial first
//     stdio_init_all();
//     stdio_usb_init();
//     sleep_ms(3000);  // Wait for USB to initialize
    
//     printf("\n\n=== Starting GPS Debug Program ===\n");
//     printf("Compiled on: %s %s\n", __DATE__, __TIME__);
    
//     // Initialize LED
//     gpio_init(LED_PIN);
//     gpio_set_dir(LED_PIN, GPIO_OUT);
//     printf("LED initialized\n");
    
//     // Initialize I2C pins
//     printf("\nConfiguring I2C pins...\n");
//     gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    
//     // Optional: Enable internal pullups if needed
//     // Uncomment these lines if you need internal pullups
    
//     gpio_pull_up(SDA_PIN);
//     gpio_pull_up(SCL_PIN);
//     bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
//     printf("Internal pullups enabled\n");
    
    
//     sleep_ms(250);  // Allow pins to stabilize
    
//     // Check initial pin states
//     check_i2c_state();
    
//     // Initialize I2C peripheral
//     printf("\nInitializing I2C...\n");
//     i2c_init(GPS_I2C, 100000);  // 100kHz standard mode
    
//     if (!i2c_get_write_available(GPS_I2C)) {
//         printf("ERROR: I2C failed to initialize\n");
//         while(1) {
//             gpio_put(LED_PIN, 1);
//             sleep_ms(100);
//             gpio_put(LED_PIN, 0);
//             sleep_ms(100);
//         }
//     }
    
//     printf("I2C initialized successfully\n");
    
//     scan_i2c_addresses();
//     // Test I2C lines
//     test_i2c_pins();
    
//     // Verify device presence
//     if (!verify_i2c_device()) {
//         printf("WARNING: GPS device not responding\n");
//     }
    
//     printf("\n=== Starting main loop ===\n");
    
//     while (true) {
//         gpio_put(LED_PIN, 1);
//         read_gps_data();
//         check_i2c_state();
//         scan_i2c_addresses();
//         sleep_ms(1000);
//         gpio_put(LED_PIN, 0);
//         sleep_ms(1000);
//     }
    
//     return 0;
// }

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
#define MAX_PAYLOAD_SIZE 100

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