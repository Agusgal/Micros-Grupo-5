################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/CMSIS/system_MK64F12.c 

C_DEPS += \
./SDK/CMSIS/system_MK64F12.d 

OBJS += \
./SDK/CMSIS/system_MK64F12.o 


# Each subdirectory must supply rules for building sources it contributes
SDK/CMSIS/%.o: ../SDK/CMSIS/%.c SDK/CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -D__REDLIB__ -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/SDK" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/source/drv" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/source" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/SDK/startup" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/I2C/SDK/CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wa,-adhlns="$@.lst" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-SDK-2f-CMSIS

clean-SDK-2f-CMSIS:
	-$(RM) ./SDK/CMSIS/system_MK64F12.d ./SDK/CMSIS/system_MK64F12.o

.PHONY: clean-SDK-2f-CMSIS

