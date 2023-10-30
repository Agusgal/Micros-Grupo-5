################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/FSM/States/id_entry.c 

C_DEPS += \
./source/FSM/States/id_entry.d 

OBJS += \
./source/FSM/States/id_entry.o 


# Each subdirectory must supply rules for building sources it contributes
source/FSM/States/%.o: ../source/FSM/States/%.c source/FSM/States/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-FSM-2f-States

clean-source-2f-FSM-2f-States:
	-$(RM) ./source/FSM/States/id_entry.d ./source/FSM/States/id_entry.o

.PHONY: clean-source-2f-FSM-2f-States

