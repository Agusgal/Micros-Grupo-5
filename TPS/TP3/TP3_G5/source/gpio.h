
#ifndef SOURCES_GPIO_H_
#define SOURCES_GPIO_H_

#include <hardware.h>
/*
 * See _BASE_PTRS and _IRQS
 */

#define GPIO_IN(x) 	(0 << x)
#define GPIO_OUT(x) (1 << x)

#define GPIO__IN 	0x00000000u
#define GPIO__OUT   0xFFFFFFFFu

#define GPIO__LO 	0x00000000u
#define GPIO__HI   	0xFFFFFFFFu


typedef GPIO_Type *GPIO_t; 		  /* PTA, PTB, PTC, PTD, PTE */
typedef uint32_t GPIODirection_t; /* Input: 0, Output: 1 */
typedef uint32_t GPIOData_t;
typedef uint32_t GPIOMask_t;
typedef uint8_t pin_t;

enum {PA,PB,PC,PD,PE};
#define PORTNUM2PIN(p,n)    (((p)<<5) + (n))
#define PIN2PORT(p)         (((p)>>5) & 0x07)
#define PIN2NUM(p)          ((p) & 0x1F)

void   			GPIO_Init		  (void);

void   			GPIO_SetDirection (GPIO_t, GPIOMask_t, GPIODirection_t);
GPIODirection_t GPIO_GetDirection (GPIO_t, GPIOMask_t);

void   			GPIO_Write		  (GPIO_t, GPIOMask_t, GPIOData_t);
GPIOData_t 		GPIO_Read		  (GPIO_t, GPIOMask_t);

void   			GPIO_Set		  (GPIO_t, GPIOMask_t);
void   			GPIO_Clear		  (GPIO_t, GPIOMask_t);
void   			GPIO_Toggle		  (GPIO_t, GPIOMask_t);

#endif /* SOURCES_GPIO_H_ */
