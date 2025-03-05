#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Motor driver I2C address
#define MOTOR_DRIVER_ADDR 0x5D  // Replace with actual I2C address

// Register map for motor control
#define SCMD_MA_DRIVE            0x20  // Master motor A drive
#define SCMD_MB_DRIVE            0x21  // Master motor B drive
#define SCMD_DRIVER_ENABLE       0x70  // Enable motor driver register
#define SCMD_MOTOR_A_INVERT      0x12  // Invert motor A (if needed)
#define SCMD_MOTOR_B_INVERT      0x13  // Invert motor B (if needed)

// Timing control
#define MOTOR_CONTROL_INTERVAL_MS 20  // Control motor every 20ms (50Hz)

void motor_control(i2c_inst_t *i2c, int motor_a_speed, int motor_b_speed) {
    uint8_t motor_a_data[2] = { SCMD_MA_DRIVE, motor_a_speed };
    uint8_t motor_b_data[2] = { SCMD_MB_DRIVE, motor_b_speed };

    // Send motor A speed command
    int res_a = i2c_write_blocking(i2c, MOTOR_DRIVER_ADDR, motor_a_data, sizeof(motor_a_data), false);
    if (res_a != sizeof(motor_a_data)) {
        printf("Error writing to motor A (I2C Write: %d)\n", res_a);
        return;
    }

    // Send motor B speed command
    int res_b = i2c_write_blocking(i2c, MOTOR_DRIVER_ADDR, motor_b_data, sizeof(motor_b_data), false);
    if (res_b != sizeof(motor_b_data)) {
        printf("Error writing to motor B (I2C Write: %d)\n", res_b);
        return;
    }

    printf("Motor A Speed: %d | Motor B Speed: %d\n", motor_a_speed, motor_b_speed);
}

void setup_motor_driver(i2c_inst_t *i2c) {
    // Enable the motor driver
    uint8_t enable_motor_driver[2] = { SCMD_DRIVER_ENABLE, 0x01 };  // 0x01 to enable the driver
    int res = i2c_write_blocking(i2c, MOTOR_DRIVER_ADDR, enable_motor_driver, sizeof(enable_motor_driver), false);
    if (res != sizeof(enable_motor_driver)) {
        printf("Error enabling motor driver (I2C Write: %d)\n", res);
    }

    printf("Motor Driver Enabled\n");

    // Optionally, invert motor directions (if necessary)
    uint8_t invert_motor_a[2] = { SCMD_MOTOR_A_INVERT, 0x01 };  // Set to 0x01 to invert, 0x00 to normal
    uint8_t invert_motor_b[2] = { SCMD_MOTOR_B_INVERT, 0x01 };  // Set to 0x01 to invert, 0x00 to normal

    // Invert motors (if needed)
    int res_a = i2c_write_blocking(i2c, MOTOR_DRIVER_ADDR, invert_motor_a, sizeof(invert_motor_a), false);
    if (res_a != sizeof(invert_motor_a)) {
        printf("Error writing to motor A inversion\n");
    }

    int res_b = i2c_write_blocking(i2c, MOTOR_DRIVER_ADDR, invert_motor_b, sizeof(invert_motor_b), false);
    if (res_b != sizeof(invert_motor_b)) {
        printf("Error writing to motor B inversion\n");
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);  // Give time for terminal initialization

    // Initialize I2C
    i2c_init(i2c_default, 100 * 1000);  // 100 kHz I2C speed
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    printf("Motor Driver Initialized\n");

    // Setup motor driver (enable and invert motors if needed)
    setup_motor_driver(i2c_default);

    // Variables for motor speed (range typically from -100 to 100, where negative is reverse)
    int motor_a_speed = 0;  // Motor A at 0 speed
    int motor_b_speed = 0;  // Motor B at 0 speed

    uint32_t next_motor_control_time = 0;

    // Main control loop
    while (true) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        // Control motors every MOTOR_CONTROL_INTERVAL_MS
        if (current_time >= next_motor_control_time) {
            next_motor_control_time = current_time + MOTOR_CONTROL_INTERVAL_MS;

            // Change motor speeds (can be set to any value you want)
            // For example, ramping up motor speeds for demonstration
            if (motor_a_speed < 100) motor_a_speed += 5;
            if (motor_b_speed < 100) motor_b_speed += 5;

            // Control the motors based on the current speeds
            motor_control(i2c_default, motor_a_speed, motor_b_speed);
        }

        sleep_us(1000);  // Sleep to reduce CPU usage
    }

    return 0;
}
