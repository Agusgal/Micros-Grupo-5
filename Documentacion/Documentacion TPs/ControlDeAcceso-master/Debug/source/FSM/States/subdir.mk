################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/FSM/States/config_device_state.c \
../source/FSM/States/config_me_state.c \
../source/FSM/States/delete_user_state.c \
../source/FSM/States/id_state.c \
../source/FSM/States/menu_state.c \
../source/FSM/States/open_state.c \
../source/FSM/States/pin_state.c \
../source/FSM/States/user_input.c \
../source/FSM/States/welcome_state.c 

OBJS += \
./source/FSM/States/config_device_state.o \
./source/FSM/States/config_me_state.o \
./source/FSM/States/delete_user_state.o \
./source/FSM/States/id_state.o \
./source/FSM/States/menu_state.o \
./source/FSM/States/open_state.o \
./source/FSM/States/pin_state.o \
./source/FSM/States/user_input.o \
./source/FSM/States/welcome_state.o 

C_DEPS += \
./source/FSM/States/config_device_state.d \
./source/FSM/States/config_me_state.d \
./source/FSM/States/delete_user_state.d \
./source/FSM/States/id_state.d \
./source/FSM/States/menu_state.d \
./source/FSM/States/open_state.d \
./source/FSM/States/pin_state.d \
./source/FSM/States/user_input.d \
./source/FSM/States/welcome_state.d 


# Each subdirectory must supply rules for building sources it contributes
source/FSM/States/%.o: ../source/FSM/States/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DTESTPOINT -I../source/startup -I../source/FSM/States -I../source/FSM -I../source/MCAL -I../source/drivers -I../source/board -I../source -I../ -I../CMSIS -I../source/database -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


