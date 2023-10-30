################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/drv/CAN_SPI.c \
../source/drv/SPI.c \
../source/drv/SysTick.c \
../source/drv/UART.c \
../source/drv/accel.c \
../source/drv/gpio.c \
../source/drv/i2c.c 

C_DEPS += \
./source/drv/CAN_SPI.d \
./source/drv/SPI.d \
./source/drv/SysTick.d \
./source/drv/UART.d \
./source/drv/accel.d \
./source/drv/gpio.d \
./source/drv/i2c.d 

OBJS += \
./source/drv/CAN_SPI.o \
./source/drv/SPI.o \
./source/drv/SysTick.o \
./source/drv/UART.o \
./source/drv/accel.o \
./source/drv/gpio.o \
./source/drv/i2c.o 


# Each subdirectory must supply rules for building sources it contributes
source/drv/%.o: ../source/drv/%.c source/drv/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -D__REDLIB__ -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/SDK" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/source/drv" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/source" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/SDK/startup" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/SDK/CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wa,-adhlns="$@.lst" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-drv

clean-source-2f-drv:
	-$(RM) ./source/drv/CAN_SPI.d ./source/drv/CAN_SPI.o ./source/drv/SPI.d ./source/drv/SPI.o ./source/drv/SysTick.d ./source/drv/SysTick.o ./source/drv/UART.d ./source/drv/UART.o ./source/drv/accel.d ./source/drv/accel.o ./source/drv/gpio.d ./source/drv/gpio.o ./source/drv/i2c.d ./source/drv/i2c.o

.PHONY: clean-source-2f-drv

