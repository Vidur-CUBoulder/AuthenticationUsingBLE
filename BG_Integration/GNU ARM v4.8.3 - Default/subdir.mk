################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../application_properties.c \
../ble-callback-stubs.c \
../ble-callbacks.c \
../gatt_db.c \
../main.c 

OBJS += \
./application_properties.o \
./ble-callback-stubs.o \
./ble-callbacks.o \
./gatt_db.o \
./main.o 

C_DEPS += \
./application_properties.d \
./ble-callback-stubs.d \
./ble-callbacks.d \
./gatt_db.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
application_properties.o: ../application_properties.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DGENERATION_DONE=1' '-DSILABS_AF_USE_HWCONF=1' '-D__NO_SYSTEM_INIT=1' '-DEFR32BG1P232F256GM48=1' -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/Device/SiliconLabs/EFR32BG1P/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/common/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/gpiointerrupt/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/sleep/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/EFR32BG1_BRD4100A/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32/rf/common/cortex" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/src" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/soc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/bootloader/api" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/configs" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include/mbedtls" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/sl_crypto/include" -O2 -fno-short-enums -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"application_properties.d" -MT"application_properties.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ble-callback-stubs.o: ../ble-callback-stubs.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DGENERATION_DONE=1' '-DSILABS_AF_USE_HWCONF=1' '-D__NO_SYSTEM_INIT=1' '-DEFR32BG1P232F256GM48=1' -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/Device/SiliconLabs/EFR32BG1P/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/common/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/gpiointerrupt/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/sleep/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/EFR32BG1_BRD4100A/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32/rf/common/cortex" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/src" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/soc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/bootloader/api" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/configs" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include/mbedtls" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/sl_crypto/include" -O2 -fno-short-enums -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"ble-callback-stubs.d" -MT"ble-callback-stubs.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ble-callbacks.o: ../ble-callbacks.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DGENERATION_DONE=1' '-DSILABS_AF_USE_HWCONF=1' '-D__NO_SYSTEM_INIT=1' '-DEFR32BG1P232F256GM48=1' -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/Device/SiliconLabs/EFR32BG1P/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/common/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/gpiointerrupt/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/sleep/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/EFR32BG1_BRD4100A/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32/rf/common/cortex" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/src" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/soc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/bootloader/api" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/configs" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include/mbedtls" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/sl_crypto/include" -O2 -fno-short-enums -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"ble-callbacks.d" -MT"ble-callbacks.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gatt_db.o: ../gatt_db.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DGENERATION_DONE=1' '-DSILABS_AF_USE_HWCONF=1' '-D__NO_SYSTEM_INIT=1' '-DEFR32BG1P232F256GM48=1' -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/Device/SiliconLabs/EFR32BG1P/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/common/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/gpiointerrupt/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/sleep/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/EFR32BG1_BRD4100A/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32/rf/common/cortex" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/src" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/soc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/bootloader/api" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/configs" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include/mbedtls" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/sl_crypto/include" -O2 -fno-short-enums -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"gatt_db.d" -MT"gatt_db.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.o: ../main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DGENERATION_DONE=1' '-DSILABS_AF_USE_HWCONF=1' '-D__NO_SYSTEM_INIT=1' '-DEFR32BG1P232F256GM48=1' -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/Device/SiliconLabs/EFR32BG1P/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/common/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/dmadrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/gpiointerrupt/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/nvm/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/rtcdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/sleep/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/spidrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/tempdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/uartdrv/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emdrv/ustimer/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/EFR32BG1_BRD4100A/config" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32/rf/common/cortex" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/radio/rail_lib/chip/efr32" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS" -I"/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/src" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/soc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/ble_stack/inc/common" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/bootloader/api" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/configs" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/include/mbedtls" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//util/third_party/mbedtls/sl_crypto/include" -O2 -fno-short-enums -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"main.d" -MT"main.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


