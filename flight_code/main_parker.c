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

int pico_led_init(void) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
}
void pico_set_led(bool led_on) {
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
}

int main(){
    stdio_init_all();
    sleep_ms(3000);

    pico_led_init();

    
    printf("Init I2C\n");
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    pico_set_led(true);
    printf("Init GNSS\n");
    gnss_init(i2c_default);

    printf("Init SD\n");
    init_sd_card();

    printf("Init LoRa\n");
    lora_init();

    printf("Init Motor\n");
    setup_motor_driver(i2c_default);

    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    uint8_t packet_size = 34;
    uint8_t lora_packet[packet_size];

    create_data_file();

    // while(current_time < GNSS_BEGIN_POLL){
    while(current_time < 15*1000){
        // printf("Polling GNSS\n");
        current_time = to_ms_since_boot(get_absolute_time());
        gnss_read_location(i2c_default, &pvt_data);
        if(gnss_get_fix_type_str(pvt_data.fixType) == "3D"){
            write_data_to_sd(&pvt_data,current_time);
        }
        else{

        }
    }

    printf("Activate Motor\n");
    motor_control(i2c_default, 2, 2);
    sleep_ms(5000);
    // motor_control(i2c_default, 0, 0);
    disable_motor_driver(i2c_default);
    file_read_setup();

    while (current_time < TX_TIME)
    {
        printf("Sending Data via LoRa\n");
        current_time = to_ms_since_boot(get_absolute_time());
        read_data_from_sd(&pvt_data);
        lora_send_packet(&pvt_data, 61); // Total bytes written (34 bytes)
    }
    return 0;
}