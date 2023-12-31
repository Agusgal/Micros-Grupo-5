################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/FTM.c \
../source/GPIO.c \
../source/PORT.c \
../source/hardware.c \
../source/main.c 

C_DEPS += \
./source/FTM.d \
./source/GPIO.d \
./source/PORT.d \
./source/hardware.d \
./source/main.d 

OBJS += \
./source/FTM.o \
./source/GPIO.o \
./source/PORT.o \
./source/hardware.o \
./source/main.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -D__REDLIB__ -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/TP3/Ejemplos Dani FTM/FTM_IC/source" -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/TP3/Ejemplos Dani FTM/FTM_IC" -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/TP3/Ejemplos Dani FTM/FTM_IC/startup" -I"/Users/juanpablo/Documents/Micros/MCUXpressoIDE/workspace/Micros-Grupo-5/TPS/TP3/Ejemplos Dani FTM/FTM_IC/CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wa,-adhlns="$@.lst" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/FTM.d ./source/FTM.o ./source/GPIO.d ./source/GPIO.o ./source/PORT.d ./source/PORT.o ./source/hardware.d ./source/hardware.o ./source/main.d ./source/main.o

.PHONY: clean-source

