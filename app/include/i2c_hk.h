/*******************************************************************************************************/
/**						BIblioteca de funcoes de acesso ao display de LCD	e outros perif�ricos I2C							**/
/**														Autorizado o uso publico, sem modifica��es															**/
/**															Desenvolvida por : Prof. Hegler Rocha																	**/
/**																				PUC Minas - 05/2019																					**/
/**																				Arquivo i2c_hk.c	    																			**/
/**																				 Vers�o : v1.0.0																						**/
/**															Problemas : contactar hk4@pucminas.br																	**/
/**																																																		**/
/*******************************************************************************************************/

#ifndef _I2C_H
#define _I2C_H
#include <inttypes.h>

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef HIGH
#define HIGH 1
#endif
#ifndef LOW
#define LOW 0
#endif

#ifndef TWI_FREQ
#define TWI_FREQ 100000L
#endif

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif

#define TWI_READY 0
#define TWI_MRX 1
#define TWI_MTX 2
#define TWI_SRX 3
#define TWI_STX 4

#define TW_START 0x08
#define TW_REP_START 0x10
#define TW_MT_SLA_ACK 0x18
#define TW_MT_SLA_NACK 0x20
#define TW_MT_DATA_ACK 0x28
#define TW_MT_DATA_NACK 0x30
#define TW_MT_ARB_LOST 0x38
#define TW_MR_ARB_LOST 0x38
#define TW_MR_SLA_ACK 0x40
#define TW_MR_SLA_NACK 0x48
#define TW_MR_DATA_ACK 0x50
#define TW_MR_DATA_NACK 0x58
#define TW_ST_SLA_ACK 0xA8
#define TW_ST_ARB_LOST_SLA_ACK 0xB0
#define TW_ST_DATA_ACK 0xB8
#define TW_ST_DATA_NACK 0xC0
#define TW_ST_LAST_DATA 0xC8
#define TW_SR_SLA_ACK 0x60
#define TW_SR_ARB_LOST_SLA_ACK 0x68
#define TW_SR_GCALL_ACK 0x70
#define TW_SR_ARB_LOST_GCALL_ACK 0x78
#define TW_SR_DATA_ACK 0x80
#define TW_SR_DATA_NACK 0x88
#define TW_SR_GCALL_DATA_ACK 0x90
#define TW_SR_GCALL_DATA_NACK 0x98
#define TW_SR_STOP 0xA0
#define TW_NO_INFO 0xF8
#define TW_BUS_ERROR 0x00
#define TW_READ 1
#define TW_WRITE 0
#define TW_STATUS (TWSR & (_BV(TWS7) | _BV(TWS6) | _BV(TWS5) | _BV(TWS4) | _BV(TWS3)))
#define BUFFER_LENGTH 32

uint8_t rxBuffer[32];
uint8_t rxBufferIndex;
uint8_t rxBufferLength;
uint8_t txAddress;
uint8_t txBuffer[32];
uint8_t txBufferIndex;
uint8_t txBufferLength;
uint8_t transmitting;

uint8_t write(uint8_t);
int read(void);
uint8_t endTransmission(void);
void beginTransmission(uint8_t);
void twi_init(void);
uint8_t twi_writeTo(uint8_t, uint8_t *, uint8_t);
void twi_stop(void);

#endif