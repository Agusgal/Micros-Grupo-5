################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/MCAL/SysTick.c \
../drivers/MCAL/ftm.c \
../drivers/MCAL/gpio.c \
../drivers/MCAL/uart.c 

C_DEPS += \
./drivers/MCAL/SysTick.d \
./drivers/MCAL/ftm.d \
./drivers/MCAL/gpio.d \
./drivers/MCAL/uart.d 

OBJS += \
./drivers/MCAL/SysTick.o \
./drivers/MCAL/ftm.o \
./drivers/MCAL/gpio.o \
./drivers/MCAL/uart.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/MCAL/%.o: ../drivers/MCAL/%.c drivers/MCAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/audio_manager" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/equalizer" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/CMSIS" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/startup" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers/HAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers/MCAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers/SDK" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/drivers/SDK/SD" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/id3" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/lists" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/helix" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/fatfs" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/component/fatfs/fsl_sd_disk" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/board" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/fft" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/fsm" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/source/file_manager" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/MP3Player/device" -O1 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers-2f-MCAL

clean-drivers-2f-MCAL:
	-$(RM) ./drivers/MCAL/SysTick.d ./drivers/MCAL/SysTick.o ./drivers/MCAL/ftm.d ./drivers/MCAL/ftm.o ./drivers/MCAL/gpio.d ./drivers/MCAL/gpio.o ./drivers/MCAL/uart.d ./drivers/MCAL/uart.o

.PHONY: clean-drivers-2f-MCAL

