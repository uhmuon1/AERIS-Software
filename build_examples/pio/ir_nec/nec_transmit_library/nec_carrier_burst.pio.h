// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ----------------- //
// nec_carrier_burst //
// ----------------- //

#define nec_carrier_burst_wrap_target 0
#define nec_carrier_burst_wrap 4
#define nec_carrier_burst_pio_version 0

#define nec_carrier_burst_TICKS_PER_LOOP 4

static const uint16_t nec_carrier_burst_program_instructions[] = {
            //     .wrap_target
    0xe034, //  0: set    x, 20                      
    0x20c7, //  1: wait   1 irq, 7                   
    0xe001, //  2: set    pins, 1                    
    0xe100, //  3: set    pins, 0                [1] 
    0x0042, //  4: jmp    x--, 2                     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program nec_carrier_burst_program = {
    .instructions = nec_carrier_burst_program_instructions,
    .length = 5,
    .origin = -1,
    .pio_version = nec_carrier_burst_pio_version,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config nec_carrier_burst_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + nec_carrier_burst_wrap_target, offset + nec_carrier_burst_wrap);
    return c;
}

static inline void nec_carrier_burst_program_init(PIO pio, uint sm, uint offset, uint pin, float freq) {
    // Create a new state machine configuration
    //
    pio_sm_config c = nec_carrier_burst_program_get_default_config (offset);
    // Map the SET pin group to one pin, namely the `pin`
    // parameter to this function.
    //
    sm_config_set_set_pins (&c, pin, 1);
    // Set the GPIO function of the pin (connect the PIO to the pad)
    //
    pio_gpio_init (pio, pin);
    // Set the pin direction to output at the PIO
    //
    pio_sm_set_consecutive_pindirs (pio, sm, pin, 1, true);
    // Set the clock divider to generate the required frequency
    //
    float div = clock_get_hz (clk_sys) / (freq * nec_carrier_burst_TICKS_PER_LOOP);
    sm_config_set_clkdiv (&c, div);
    // Apply the configuration to the state machine
    //
    pio_sm_init (pio, sm, offset, &c);
    // Set the state machine running
    //
    pio_sm_set_enabled (pio, sm, true);
}

#endif

