#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "config.h"
#include "gnss.h"
#include "logging.h"
#include "lora.h"
#include "motor.h"

// Define operation modes
#define MODE_MOTOR_TEST 1
#define MODE_LORA_TX    2
#define MODE_GNSS_LOG   3


int main(){
    switch ()

void get_data() {
    stdio_init_all();
    
    // Important: Give enough time for UART to initialize
    sleep_ms(5000);
    
    // Initialize I2C for GNSS module
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    
    printf("I2C Initialized\n");

    pico_led_init();

    // Initialize SD card
    if (!init_sd_card()) {
        printf("SD card failed\n");
        while (1) tight_loop_contents();
    }
    
    // Variables for timing
    gnss_data_t pvt_data;
    uint32_t next_sample_time = 0;
    uint32_t next_status_time = 0;
    
    // Set the recording time limit (80 seconds)
    uint32_t start_time = 0;
    uint32_t time_limit = 80 * 1000;  // 80 seconds in milliseconds
    bool time_limit_started = false;  // Flag to indicate if the 3D fix has been obtained

    printf("Logging started\n");
    
    while (true) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        
        // Collect a sample
        if (current_time >= next_sample_time) {
            next_sample_time = current_time + LOG_INTERVAL_MS;
            // blink_freq = 1000;
            if (read_ubx_message(i2c_default, &pvt_data)) {
                // Only log if we have a valid position
                if (pvt_data.fix_type >= 3) {  // 3D fix is fixType 3 or higher
                    if (!time_limit_started) {
                        // Start the timer once we get the first 3D fix
                        
                        time_limit_started = true;
                        start_time = current_time;  // Start counting time from here
                        // Create the file with a date-time based filename
                        if (!create_data_file(&pvt_data)) {
                            printf("File creation failed\n");
                            while (1) tight_loop_contents();
                        }
                    }
                    write_data_to_sd(&pvt_data, current_time);
                }
                
                // Display status update (throttled to reduce serial traffic)
                if (current_time >= next_status_time) {
                    print_status_line(&pvt_data);
                    next_status_time = current_time + STATUS_INTERVAL_MS;
                }
            }
        }
        
        // Check if we've exceeded the time limit after the first 3D fix
        if (time_limit_started && current_time - start_time >= time_limit) {
            printf("Time limit reached, stopping logging...\n");
            break;  // Exit the loop after 80 seconds
        }

        // Small sleep to prevent tight loop when waiting for next sample time
        sleep_us(500);
    }
    
    // Close the data file and unmount the SD card
    f_close(&data_file);
    f_unmount("0:");
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

    uint32_t current_time, motor_start_time = 0;
    bool motors_running = false;

    // Main control loop
    while (true) {
        current_time = to_ms_since_boot(get_absolute_time());

        // Start motors after 1 second of running
        if (!motors_running) {
            motor_a_speed = 100;  // Set motor speed to maximum (or whatever speed you desire)
            motor_b_speed = 100;
            motor_control(i2c_default, motor_a_speed, motor_b_speed);
            motor_start_time = current_time;  // Record when the motors started
            motors_running = true;
        }

        // Stop motors after 1 second
        if (motors_running && current_time >= motor_start_time + MOTOR_RUN_TIME_MS) {
            motor_a_speed = 0;  // Stop motor A
            motor_b_speed = 0;  // Stop motor B
            motor_control(i2c_default, motor_a_speed, motor_b_speed);
            motors_running = false;  // Motors stopped

            // Wait for 2 seconds before the next cycle (adjust as needed)
            sleep_ms(MOTOR_STOP_TIME_MS);
        }

        sleep_us(1000);  // Sleep to reduce CPU usage
    }

    return 0;
}