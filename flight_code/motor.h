/**
 * motor.h - Motor Driver
 */

#ifndef GNSS_H
#define GNSS_H

#include <stdbool.h>
#include <stdint.h>
#include "hardware/i2c.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"

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

void motor_control(i2c_inst_t *i2c, int motor_a_speed, int motor_b_speed);
void setup_motor_driver(i2c_inst_t *i2c);

#endif // GNSS_H