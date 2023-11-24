################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include source/rtos/uCOSIII/src/uCOS-III/Source/subdir.mk
-include source/rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/RealView/subdir.mk
-include source/rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/IAR/subdir.mk
-include source/rtos/uCOSIII/src/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/subdir.mk
-include source/rtos/uCOSIII/src/uC-LIB/Ports/ARM-Cortex-M4/RealView/subdir.mk
-include source/rtos/uCOSIII/src/uC-LIB/Ports/ARM-Cortex-M4/IAR/subdir.mk
-include source/rtos/uCOSIII/src/uC-LIB/Ports/ARM-Cortex-M4/GNU/subdir.mk
-include source/rtos/uCOSIII/src/uC-LIB/subdir.mk
-include source/rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/RealView/subdir.mk
-include source/rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/IAR/subdir.mk
-include source/rtos/uCOSIII/src/uC-CPU/ARM-Cortex-M4/GNU/subdir.mk
-include source/rtos/uCOSIII/src/uC-CPU/subdir.mk
-include source/rtos/uCOSIII/BSP/subdir.mk
-include source/User/Debug/source/User/subdir.mk
-include source/User/Debug/source/FSM/States/subdir.mk
-include source/User/Debug/source/FSM/subdir.mk
-include source/User/Debug/source/EventQueue/subdir.mk
-include source/User/Debug/source/Drivers/subdir.mk
-include source/User/Debug/source/subdir.mk
-include source/User/Debug/SDK/startup/subdir.mk
-include source/User/Debug/SDK/CMSIS/subdir.mk
-include source/User/subdir.mk
-include source/FSM/States/subdir.mk
-include source/FSM/subdir.mk
-include source/EventQueue/subdir.mk
-include source/Drivers/subdir.mk
-include source/subdir.mk
-include SDK/startup/subdir.mk
-include SDK/CMSIS/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := TP4
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: TP4.axf

# Tool invocations
TP4.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="TP4.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T TP4_Debug.ld -o "TP4.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) TP4.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TP4.axf"; # arm-none-eabi-objcopy -v -O binary "TP4.axf" "TP4.bin" ; # checksum -p MK64FN1M0xxx12 -d "TP4.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
