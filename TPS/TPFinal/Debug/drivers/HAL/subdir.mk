################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/HAL/AudioPlayer.c \
../drivers/HAL/I2CWrapper.c \
../drivers/HAL/OLEDdisplay.c \
../drivers/HAL/SPI_wrapper.c \
../drivers/HAL/Timer.c \
../drivers/HAL/button.c \
../drivers/HAL/daytime.c \
../drivers/HAL/decoder.c \
../drivers/HAL/encoder.c \
../drivers/HAL/matrix_display.c \
../drivers/HAL/memory_handler.c \
../drivers/HAL/memory_manager.c 

OBJS += \
./drivers/HAL/AudioPlayer.o \
./drivers/HAL/I2CWrapper.o \
./drivers/HAL/OLEDdisplay.o \
./drivers/HAL/SPI_wrapper.o \
./drivers/HAL/Timer.o \
./drivers/HAL/button.o \
./drivers/HAL/daytime.o \
./drivers/HAL/decoder.o \
./drivers/HAL/encoder.o \
./drivers/HAL/matrix_display.o \
./drivers/HAL/memory_handler.o \
./drivers/HAL/memory_manager.o 

C_DEPS += \
./drivers/HAL/AudioPlayer.d \
./drivers/HAL/I2CWrapper.d \
./drivers/HAL/OLEDdisplay.d \
./drivers/HAL/SPI_wrapper.d \
./drivers/HAL/Timer.d \
./drivers/HAL/button.d \
./drivers/HAL/daytime.d \
./drivers/HAL/decoder.d \
./drivers/HAL/encoder.d \
./drivers/HAL/matrix_display.d \
./drivers/HAL/memory_handler.d \
./drivers/HAL/memory_manager.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/HAL/%.o: ../drivers/HAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\audio_manager" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\equalizer" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\CMSIS" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\startup" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\drivers" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\drivers\HAL" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\drivers\MCAL" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\drivers\SDK" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\id3" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\lists" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\helix" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs\fsl_sd_disk" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\board" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\fft" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\source\file_manager" -I"C:\Users\lucia\OneDrive\Documents\2C2023\Labo_de_Micros\repo\Micros-Grupo-5\TPS\TPFinal\device" -O1 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


