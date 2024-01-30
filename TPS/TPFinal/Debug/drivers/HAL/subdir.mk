################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/HAL/AudioPlayer.c \
../drivers/HAL/Buttons.c \
../drivers/HAL/Encoder.c \
../drivers/HAL/I2CWrapper.c \
../drivers/HAL/OLEDdisplay.c \
../drivers/HAL/SPI_wrapper.c \
../drivers/HAL/Timer.c \
../drivers/HAL/daytime.c \
../drivers/HAL/matrix_display.c \
../drivers/HAL/memory_handler.c \
../drivers/HAL/mp3_decoder.c 

C_DEPS += \
./drivers/HAL/AudioPlayer.d \
./drivers/HAL/Buttons.d \
./drivers/HAL/Encoder.d \
./drivers/HAL/I2CWrapper.d \
./drivers/HAL/OLEDdisplay.d \
./drivers/HAL/SPI_wrapper.d \
./drivers/HAL/Timer.d \
./drivers/HAL/daytime.d \
./drivers/HAL/matrix_display.d \
./drivers/HAL/memory_handler.d \
./drivers/HAL/mp3_decoder.d 

OBJS += \
./drivers/HAL/AudioPlayer.o \
./drivers/HAL/Buttons.o \
./drivers/HAL/Encoder.o \
./drivers/HAL/I2CWrapper.o \
./drivers/HAL/OLEDdisplay.o \
./drivers/HAL/SPI_wrapper.o \
./drivers/HAL/Timer.o \
./drivers/HAL/daytime.o \
./drivers/HAL/matrix_display.o \
./drivers/HAL/memory_handler.o \
./drivers/HAL/mp3_decoder.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/HAL/%.o: ../drivers/HAL/%.c drivers/HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/equalizer" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/CMSIS" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/startup" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/drivers" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/drivers/HAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/drivers/MCAL" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/drivers/SDK" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/id3" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/lists" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/helix" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/fatfs" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/component/fatfs/fsl_sd_disk" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/board" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/source/fft" -I"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/device" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\features\include" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\redlib\include" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers-2f-HAL

clean-drivers-2f-HAL:
	-$(RM) ./drivers/HAL/AudioPlayer.d ./drivers/HAL/AudioPlayer.o ./drivers/HAL/Buttons.d ./drivers/HAL/Buttons.o ./drivers/HAL/Encoder.d ./drivers/HAL/Encoder.o ./drivers/HAL/I2CWrapper.d ./drivers/HAL/I2CWrapper.o ./drivers/HAL/OLEDdisplay.d ./drivers/HAL/OLEDdisplay.o ./drivers/HAL/SPI_wrapper.d ./drivers/HAL/SPI_wrapper.o ./drivers/HAL/Timer.d ./drivers/HAL/Timer.o ./drivers/HAL/daytime.d ./drivers/HAL/daytime.o ./drivers/HAL/matrix_display.d ./drivers/HAL/matrix_display.o ./drivers/HAL/memory_handler.d ./drivers/HAL/memory_handler.o ./drivers/HAL/mp3_decoder.d ./drivers/HAL/mp3_decoder.o

.PHONY: clean-drivers-2f-HAL

