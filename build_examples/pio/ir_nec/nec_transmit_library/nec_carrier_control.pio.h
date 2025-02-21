// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------------------- //
// nec_carrier_control //
// ------------------- //

#define nec_carrier_control_wrap_target 0
#define nec_carrier_control_wrap 10
#define nec_carrier_control_pio_version 0

static const uint16_t nec_carrier_control_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block                      
    0xe02f, //  1: set    x, 15                      
    0xc007, //  2: irq    nowait 7                   
    0x0042, //  3: jmp    x--, 2                     
    0xaf42, //  4: nop                           [15]
    0xc107, //  5: irq    nowait 7               [1] 
    0x6021, //  6: out    x, 1                       
    0x0029, //  7: jmp    !x, 9                      
    0xa342, //  8: nop                           [3] 
    0xc007, //  9: irq    nowait 7                   
    0x00e6, // 10: jmp    !osre, 6                   
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program nec_carrier_control_program = {
    .instructions = nec_carrier_control_program_instructions,
    .length = 11,
    .origin = -1,
    .pio_version = nec_carrier_control_pio_version,
#if PICO_PIO_VERSION > 0
    .used_gpio_ranges = 0x0
#endif
};

static inline pio_sm_config nec_carrier_control_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + nec_carrier_control_wrap_target, offset + nec_carrier_control_wrap);
    return c;
}

static inline void nec_carrier_control_program_init (PIO pio, uint sm, uint offset, float tick_rate, int bits_per_frame) {
    // create a new state machine configuration
    //
    pio_sm_config c = nec_carrier_control_program_get_default_config(offset);
    // configure the output shift register
    //
    sm_config_set_out_shift (&c,
                             true,       // shift right
                             false,      // disable autopull
                             bits_per_frame);
    // join the FIFOs to make a single large transmit FIFO
    //
    sm_config_set_fifo_join (&c, PIO_FIFO_JOIN_TX);
    // configure the clock divider
    //
    float div = clock_get_hz (clk_sys) / tick_rate;
    sm_config_set_clkdiv (&c, div);
    // apply the configuration to the state machine
    //
    pio_sm_init(pio, sm, offset, &c);
    // set the state machine running
    //
    pio_sm_set_enabled(pio, sm, true);
}

#endif

