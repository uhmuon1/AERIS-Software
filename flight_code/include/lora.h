/**
 * lora.h - LoRa module interface
 */
#ifndef LORA_H
#define LORA_H

#include <stdbool.h>
#include <stdint.h>

#define LORA_MAX_PACKET_SIZE 64

// LoRa configuration
#define CONFIG_LORA_FREQUENCY           433000000  // 433 MHz
#define CONFIG_LORA_TX_INTERVAL_MS      5000       // 5 seconds between transmissions
#define CONFIG_LORA_POWER               0xff       // Maximum power

// Hardware pins - SPI for LoRa
#define CONFIG_PIN_MISO                 4
#define CONFIG_PIN_CS                   1
#define CONFIG_PIN_SCK                  2
#define CONFIG_PIN_MOSI                 3
#define CONFIG_PIN_RST                  17 // Alex said so
#define CONFIG_PIN_TX                   18
#define CONFIG_PIN_RX                   22
#define CONFIG_SPI_PORT                 spi0

// Function prototypes
/*! \brief Initialise LoRa Board to transmit
 *  \ingroup lora
 *
 * Puts the LoRa into a tramsit ready state, and readys it. Must be called before other
 * functions.
 *
 * \note Sets SPI to work at 1 MHz, Refer to implementation and doc for RF parameters
 *
 * \return true if init was successful
 */
bool lora_init(void);

/*! \brief Performs a soft reset of LoRa Board
 *  \ingroup lora
 *
 * Puts high on reset pin then puts low, performing a software
 *
 * \note Reset pin is defined in config.h
 *
 */
void lora_reset(void);

/*! \brief Writes a byte to a register on LoRa Board
 *  \ingroup lora
 *
 * Writes a value to register on the LoRa Board
 *
 * \note This assumes the data is a single byte
 *
 * \param reg register on LoRa board to write to
 * \param data byte of data to write to register
 */
void lora_write_reg(uint8_t reg, uint8_t data);

/*! \brief Reads a byte from a register on LoRa Board
 *  \ingroup lora
 *
 * Reads a value from a register on the LoRa Board
 *
 * \note This assumes the data read is a single byte
 *
 * \param reg register on LoRa board to read from
 * \return byte of data in register
 */
uint8_t lora_read_reg(uint8_t reg);

/*! \brief Transmits a packet of data
 *  \ingroup lora
 *
 * Loads data into LoRa Board and transmits it
 *
 * \note Max packet length is 64 bytes and timeout occurs after 2 seconds
 *
 * \param data array of data to transmit
 * \param len length of data array
 * \return true if tx success
 */
bool lora_send_packet(const uint8_t *data, uint8_t len);

/*! \brief Displays debugging parameters
 *  \ingroup lora
 *
 * Reads spreading factor, bandwidth, coding rate and output power registers.
 *
 * \note Refer to documetation to interpret these values
 */
void lora_debug_parameters(void);

#endif // LORA_H
