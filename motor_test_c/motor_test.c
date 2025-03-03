/**
 * SparkFun Serial Controlled Motor Driver (SCMD) Test for RP2040
 * 
 * This code tests a SparkFun SCMD connected to a
 * SparkFun Thing Plus RP2040 using the built-in Qwiic/I2C connector.
 * 
 * Connection:
 * - Connect the SCMD to the Thing Plus RP2040 using a Qwiic cable
 * - Power the SCMD separately as required
 * - Connect motors to the driver's output terminals
 * 
 * Enhanced with extensive debugging outputs
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/i2c.h"
 
 // Default I2C address for the SCMD
 #define SCMD_ADDR 0x5D
 
 // Register map for SCMD
 #define SCMD_FID                 0x00  // Factory ID
 #define SCMD_ID                  0x01  // Product ID
 #define SCMD_SLAVE_ADDR          0x02  // I2C slave address
 #define SCMD_CONFIG_BITS         0x03  // Configuration control
 #define SCMD_U_I2C_RD_ERR        0x04  // User I2C read errors
 #define SCMD_U_I2C_WR_ERR        0x05  // User I2C write errors
 #define SCMD_U_BUF_DUMPED        0x06  // User buffer dump count
 #define SCMD_E_I2C_RD_ERR        0x07  // Expansion I2C read errors
 #define SCMD_E_I2C_WR_ERR        0x08  // Expansion I2C write errors
 #define SCMD_LOOP_TIME           0x09  // Loop time
 #define SCMD_SLV_POLL_CNT        0x0A  // Slave poll count
 #define SCMD_SLV_TOP_ADDR        0x0B  // Slave top address
 #define SCMD_MST_E_ERR           0x0C  // Master expansion port errors
 #define SCMD_MST_E_STATUS        0x0D  // Master expansion port status
 #define SCMD_FSAFE_FAULTS        0x0E  // Fail-safe fault counter
 #define SCMD_REG_OOR_CNT         0x0F  // Out of range register counter
 #define SCMD_REG_RO_WRITE_CNT    0x10  // Read-only register write counter
 #define SCMD_GEN_TEST_WORD       0x11  // General test word register
 #define SCMD_MOTOR_A_INVERT      0x12  // Invert motor A
 #define SCMD_MOTOR_B_INVERT      0x13  // Invert motor B
 #define SCMD_BRIDGE              0x14  // Bridge control
 #define SCMD_LOCAL_MASTER_LOCK   0x15  // Local master lock
 #define SCMD_LOCAL_USER_LOCK     0x16  // Local user lock
 #define SCMD_MST_E_IN_FN         0x17  // Master expansion input function
 #define SCMD_U_PORT_CLKDIV_U     0x18  // User port clock divider upper
 #define SCMD_U_PORT_CLKDIV_L     0x19  // User port clock divider lower
 #define SCMD_U_PORT_CLKDIV_CTRL  0x1A  // User port clock divider control
 #define SCMD_E_PORT_CLKDIV_U     0x1B  // Expansion port clock divider upper
 #define SCMD_E_PORT_CLKDIV_L     0x1C  // Expansion port clock divider lower
 #define SCMD_E_PORT_CLKDIV_CTRL  0x1D  // Expansion port clock divider control
 #define SCMD_U_BUS_UART_BAUD     0x1E  // User bus UART baud rate
 #define SCMD_FSAFE_CTRL          0x1F  // Fail-safe control
 #define SCMD_MA_DRIVE            0x20  // Master motor A drive
 #define SCMD_MB_DRIVE            0x21  // Master motor B drive
 
 // Bridge control values
 #define BRIDGE_ENABLE            0x01  // Enable the H-bridge outputs
 #define BRIDGE_DISABLE           0x00  // Disable the H-bridge outputs
 
 // I2C instance - RP2040 has two I2C controllers
 // Default Qwiic connector on Thing Plus RP2040 uses I2C0
 #define I2C_INST i2c_default
 
 // GPIO pins for I2C on the Thing Plus RP2040 Qwiic connector
 #define I2C_SDA_PIN 6
 #define I2C_SCL_PIN 7
 
 // I2C frequency
 #define I2C_FREQ 100000 // 100 kHz
 
 // Debug level (higher = more verbose)
 #define DEBUG_LEVEL 2 // 0: none, 1: basic, 2: detailed, 3: verbose
 
 // Function prototypes
 bool write_register(uint8_t reg, uint8_t value);
 bool read_register(uint8_t reg, uint8_t *value);
 void set_motor_speed(char motor, int8_t speed);
 void setup_motor_driver();
 void print_motor_driver_info();
 void check_and_print_errors();
 bool check_device_present();
 
 // Initialize I2C and setup motor driver
 void setup() {
     stdio_init_all();
     
     printf("\n\n========================================\n");
     printf("DEBUG: Starting initialization sequence\n");
     printf("DEBUG: Timestamp: %llu ms\n", time_us_64() / 1000);
     
     // Initialize I2C
     printf("DEBUG: Initializing I2C%d at %d Hz\n", I2C_INST == i2c0 ? 0 : 1, I2C_FREQ);
     printf("DEBUG: SDA pin: %d, SCL pin: %d\n", I2C_SDA_PIN, I2C_SCL_PIN);
     
     i2c_init(I2C_INST, I2C_FREQ);
     
     printf("DEBUG: Setting GPIO pin functions for I2C\n");
     gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
     gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
     
     printf("DEBUG: Enabling pull-up resistors on I2C pins\n");
     gpio_pull_up(I2C_SDA_PIN);
     gpio_pull_up(I2C_SCL_PIN);
     
     // Wait for serial monitor to be opened
     printf("DEBUG: Waiting 3 seconds for serial monitor\n");
     for (int i = 3; i > 0; i--) {
         printf("DEBUG: Starting in %d...\n", i);
         sleep_ms(1000);
     }
     
     printf("\n========================================\n");
     printf("SparkFun SCMD Motor Driver Test\n");
     printf("- Using I2C%d at %d Hz\n", I2C_INST == i2c0 ? 0 : 1, I2C_FREQ);
     printf("- SCMD address: 0x%02X\n", SCMD_ADDR);
     printf("========================================\n\n");
     
     // Check if device is present
     if (!check_device_present()) {
         printf("ERROR: SCMD not detected! Check connections and address.\n");
         while (1) {
             printf("ERROR: Device not found. Retrying in 5 seconds...\n");
             sleep_ms(5000);
             if (check_device_present()) {
                 printf("SUCCESS: SCMD found! Continuing setup.\n");
                 break;
             }
         }
     }
     
     // Setup motor driver
     printf("DEBUG: Setting up motor driver\n");
     setup_motor_driver();
     
     // Print motor driver info
     printf("DEBUG: Retrieving motor driver information\n");
     print_motor_driver_info();
 }
 
 // Main loop
 int main() {
     setup();
     
     printf("\n========================================\n");
     printf("Starting motor test sequence...\n");
     printf("========================================\n\n");
     
     while (1) {
         printf("\nDEBUG: Beginning new test cycle at %llu ms\n", time_us_64() / 1000);
         
         // Forward at 50%
         printf("\n----- Test: Motor A Forward at 50%% -----\n");
         printf("DEBUG: Setting motor A to 50%% forward\n");
         set_motor_speed('A', 50);
         printf("DEBUG: Motor A should now be running forward at 50%%\n");
         check_and_print_errors();
         
         printf("DEBUG: Waiting 3000 ms\n");
         for (int i = 0; i < 3; i++) {
             printf("DEBUG: Forward running: %d seconds elapsed\n", i+1);
             sleep_ms(1000);
         }
         
         // Stop
         printf("\n----- Test: Motor A Stop -----\n");
         printf("DEBUG: Stopping motor A\n");
         set_motor_speed('A', 0);
         printf("DEBUG: Motor A should now be stopped\n");
         check_and_print_errors();
         
         printf("DEBUG: Waiting 1000 ms\n");
         sleep_ms(1000);
         
         // Backward at 50%
         printf("\n----- Test: Motor A Backward at 50%% -----\n");
         printf("DEBUG: Setting motor A to 50%% backward\n");
         set_motor_speed('A', -50);
         printf("DEBUG: Motor A should now be running backward at 50%%\n");
         check_and_print_errors();
         
         printf("DEBUG: Waiting 3000 ms\n");
         for (int i = 0; i < 3; i++) {
             printf("DEBUG: Backward running: %d seconds elapsed\n", i+1);
             sleep_ms(1000);
         }
         
         // Stop
         printf("\n----- Test: Motor A Stop -----\n");
         printf("DEBUG: Stopping motor A\n");
         set_motor_speed('A', 0);
         printf("DEBUG: Motor A should now be stopped\n");
         check_and_print_errors();
         
         printf("DEBUG: Waiting 1000 ms\n");
         sleep_ms(1000);
         
         // Test Motor B
         printf("\n----- Test: Motor B Forward at 50%% -----\n");
         printf("DEBUG: Setting motor B to 50%% forward\n");
         set_motor_speed('B', 50);
         printf("DEBUG: Motor B should now be running forward at 50%%\n");
         check_and_print_errors();
         
         printf("DEBUG: Waiting 3000 ms\n");
         for (int i = 0; i < 3; i++) {
             printf("DEBUG: Forward running: %d seconds elapsed\n", i+1);
             sleep_ms(1000);
         }
         
         // Stop
         printf("\n----- Test: Motor B Stop -----\n");
         printf("DEBUG: Stopping motor B\n");
         set_motor_speed('B', 0);
         printf("DEBUG: Motor B should now be stopped\n");
         check_and_print_errors();
         
         printf("DEBUG: Waiting 1000 ms\n");
         sleep_ms(1000);
         
         // Run both motors
         printf("\n----- Test: Both Motors Forward -----\n");
         printf("DEBUG: Setting both motors to 30%% forward\n");
         set_motor_speed('A', 30);
         set_motor_speed('B', 30);
         printf("DEBUG: Both motors should now be running forward\n");
         check_and_print_errors();
         
         printf("DEBUG: Waiting 3000 ms\n");
         for (int i = 0; i < 3; i++) {
             printf("DEBUG: Forward running: %d seconds elapsed\n", i+1);
             sleep_ms(1000);
         }
         
         // Ramp up motor A from 0 to 100%
         printf("\n----- Test: Ramp Motor A 0%% to 100%% -----\n");
         for (int i = 0; i <= 100; i += 10) {
             printf("DEBUG: Setting motor A speed to %d%%\n", i);
             set_motor_speed('A', i);
             if (i % 50 == 0) {
                 check_and_print_errors();
             }
             printf("DEBUG: Waiting 200 ms\n");
             sleep_ms(200);
         }
         
         // Ramp down motor A from 100% to 0
         printf("\n----- Test: Ramp Motor A 100%% to 0%% -----\n");
         for (int i = 100; i >= 0; i -= 10) {
             printf("DEBUG: Setting motor A speed to %d%%\n", i);
             set_motor_speed('A', i);
             if (i % 50 == 0) {
                 check_and_print_errors();
             }
             printf("DEBUG: Waiting 200 ms\n");
             sleep_ms(200);
         }
         
         // Stop all motors
         printf("\n----- Test: Final Stop -----\n");
         printf("DEBUG: Stopping all motors\n");
         set_motor_speed('A', 0);
         set_motor_speed('B', 0);
         printf("DEBUG: Test cycle complete, all motors should be stopped\n");
         
         // Check for any errors
         printf("\n----- Final Error Check -----\n");
         check_and_print_errors();
         
         printf("\nDEBUG: Test cycle completed at %llu ms\n", time_us_64() / 1000);
         printf("DEBUG: Waiting 5000 ms before starting next cycle\n");
         printf("========================================\n");
         for (int i = 0; i < 5; i++) {
             printf("DEBUG: Next cycle in %d seconds\n", 5-i);
             sleep_ms(1000);
         }
     }
 
     return 0;
 }
 
 // Check if device is present on I2C bus
 bool check_device_present() {
     uint8_t check_value;
     bool success = read_register(SCMD_ID, &check_value);
     
     if (success) {
         printf("DEBUG: Device check successful. ID: 0x%02X\n", check_value);
         if (check_value == 0xA9) {  // Expected ID for SCMD
             printf("DEBUG: Confirmed SCMD device (ID: 0xA9)\n");
         } else {
             printf("WARNING: Device ID doesn't match expected SCMD ID. Found: 0x%02X, Expected: 0xA9\n", check_value);
         }
     }
     
     return success;
 }
 
 // Check for errors and print detailed information
 void check_and_print_errors() {
     uint8_t readErr, writeErr, bufDump, expReadErr, expWriteErr, fsafeFaults;
     
     printf("DEBUG: Checking for errors\n");
     
     if (read_register(SCMD_U_I2C_RD_ERR, &readErr) && readErr > 0) {
         printf("ERROR: User I2C read errors: %d\n", readErr);
     }
     
     if (read_register(SCMD_U_I2C_WR_ERR, &writeErr) && writeErr > 0) {
         printf("ERROR: User I2C write errors: %d\n", writeErr);
     }
     
     if (read_register(SCMD_U_BUF_DUMPED, &bufDump) && bufDump > 0) {
         printf("ERROR: User buffer dump count: %d\n", bufDump);
     }
     
     if (read_register(SCMD_E_I2C_RD_ERR, &expReadErr) && expReadErr > 0) {
         printf("ERROR: Expansion I2C read errors: %d\n", expReadErr);
     }
     
     if (read_register(SCMD_E_I2C_WR_ERR, &expWriteErr) && expWriteErr > 0) {
         printf("ERROR: Expansion I2C write errors: %d\n", expWriteErr);
     }
     
     if (read_register(SCMD_FSAFE_FAULTS, &fsafeFaults) && fsafeFaults > 0) {
         printf("ERROR: Fail-safe fault count: %d\n", fsafeFaults);
     }
     
     if (readErr == 0 && writeErr == 0 && bufDump == 0 && 
         expReadErr == 0 && expWriteErr == 0 && fsafeFaults == 0) {
         printf("DEBUG: No errors detected\n");
     }
 }
 
 // Set up the motor driver
 void setup_motor_driver() {
     bool success;
     
     // Enable the bridge
     printf("DEBUG: Enabling motor bridge (reg 0x%02X)\n", SCMD_BRIDGE);
     success = write_register(SCMD_BRIDGE, BRIDGE_ENABLE);
     if (success) {
         printf("DEBUG: Bridge enabled successfully\n");
     } else {
         printf("ERROR: Failed to enable bridge\n");
     }
     
     // Set motor direction to default (not inverted)
     printf("DEBUG: Setting motor A direction to default (reg 0x%02X)\n", SCMD_MOTOR_A_INVERT);
     success = write_register(SCMD_MOTOR_A_INVERT, 0x00);
     if (!success) printf("ERROR: Failed to set motor A direction\n");
     
     printf("DEBUG: Setting motor B direction to default (reg 0x%02X)\n", SCMD_MOTOR_B_INVERT);
     success = write_register(SCMD_MOTOR_B_INVERT, 0x00);
     if (!success) printf("ERROR: Failed to set motor B direction\n");
     
     // Set both motors to stop
     printf("DEBUG: Setting initial motor speeds to 0\n");
     set_motor_speed('A', 0);
     set_motor_speed('B', 0);
 }
 
 // Print motor driver information
 void print_motor_driver_info() {
     uint8_t id, fid, config, bridge;
     bool success;
     
     printf("\n----- Motor Driver Information -----\n");
     
     printf("DEBUG: Reading factory ID (reg 0x%02X)\n", SCMD_FID);
     success = read_register(SCMD_FID, &fid);
     if (success) {
         printf("Factory ID: 0x%02X\n", fid);
     } else {
         printf("ERROR: Failed to read factory ID\n");
     }
     
     printf("DEBUG: Reading product ID (reg 0x%02X)\n", SCMD_ID);
     success = read_register(SCMD_ID, &id);
     if (success) {
         printf("Product ID: 0x%02X\n", id);
         if (id == 0xA9) {
             printf("  Confirmed as SparkFun SCMD\n");
         } else {
             printf("  WARNING: ID doesn't match SCMD expected value\n");
         }
     } else {
         printf("ERROR: Failed to read product ID\n");
     }
     
     printf("DEBUG: Reading configuration (reg 0x%02X)\n", SCMD_CONFIG_BITS);
     success = read_register(SCMD_CONFIG_BITS, &config);
     if (success) {
         printf("Configuration Bits: 0x%02X\n", config);
     } else {
         printf("ERROR: Failed to read configuration\n");
     }
     
     printf("DEBUG: Reading bridge status (reg 0x%02X)\n", SCMD_BRIDGE);
     success = read_register(SCMD_BRIDGE, &bridge);
     if (success) {
         printf("Bridge Status: 0x%02X (%s)\n", bridge, 
                bridge & BRIDGE_ENABLE ? "ENABLED" : "DISABLED");
     } else {
         printf("ERROR: Failed to read bridge status\n");
     }
     
     // Check for errors
     printf("\n----- Error Status -----\n");
     check_and_print_errors();
     
     printf("----- End of Driver Information -----\n\n");
 }
 
 // Set motor speed (-100 to 100)
 void set_motor_speed(char motor, int8_t speed) {
     bool success;
     uint8_t motorReg;
     
     // Determine which motor register to use
     if (motor == 'A' || motor == 'a') {
         motorReg = SCMD_MA_DRIVE;
     } else if (motor == 'B' || motor == 'b') {
         motorReg = SCMD_MB_DRIVE;
     } else {
         printf("ERROR: Invalid motor selection '%c'. Use 'A' or 'B'.\n", motor);
         return;
     }
     
     // Constrain speed to -100 to 100
     int8_t original_speed = speed;
     if (speed > 100) speed = 100;
     if (speed < -100) speed = -100;
     
     if (original_speed != speed) {
         printf("DEBUG: Speed constrained from %d to %d\n", original_speed, speed);
     }
     
     // Convert -100 to 100 range to 0 to 254 range that SCMD uses
     // SCMD uses: 0 = full reverse, 127 = stop, 254 = full forward
     uint8_t scmd_speed;
     
     if (speed == 0) {
         scmd_speed = 127;  // Stop
     } else if (speed > 0) {
         // Map 1-100 to 128-254
         scmd_speed = 127 + (uint8_t)((speed * 127) / 100);
     } else {
         // Map -100 to -1 to 0 to 126
         scmd_speed = 127 + (uint8_t)((speed * 127) / 100);
     }
     
     printf("DEBUG: Setting motor %c speed to %d%% (SCMD value: %d)\n", 
            motor, speed, scmd_speed);
     
     // Write speed to the appropriate register
     success = write_register(motorReg, scmd_speed);
     
     if (success) {
         printf("DEBUG: Motor %c speed set successfully\n", motor);
     } else {
         printf("ERROR: Failed to set motor %c speed\n", motor);
     }
     
     // Verify by reading back
     if (DEBUG_LEVEL >= 2) {
         uint8_t readback;
         printf("DEBUG: Verifying motor %c speed setting\n", motor);
         
         if (read_register(motorReg, &readback)) {
             printf("DEBUG: Motor %c register readback: %d\n", motor, readback);
             if (readback != scmd_speed) {
                 printf("WARNING: Motor %c speed verification failed. Expected: %d, Got: %d\n", 
                        motor, scmd_speed, readback);
             }
         } else {
             printf("ERROR: Failed to read back motor %c speed\n", motor);
         }
     }
 }
 
 // Write a value to a register
 bool write_register(uint8_t reg, uint8_t value) {
     if (DEBUG_LEVEL >= 3) {
         printf("DEBUG: Writing 0x%02X to register 0x%02X\n", value, reg);
     }
     
     uint8_t buffer[2] = {reg, value};
     int result = i2c_write_blocking(I2C_INST, SCMD_ADDR, buffer, 2, false);
     
     if (DEBUG_LEVEL >= 3) {
         if (result == 2) {
             printf("DEBUG: Write successful\n");
         } else {
             printf("ERROR: Write failed, result: %d\n", result);
         }
     }
     
     return (result == 2);
 }
 
 // Read a value from a register
 bool read_register(uint8_t reg, uint8_t *value) {
     if (DEBUG_LEVEL >= 3) {
         printf("DEBUG: Reading from register 0x%02X\n", reg);
     }
     
     int write_result = i2c_write_blocking(I2C_INST, SCMD_ADDR, &reg, 1, true); // true to keep master control of bus
     if (write_result != 1) {
         if (DEBUG_LEVEL >= 3) {
             printf("ERROR: Read failed at write phase, result: %d\n", write_result);
         }
         return false;
     }
     
     int read_result = i2c_read_blocking(I2C_INST, SCMD_ADDR, value, 1, false);
     
     if (DEBUG_LEVEL >= 3) {
         if (read_result == 1) {
             printf("DEBUG: Read successful, value: 0x%02X\n", *value);
         } else {
             printf("ERROR: Read failed at read phase, result: %d\n", read_result);
         }
     }
     
     return (read_result == 1);
 }