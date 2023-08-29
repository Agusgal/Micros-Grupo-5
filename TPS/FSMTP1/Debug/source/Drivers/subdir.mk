################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Drivers/CardReader_DRV.c \
../source/Drivers/Encoder.c \
../source/Drivers/SysTick.c \
../source/Drivers/gpio.c 

OBJS += \
./source/Drivers/CardReader_DRV.o \
./source/Drivers/Encoder.o \
./source/Drivers/SysTick.o \
./source/Drivers/gpio.o 

C_DEPS += \
./source/Drivers/CardReader_DRV.d \
./source/Drivers/Encoder.d \
./source/Drivers/SysTick.d \
./source/Drivers/gpio.d 


# Each subdirectory must supply rules for building sources it contributes
source/Drivers/%.o: ../source/Drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


