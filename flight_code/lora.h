/**
 * lora.h - LoRa module interface
 */
#ifndef LORA_H
#define LORA_H

#include <stdbool.h>
#include <stdint.h>

#define LORA_MAX_PACKET_SIZE 64

// Function prototypes
bool lora_init(void);
void lora_reset(void);
void lora_write_reg(uint8_t reg, uint8_t data);
uint8_t lora_read_reg(uint8_t reg);
bool lora_send_packet(const uint8_t *data, uint8_t len);
void lora_debug_parameters(void);

#endif // LORA_H
