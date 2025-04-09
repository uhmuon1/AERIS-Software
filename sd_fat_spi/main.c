#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ff.h"
#include "f_util.h"
#include "hw_config.h"

static FATFS fs;
static FIL fil;

int main() {
    stdio_init_all();
    sleep_ms(1000);  // Wait for UART to initialize
    printf("SD Card Test Program\n");

    // Initialize SD card
    FRESULT fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK) {
        printf("Failed to mount SD card: %d\n", fr);
        return -1;
    }
    printf("SD card mounted successfully\n");

    // Create and write to a test file
    fr = f_open(&fil, "0:/test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        printf("Failed to open file: %d\n", fr);
        return -1;
    }

    const char *message = "Hello from RP2040!\n";
    UINT bw;
    fr = f_write(&fil, message, strlen(message), &bw);
    if (fr != FR_OK) {
        printf("Failed to write to file: %d\n", fr);
        f_close(&fil);
        return -1;
    }
    printf("Wrote %d bytes to file\n", bw);

    // Close the file
    f_close(&fil);
    
    // Read back the file to verify
    fr = f_open(&fil, "0:/test.txt", FA_READ);
    if (fr != FR_OK) {
            printf("Failed to open file for reading: %d\n", fr);
            return -1;
        }
        
    char buffer[64];
    UINT br;
    fr = f_read(&fil, buffer, sizeof(buffer) - 1, &br);
    if (fr != FR_OK) {
        printf("Failed to read file: %d\n", fr);
        f_close(&fil);
        return -1;
    }
    buffer[br] = '\0';
    printf("Read back: %s\n", buffer);

    // Close file and unmount
    f_close(&fil);
    f_unmount("0:");

    printf("Test complete!\n");

    while(1) {
        sleep_ms(1000);
    }
    return 0;
}