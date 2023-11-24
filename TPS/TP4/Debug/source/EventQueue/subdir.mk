################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/EventQueue/queue.c 

C_DEPS += \
./source/EventQueue/queue.d 

OBJS += \
./source/EventQueue/queue.o 


# Each subdirectory must supply rules for building sources it contributes
source/EventQueue/%.o: ../source/EventQueue/%.c source/EventQueue/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/ucosiii_config" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uC-CPU" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uC-LIB" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TP4/source/rtos/uCOSIII/src/uCOS-III/Source" -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-EventQueue

clean-source-2f-EventQueue:
	-$(RM) ./source/EventQueue/queue.d ./source/EventQueue/queue.o

.PHONY: clean-source-2f-EventQueue

