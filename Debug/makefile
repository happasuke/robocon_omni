################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include mbed/TARGET_LPC1768/TOOLCHAIN_GCC_CR/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: robocon2015_omni3.axf

# Tool invocations
robocon2015_omni3.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"D:\src\xpresso\robocon2015_omni3" -L"D:\src\xpresso\robocon2015_omni3\mbed" -L"D:\src\xpresso\robocon2015_omni3\mbed\TARGET_LPC1768" -L"D:\src\xpresso\robocon2015_omni3\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR" -L"D:\src\xpresso\robocon2015_omni3\mbed\TARGET_LPC1768\TARGET_NXP" -L"D:\src\xpresso\robocon2015_omni3\mbed\TARGET_LPC1768\TARGET_NXP\TARGET_LPC176X" -L"D:\src\xpresso\robocon2015_omni3\mbed\TARGET_LPC1768\TARGET_NXP\TARGET_LPC176X\TARGET_MBED_LPC1768" -Xlinker -Map="robocon2015_omni3.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "D:\src\xpresso\robocon2015_omni3\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR\LPC1768.ld" -o "robocon2015_omni3.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) robocon2015_omni3.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "robocon2015_omni3.axf"; arm-none-eabi-objcopy -O binary "robocon2015_omni3.axf" "robocon2015_omni3.bin" ; # checksum -p LPC1768 -d "robocon2015_omni3.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
