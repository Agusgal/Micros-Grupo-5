################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Drivers/BoardLeds.c \
../source/Drivers/CardReader_DRV.c \
../source/Drivers/DataTransmissionDRV.c \
../source/Drivers/Display.c \
../source/Drivers/Encoder.c \
../source/Drivers/PDRV_UART.c \
../source/Drivers/SysTick.c \
../source/Drivers/Timers.c \
../source/Drivers/gpio.c 

C_DEPS += \
./source/Drivers/BoardLeds.d \
./source/Drivers/CardReader_DRV.d \
./source/Drivers/DataTransmissionDRV.d \
./source/Drivers/Display.d \
./source/Drivers/Encoder.d \
./source/Drivers/PDRV_UART.d \
./source/Drivers/SysTick.d \
./source/Drivers/Timers.d \
./source/Drivers/gpio.d 

OBJS += \
./source/Drivers/BoardLeds.o \
./source/Drivers/CardReader_DRV.o \
./source/Drivers/DataTransmissionDRV.o \
./source/Drivers/Display.o \
./source/Drivers/Encoder.o \
./source/Drivers/PDRV_UART.o \
./source/Drivers/SysTick.o \
./source/Drivers/Timers.o \
./source/Drivers/gpio.o 


# Each subdirectory must supply rules for building sources it contributes
source/Drivers/%.o: ../source/Drivers/%.c source/Drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/ucosiii_config" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uC-CPU" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uC-LIB" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uCOS-III/Source" -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-Drivers

clean-source-2f-Drivers:
	-$(RM) ./source/Drivers/BoardLeds.d ./source/Drivers/BoardLeds.o ./source/Drivers/CardReader_DRV.d ./source/Drivers/CardReader_DRV.o ./source/Drivers/DataTransmissionDRV.d ./source/Drivers/DataTransmissionDRV.o ./source/Drivers/Display.d ./source/Drivers/Display.o ./source/Drivers/Encoder.d ./source/Drivers/Encoder.o ./source/Drivers/PDRV_UART.d ./source/Drivers/PDRV_UART.o ./source/Drivers/SysTick.d ./source/Drivers/SysTick.o ./source/Drivers/Timers.d ./source/Drivers/Timers.o ./source/Drivers/gpio.d ./source/Drivers/gpio.o

.PHONY: clean-source-2f-Drivers

