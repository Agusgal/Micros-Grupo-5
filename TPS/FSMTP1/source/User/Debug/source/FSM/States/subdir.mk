################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/FSM/States/card_entry.c \
../source/FSM/States/encoder_entry.c \
../source/FSM/States/id_entry.c \
../source/FSM/States/pin_entry.c 

OBJS += \
./source/FSM/States/card_entry.o \
./source/FSM/States/encoder_entry.o \
./source/FSM/States/id_entry.o \
./source/FSM/States/pin_entry.o 

C_DEPS += \
./source/FSM/States/card_entry.d \
./source/FSM/States/encoder_entry.d \
./source/FSM/States/id_entry.d \
./source/FSM/States/pin_entry.d 


# Each subdirectory must supply rules for building sources it contributes
source/FSM/States/%.o: ../source/FSM/States/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


