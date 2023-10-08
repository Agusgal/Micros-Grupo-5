################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/CAN_SPI.c \
../source/SPI.c \
../source/Systick.c \
../source/UART.c \
../source/hardware.c \
../source/main.c 

OBJS += \
./source/CAN_SPI.o \
./source/SPI.o \
./source/Systick.o \
./source/UART.o \
./source/hardware.o \
./source/main.o 

C_DEPS += \
./source/CAN_SPI.d \
./source/SPI.d \
./source/Systick.d \
./source/UART.d \
./source/hardware.d \
./source/main.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -D__REDLIB__ -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TP2 beta\UART1\source" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TP2 beta\UART1" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TP2 beta\UART1\startup" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TP2 beta\UART1\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wa,-adhlns="$@.lst" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


