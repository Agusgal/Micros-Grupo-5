################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/startup/hardware.c \
../SDK/startup/main.c \
../SDK/startup/semihost_hardfault.c \
../SDK/startup/startup_mk64f12.c 

C_DEPS += \
./SDK/startup/hardware.d \
./SDK/startup/main.d \
./SDK/startup/semihost_hardfault.d \
./SDK/startup/startup_mk64f12.d 

OBJS += \
./SDK/startup/hardware.o \
./SDK/startup/main.o \
./SDK/startup/semihost_hardfault.o \
./SDK/startup/startup_mk64f12.o 


# Each subdirectory must supply rules for building sources it contributes
SDK/startup/%.o: ../SDK/startup/%.c SDK/startup/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"/Users/agus/Downloads/Documentacion TPFINAL1" -I"/Users/agus/Downloads/Documentacion TPFINAL1/source/audio_manager" -I"/Users/agus/Downloads/Documentacion TPFINAL1/source/equalizer" -I"/Users/agus/Downloads/Documentacion TPFINAL1/drivers" -I"/Users/agus/Downloads/Documentacion TPFINAL1/drivers/HAL" -I"/Users/agus/Downloads/Documentacion TPFINAL1/drivers/MCAL" -I"/Users/agus/Downloads/Documentacion TPFINAL1/drivers/SDK" -I"/Users/agus/Downloads/Documentacion TPFINAL1/drivers/SDK/SD" -I"/Users/agus/Downloads/Documentacion TPFINAL1/component/id3" -I"/Users/agus/Downloads/Documentacion TPFINAL1/component/lists" -I"/Users/agus/Downloads/Documentacion TPFINAL1/component/helix" -I"/Users/agus/Downloads/Documentacion TPFINAL1/component/fatfs" -I"/Users/agus/Downloads/Documentacion TPFINAL1/component/fatfs/fsl_sd_disk" -I"/Users/agus/Downloads/Documentacion TPFINAL1/board" -I"/Users/agus/Downloads/Documentacion TPFINAL1/source" -I"/Users/agus/Downloads/Documentacion TPFINAL1/source/fft" -I"/Users/agus/Downloads/Documentacion TPFINAL1/source/fsm" -I"/Users/agus/Downloads/Documentacion TPFINAL1/source/file_manager" -I"/Users/agus/Downloads/Documentacion TPFINAL1/device" -O1 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-SDK-2f-startup

clean-SDK-2f-startup:
	-$(RM) ./SDK/startup/hardware.d ./SDK/startup/hardware.o ./SDK/startup/main.d ./SDK/startup/main.o ./SDK/startup/semihost_hardfault.d ./SDK/startup/semihost_hardfault.o ./SDK/startup/startup_mk64f12.d ./SDK/startup/startup_mk64f12.o

.PHONY: clean-SDK-2f-startup

