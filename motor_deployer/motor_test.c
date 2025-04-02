// #include <stdio.h>
// #include <stdlib.h>
// #include "pico/stdlib.h"

// // Motor control pins
// #define MOTOR_A_PWM_PIN 17  // Motor A PWM control (speed)
// #define MOTOR_A_DIR_PIN 16  // Motor A direction control
// #define MOTOR_EN_PIN 19     // Motor driver enable (EN pin)

// // Stepper motor parameters (Assuming 200 steps per revolution)
// #define STEPS_PER_REVOLUTION 200  // 200 steps for full rotation (common for stepper motors)
// #define DEGREES_TO_ROTATE    90   // Rotate 90 degrees
// #define STEPS_TO_ROTATE      (STEPS_PER_REVOLUTION * DEGREES_TO_ROTATE / 360)  // Steps to rotate 90 degrees

// // Setup the GPIO for motor control
// void setup_gpio() {
//     gpio_init(MOTOR_A_PWM_PIN);
//     gpio_init(MOTOR_A_DIR_PIN);
//     gpio_init(MOTOR_EN_PIN);  // Initialize EN pin
//     gpio_set_dir(MOTOR_A_PWM_PIN, GPIO_OUT);
//     gpio_set_dir(MOTOR_A_DIR_PIN, GPIO_OUT);
//     gpio_set_dir(MOTOR_EN_PIN, GPIO_OUT);  // Set EN pin as output
// }

// // Function to enable the motor driver
// void enable_motor_driver() {
//     gpio_put(MOTOR_EN_PIN, true);  // Set EN pin to HIGH to enable the motor driver
//     printf("Motor driver enabled.\n");
// }

// // Function to disable the motor driver
// void disable_motor_driver() {
//     gpio_put(MOTOR_EN_PIN, false);  // Set EN pin to LOW to disable the motor driver
//     printf("Motor driver disabled.\n");
// }

// // Function to set the motor direction
// void set_motor_direction(bool forward) {
//     gpio_put(MOTOR_A_DIR_PIN, forward);  // Set the direction (0 for reverse, 1 for forward)
// }

// // Function to generate step pulses
// void step_motor(int steps, int delay_ms) {
//     for (int i = 0; i < steps; i++) {
//         gpio_put(MOTOR_A_PWM_PIN, true);  // Step pulse HIGH
//         sleep_ms(1);  // Pulse duration
//         gpio_put(MOTOR_A_PWM_PIN, false); // Step pulse LOW
//         sleep_ms(delay_ms);  // Delay between pulses (speed control)
//     }
// }

// int main() {
//     stdio_init_all();
//     sleep_ms(2000);  // Give time for terminal initialization

//     // Initialize the motor driver and GPIO pins
//     setup_gpio();

//     // Enable the motor driver
//     enable_motor_driver();

//     printf("Motor Driver Initialized\n");

//     // Set motor direction (forward)
//     set_motor_direction(true);

//     // Control loop to rotate the motor 90 degrees (50 steps for a 200-step motor)
//     int steps_to_rotate = STEPS_TO_ROTATE;  // Number of steps for 90 degrees
//     int step_delay_ms = 10;  // Delay between steps (adjust for speed)
    
//     printf("Rotating motor 90 degrees (steps: %d)\n", steps_to_rotate);

//     // Step the motor
//     step_motor(steps_to_rotate, step_delay_ms);

//     // Disable motor driver after rotation
//     disable_motor_driver();

//     printf("Rotation complete!\n");

//     return 0;
// }

