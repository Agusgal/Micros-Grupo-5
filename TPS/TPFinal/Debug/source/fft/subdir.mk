################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/fft/vumeter.c 

OBJS += \
./source/fft/vumeter.o 

C_DEPS += \
./source/fft/vumeter.d 


# Each subdirectory must supply rules for building sources it contributes
source/fft/%.o: ../source/fft/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\equalizer" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\CMSIS" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\startup" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\drivers" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\drivers\HAL" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\drivers\MCAL" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\drivers\SDK" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\id3" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\lists" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\helix" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs\fsl_sd_disk" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\board" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\fft" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\device" -O1 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


