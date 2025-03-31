#include "pico/stdlib.h"

#define SIGNAL_PIN 22  // GPIO pin, can change

int main() {
    stdio_init_all();

    gpio_init(SIGNAL_PIN);
    gpio_init(25);
    gpio_set_dir(SIGNAL_PIN, GPIO_OUT);
    gpio_set_dir(25, GPIO_OUT);

    // TODO for main code: conditional to turn beacon on
    while (true) {
        // Send high signal
        gpio_put(SIGNAL_PIN, 1);
        gpio_put(25,1);
        sleep_ms(15000);

        // Set back to low
        gpio_put(SIGNAL_PIN, 0);
        gpio_put(25,0);
        sleep_ms(15000);

    }
    return 0;
}
