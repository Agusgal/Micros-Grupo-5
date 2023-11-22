################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/User/userData.c \
../source/User/userInput.c 

C_DEPS += \
./source/User/userData.d \
./source/User/userInput.d 

OBJS += \
./source/User/userData.o \
./source/User/userInput.o 


# Each subdirectory must supply rules for building sources it contributes
source/User/%.o: ../source/User/%.c source/User/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-User

clean-source-2f-User:
	-$(RM) ./source/User/userData.d ./source/User/userData.o ./source/User/userInput.d ./source/User/userInput.o

.PHONY: clean-source-2f-User

