################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/drivers/Led.c \
../source/drivers/MplxLed.c \
../source/drivers/Timer.c \
../source/drivers/button.c \
../source/drivers/encoder.c \
../source/drivers/lector.c \
../source/drivers/seven_seg_display.c 

OBJS += \
./source/drivers/Led.o \
./source/drivers/MplxLed.o \
./source/drivers/Timer.o \
./source/drivers/button.o \
./source/drivers/encoder.o \
./source/drivers/lector.o \
./source/drivers/seven_seg_display.o 

C_DEPS += \
./source/drivers/Led.d \
./source/drivers/MplxLed.d \
./source/drivers/Timer.d \
./source/drivers/button.d \
./source/drivers/encoder.d \
./source/drivers/lector.d \
./source/drivers/seven_seg_display.d 


# Each subdirectory must supply rules for building sources it contributes
source/drivers/%.o: ../source/drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DTESTPOINT -I../source/startup -I../source/FSM/States -I../source/FSM -I../source/MCAL -I../source/drivers -I../source/board -I../source -I../ -I../CMSIS -I../source/database -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


