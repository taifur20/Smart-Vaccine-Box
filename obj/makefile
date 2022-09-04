################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include rtthread/src/subdir.mk
-include rtthread/libcpu/risc-v/common/subdir.mk
-include rtthread/components/finsh/subdir.mk
-include rtthread/components/drivers/serial/subdir.mk
-include rtthread/components/drivers/misc/subdir.mk
-include rtthread/subdir.mk
-include drivers/subdir.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include NEO7/subdir.mk
-include LCD/subdir.mk
-include HSM20G/subdir.mk
-include GPRS/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
smart_vaccine_box.hex \

SECONDARY_LIST += \
smart_vaccine_box.lst \

SECONDARY_SIZE += \
smart_vaccine_box.siz \


# All Target
all: smart_vaccine_box.elf secondary-outputs

# Tool invocations
smart_vaccine_box.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "C:\Users\khair\Desktop\WCH Project\ch32v307-main\ch32v307-main\EVT\EXAM\RT-Thread\rt-thread\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"smart_vaccine_box.map" --specs=nano.specs --specs=nosys.specs -o "smart_vaccine_box.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
smart_vaccine_box.hex: smart_vaccine_box.elf
	@	riscv-none-embed-objcopy -O ihex "smart_vaccine_box.elf"  "smart_vaccine_box.hex"
	@	@
smart_vaccine_box.lst: smart_vaccine_box.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "smart_vaccine_box.elf" > "smart_vaccine_box.lst"
	@	@
smart_vaccine_box.siz: smart_vaccine_box.elf
	@	riscv-none-embed-size --format=berkeley "smart_vaccine_box.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) smart_vaccine_box.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
