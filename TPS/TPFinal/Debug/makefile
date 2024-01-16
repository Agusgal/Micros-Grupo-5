################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/file_manager/subdir.mk
-include source/fft/subdir.mk
-include source/equalizer/subdir.mk
-include source/audio_manager/subdir.mk
-include source/User/Debug/source/User/subdir.mk
-include source/User/Debug/source/FSM/States/subdir.mk
-include source/User/Debug/source/FSM/subdir.mk
-include source/User/Debug/source/EventQueue/subdir.mk
-include source/User/Debug/source/Drivers/subdir.mk
-include source/User/Debug/source/subdir.mk
-include source/User/Debug/SDK/startup/subdir.mk
-include source/User/Debug/SDK/CMSIS/subdir.mk
-include source/User/subdir.mk
-include source/FSM_1/States/subdir.mk
-include source/FSM_1/subdir.mk
-include source/EventQueue/subdir.mk
-include source/subdir.mk
-include drivers/SDK/SD/subdir.mk
-include drivers/SDK/subdir.mk
-include drivers/MCAL/subdir.mk
-include drivers/HAL/subdir.mk
-include device/subdir.mk
-include component/lists/subdir.mk
-include component/id3/subdir.mk
-include component/helix/real/subdir.mk
-include component/helix/subdir.mk
-include component/fatfs/fsl_sd_disk/subdir.mk
-include component/fatfs/subdir.mk
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


BUILD_ARTIFACT_NAME := TPFinal
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: TPFinal.axf

# Tool invocations
TPFinal.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/Users/agus/Micros/Github/Micros-Grupo-5/TPS/TPFinal/libs" -Xlinker -Map="TPFinal.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T MP3Player_Debug.ld -o "TPFinal.axf" $(OBJS) $(USER_OBJS) $(LIBS) -larm_cortexM4lf_math
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) TPFinal.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TPFinal.axf"; # arm-none-eabi-objcopy -v -O binary "TPFinal.axf" "TPFinal.bin" ; # checksum -p MK64FN1M0xxx12 -d "TPFinal.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
