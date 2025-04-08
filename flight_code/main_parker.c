#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "config.h"
#include "gnss.h"
#include "lora.h"
#include "motor.h"
#include "sdCard.h"

#define GNSS_BEGIN_POLL    80*1000
#define TX_TIME   3600*1000

uint32_t start_time = 0;
uint32_t time_limit = 80 * 1000;  // 80 seconds in milliseconds
uint32_t next_sample_time = 0;
uint32_t next_status_time = 0;
ubx_pvt_data_t pvt_data;
bool mode_switched = false;
bool beacon_tx = false;

int main(){

    stdio_init_all();

    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    gnss_init(i2c0);
    init_sd_card();
    lora_init();
    setup_motor_driver(i2c0);

    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    uint16_t packet_size = sizeof(ubx_pvt_data_t);
    uint8_t lora_packet[packet_size];

    while(current_time < GNSS_BEGIN_POLL){
        current_time = to_ms_since_boot(get_absolute_time());
        gnss_read_location(i2c0, &pvt_data);
        write_data_to_sd(&pvt_data,current_time);
    }

    motor_control(i2c0, 100, 100);
    sleep_ms(500);
    motor_control(i2c0, 0, 0);
    reset_f_ptr();

    while (current_time < TX_TIME)
    {
        current_time = to_ms_since_boot(get_absolute_time());
        read_data_from_sd(lora_packet);
        lora_send_packet(lora_packet, packet_size);
    }
    

    // Check alt if needed
    // gnss_read_location(i2c0, &pvt_data);
    // pvt_data.hMSL;
}