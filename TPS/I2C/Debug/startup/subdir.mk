################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/hardware.c \
../startup/startup_mk64f12.c 

C_DEPS += \
./startup/hardware.d \
./startup/startup_mk64f12.d 

OBJS += \
./startup/hardware.o \
./startup/startup_mk64f12.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -D__REDLIB__ -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/I2C/SDK" -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/I2C/source" -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/I2C" -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/I2C/startup" -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/I2C/CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wa,-adhlns="$@.lst" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-startup

clean-startup:
	-$(RM) ./startup/hardware.d ./startup/hardware.o ./startup/startup_mk64f12.d ./startup/startup_mk64f12.o

.PHONY: clean-startup

