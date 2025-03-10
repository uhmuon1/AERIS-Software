#include "pico/stdlib.h"

#define SIGNAL_PIN 16  // GPIO pin, can change

int main() {
    stdio_init_all();

    gpio_init(SIGNAL_PIN);
    gpio_set_dir(SIGNAL_PIN, GPIO_OUT);

    // TODO for main code: conditional to turn beacon on
    // Send high signal
    gpio_put(SIGNAL_PIN, 1);
    sleep_ms(1000);

    // Set back to low
    gpio_put(SIGNAL_PIN, 0);

    return 0;
}
