################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/generic_list.c 

C_DEPS += \
./component/lists/generic_list.d 

OBJS += \
./component/lists/generic_list.o 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c component/lists/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/audio_manager" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/equalizer" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/CMSIS" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/startup" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers/HAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers/MCAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers/SDK" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers/SDK/SD" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/id3" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/lists" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/helix" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/fatfs" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/fatfs/fsl_sd_disk" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/board" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/fft" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/fsm" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/file_manager" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/device" -O1 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-lists

clean-component-2f-lists:
	-$(RM) ./component/lists/generic_list.d ./component/lists/generic_list.o

.PHONY: clean-component-2f-lists

