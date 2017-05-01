################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include kit_flashpwr/BRD4302A/subdir.mk
-include kit/BRD4302A/subdir.mk
-include emlib/subdir.mk
-include device/EFR32_B_1_1_B/gcc/subdir.mk
-include bgapi/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: soc-empty.axf

# Tool invocations
soc-empty.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\SiliconLabs\workspace\soc-empty\linker\GCC\efr32bg1b232f256gm56.ld" -Xlinker -no-enum-size-warning -Xlinker -no-wchar-size-warning -Xlinker --gc-sections -Xlinker -Map="soc-empty.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -o soc-empty.axf "./application_properties.o" "./ble-callback-stubs.o" "./ble-callbacks.o" "./bsp_trace.o" "./display.o" "./displayls013b7dh03.o" "./displaypalemlib.o" "./gatt_db.o" "./main.o" "./retargetio.o" "./retargettextdisplay.o" "./textdisplay.o" "./bgapi/dmadrv.o" "./bgapi/em_adc.o" "./bgapi/em_ldma.o" "./bgapi/em_leuart.o" "./device/EFR32_B_1_1_B/gcc/startup_efr32bg1b.o" "./emlib/em_aes.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_cryotimer.o" "./emlib/em_crypto.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_lcd.o" "./emlib/em_msc.o" "./emlib/em_prs.o" "./emlib/em_rtcc.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./kit/BRD4302A/bsp_bcc.o" "./kit/BRD4302A/bsp_stk.o" "./kit/BRD4302A/bsp_stk_leds.o" "./kit/BRD4302A/i2cspm.o" "./kit/BRD4302A/si7013.o" "./kit/BRD4302A/tempsens.o" "./kit/BRD4302A/udelay.o" "./kit_flashpwr/BRD4302A/mx25flash_spi.o" "./src/InitDevice.o" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1B/GCC/bgapi.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1B/EABI/binbootloader.o" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1B/EABI/stack.a" "C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1B/EABI/binstack.o" -lm -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: soc-empty.hex'
	arm-none-eabi-objcopy -O ihex "soc-empty.axf" "soc-empty.hex"
	@echo ' '

	@echo 'Building bin file: soc-empty.bin'
	arm-none-eabi-objcopy -O binary "soc-empty.axf" "soc-empty.bin"
	@echo ' '

	@echo 'Building s37 file: soc-empty.s37'
	arm-none-eabi-objcopy -O srec "soc-empty.axf" "soc-empty.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "soc-empty.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) soc-empty.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
