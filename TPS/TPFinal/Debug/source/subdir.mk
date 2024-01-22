################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App.c \
../source/App_test.c 

<<<<<<< HEAD
OBJS += \
./source/App.o \
./source/App_test.o 

=======
>>>>>>> 516a446ea5f6eac7c06b427170ec0043fd1c4421
C_DEPS += \
./source/App.d \
./source/App_test.d 

OBJS += \
./source/App.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/audio_manager" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/equalizer" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/CMSIS" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/startup" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/drivers" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/drivers/HAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/drivers/MCAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/drivers/SDK" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/id3" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/lists" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/helix" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/fatfs" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/fatfs/fsl_sd_disk" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/board" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/fft" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/file_manager" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/device" -O1 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/App.d ./source/App.o

.PHONY: clean-source

