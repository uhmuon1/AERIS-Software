/**
 * motor.c - Implementation of deployment motors
 */

 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "motor.h"
#include "config.h"

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

void disable_motor_driver(i2c_inst_t *i2c) {
    // Enable the motor driver
    uint8_t enable_motor_driver[2] = { SCMD_DRIVER_ENABLE, 0x00 };  // 0x00 to disable the driver
    int res = i2c_write_blocking(i2c, MOTOR_DRIVER_ADDR, enable_motor_driver, sizeof(enable_motor_driver), false);
    if (res != sizeof(enable_motor_driver)) {
        printf("Error enabling motor driver (I2C Write: %d)\n", res);
    }

    printf("Motor Driver Disable\n");
}