/**
 * SparkFun Thing Plus RP2040 - DRV8834 Stepper Motor Control
 * 
 * This program controls a stepper motor using a DRV8834 Low-Voltage
 * Stepper Motor Driver connected to a SparkFun Thing Plus RP2040.
 * 
 * Connections:
 * - DRV8834 A1 to Motor coil A+
 * - DRV8834 A2 to Motor coil A-
 * - DRV8834 B1 to Motor coil B+
 * - DRV8834 B2 to Motor coil B-
 * - DRV8834 STEP to RP2040 GPIO 2
 * - DRV8834 DIR to RP2040 GPIO 3
 * - DRV8834 SLP to RP2040 GPIO 4
 * - DRV8834 GND to RP2040 GND
 * - DRV8834 VCC to appropriate voltage source for your motor
 */

 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/gpio.h"
 
 // Pin Definitions
 #define STEP_PIN    17   // Connect to DRV8834 STEP pin
 #define DIR_PIN     16   // Connect to DRV8834 DIR pin
 #define SLP_PIN     19   // Connect to DRV8834 SLP pin (Sleep, active low)
 
 // Stepper control parameters
 #define STEPS_PER_REV   200     // Standard for 1.8° stepper motor (adjust for your motor)
 #define STEP_DELAY_US   2000    // Delay between steps (controls speed)
 #define CW              1       // Clockwise direction
 #define CCW             0       // Counter-clockwise direction
 
 // Function prototypes
 void init_stepper_pins(void);
 void wake_driver(void);
 void sleep_driver(void);
 void step_motor(uint32_t steps, bool direction);
 void rotate_degrees(float degrees, bool direction);
 
 int main() {
     // Initialize standard I/O
     stdio_init_all();
     
     // Wait a moment for USB to connect (helpful for debugging)
     sleep_ms(1000);
     
     printf("SparkFun Thing Plus RP2040 - DRV8834 Stepper Control\n");
     
     // Initialize stepper motor control pins
     init_stepper_pins();
     
     // Main control loop
     while (true) {
         // Wake up the driver
         wake_driver();
         sleep_ms(5);  // Small delay to ensure driver is ready
         
         // Example movement pattern
         
         // Rotate 360 degrees clockwise
         printf("Rotating 360° clockwise...\n");
         rotate_degrees(360.0, CW);
         sleep_ms(500);
         
         // Rotate 180 degrees counter-clockwise
         printf("Rotating 180° counter-clockwise...\n");
         rotate_degrees(180.0, CCW);
         sleep_ms(500);
         
         // Step by specific step count in each direction
         printf("Stepping 50 steps clockwise...\n");
         step_motor(50, CW);
         sleep_ms(500);
         
         printf("Stepping 50 steps counter-clockwise...\n");
         step_motor(50, CCW);
         sleep_ms(500);
         
         // Put the driver to sleep to save power
         sleep_driver();
         
         // Wait before repeating
         printf("Waiting 3 seconds...\n");
         sleep_ms(3000);
     }
     
     return 0;
 }
 
 /**
  * Initialize GPIO pins for stepper motor control
  */
 void init_stepper_pins(void) {
     // Initialize pins
     gpio_init(STEP_PIN);
     gpio_init(DIR_PIN);
     gpio_init(SLP_PIN);
     
     // Set as outputs
     gpio_set_dir(STEP_PIN, GPIO_OUT);
     gpio_set_dir(DIR_PIN, GPIO_OUT);
     gpio_set_dir(SLP_PIN, GPIO_OUT);
     
     // Set initial state
     gpio_put(STEP_PIN, 0);
     gpio_put(DIR_PIN, 0);
     gpio_put(SLP_PIN, 0);  // Start in sleep mode
     
     printf("Stepper pins initialized\n");
 }
 
 /**
  * Wake up the motor driver
  */
 void wake_driver(void) {
     gpio_put(SLP_PIN, 1);  // Set SLP high to wake up driver
     printf("Driver awake\n");
 }
 
 /**
  * Put the motor driver to sleep
  */
 void sleep_driver(void) {
     gpio_put(SLP_PIN, 0);  // Set SLP low to put driver to sleep
     printf("Driver sleeping\n");
 }
 
 /**
  * Step the motor a specified number of steps in the given direction
  * 
  * @param steps Number of steps to move
  * @param direction CW (1) or CCW (0)
  */
 void step_motor(uint32_t steps, bool direction) {
     // Set direction
     gpio_put(DIR_PIN, direction);
     
     // Execute steps
     for (uint32_t i = 0; i < steps; i++) {
         gpio_put(STEP_PIN, 1);  // Rising edge triggers step
         sleep_us(10);           // Small pulse width
         gpio_put(STEP_PIN, 0);  // Complete the pulse
         sleep_us(STEP_DELAY_US);  // Delay between steps determines speed
     }
 }
 
 /**
  * Rotate the motor by a specified angle in degrees
  * 
  * @param degrees Angle to rotate in degrees
  * @param direction CW (1) or CCW (0)
  */
 void rotate_degrees(float degrees, bool direction) {
     // Calculate steps needed for the requested angle
     uint32_t steps = (uint32_t)((degrees / 360.0) * STEPS_PER_REV);
     
     // Make sure we move at least one step
     if (steps < 1) steps = 1;
     
     // Execute the movement
     step_motor(steps, direction);
 }

