################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/hardware.c \
../startup/main.c \
../startup/semihost_hardfault.c \
../startup/startup_mk64f12.c 

OBJS += \
./startup/hardware.o \
./startup/main.o \
./startup/semihost_hardfault.o \
./startup/startup_mk64f12.o 

C_DEPS += \
./startup/hardware.d \
./startup/main.d \
./startup/semihost_hardfault.d \
./startup/startup_mk64f12.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DSD_ENABLE -DCPU_MK64FN1M0VLL12_cm4 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -D__GNUC__ -D__REDLIB__ -DDEBUG -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\equalizer" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\CMSIS" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\startup" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\id3" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\lists" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\helix" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\component\fatfs\fsl_sd_disk" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\device" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\features\include" -I"C:\nxp\MCUXpressoIDE_10.2.0_759\ide\tools\redlib\include" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\drivers" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\drivers\HAL" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\drivers\MCAL" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\drivers\SDK" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\FSM" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\mp3_file_handler" -I"/TPFinal/source/mp3_handler" -I"C:\Bruno\ITBA\7mo cuatri\Lab. de Microprocesadores\Repo\Micros-Grupo-5\TPS\TPFinal\source\board" -Og -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


