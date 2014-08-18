#include <stdint.h>
#include <stdbool.h>       /* For true/false definition */

// Initialization
void SerialInit(uint16_t baud_rate, char volatile ** rxBuffer,
        uint8_t rxBufferSize);

// Writing
void SerialPutCh(char ch);
void SerialPuts(const char *str);
void SerialWriteLine(const char *str);

// Reading
uint8_t SerialDataAvailable();
char SerialGetCh();
void SerialRead(char *buff, uint16_t len);
void SerialFlushBuffer();

/* Interrupt service routine for receiving serial data.
 * 
 * <p>Call this in your ISR.
 *
 * @returns true if the interrupt was handled.
 */
bool SerialRxISR();
