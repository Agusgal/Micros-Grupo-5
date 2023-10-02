################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/FSM/States/add_user_state/add_user_fsm_table.c \
../source/FSM/States/add_user_state/add_user_state.c 

OBJS += \
./source/FSM/States/add_user_state/add_user_fsm_table.o \
./source/FSM/States/add_user_state/add_user_state.o 

C_DEPS += \
./source/FSM/States/add_user_state/add_user_fsm_table.d \
./source/FSM/States/add_user_state/add_user_state.d 


# Each subdirectory must supply rules for building sources it contributes
source/FSM/States/add_user_state/%.o: ../source/FSM/States/add_user_state/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DCPU_MK64FN1M0VLL12_cm4 -DCPU_MK64FN1M0VLL12 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DTESTPOINT -I../source/startup -I../source/FSM/States -I../source/FSM -I../source/MCAL -I../source/drivers -I../source/board -I../source -I../ -I../CMSIS -I../source/database -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


