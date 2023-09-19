
#ifndef UART_H_
#define UART_H_


void UART_Init (void);
void UART_SetBaudRate (UART_Type *uart, uint32_t baudrate);
unsigned char UART_Get_Status(uint8_t id);
unsigned char UART_Get_Data(uint8_t id);
void UART_Send_Data(unsigned char txdata);
void UART_SendMsg(char* msg, uint8_t id);
void UART_SendChar(char msg, uint8_t id);




#endif /* UART_H_ */
