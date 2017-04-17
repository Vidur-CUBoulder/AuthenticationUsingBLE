################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include sl_crypto/subdir.mk
-include mbedtls/subdir.mk
-include kit_flashpwr/BRD4100A/subdir.mk
-include emlib/subdir.mk
-include device/EFR32_B_1_1_P/gcc/subdir.mk
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
all: Trial_SOC_TLS.axf

# Tool invocations
Trial_SOC_TLS.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "/Users/vidursarin/SimplicityStudio/blue_gecko/Trial_SOC_TLS/linker/GCC/efr32bg1p232f256gm48.ld" -Xlinker -no-enum-size-warning -Xlinker -no-wchar-size-warning -Xlinker --gc-sections -Xlinker -Map="Trial_SOC_TLS.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -o Trial_SOC_TLS.axf "./application_properties.o" "./ble-callback-stubs.o" "./ble-callbacks.o" "./gatt_db.o" "./main.o" "./bgapi/dmadrv.o" "./bgapi/em_adc.o" "./bgapi/em_ldma.o" "./bgapi/em_leuart.o" "./device/EFR32_B_1_1_P/gcc/startup_efr32bg1p.o" "./emlib/em_cmu.o" "./emlib/em_cryotimer.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_msc.o" "./emlib/em_rtcc.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./kit_flashpwr/BRD4100A/mx25flash_spi.o" "./mbedtls/aes.o" "./mbedtls/aesni.o" "./mbedtls/arc4.o" "./mbedtls/asn1parse.o" "./mbedtls/asn1write.o" "./mbedtls/base64.o" "./mbedtls/bignum.o" "./mbedtls/blowfish.o" "./mbedtls/camellia.o" "./mbedtls/ccm.o" "./mbedtls/certs.o" "./mbedtls/cipher.o" "./mbedtls/cipher_wrap.o" "./mbedtls/ctr_drbg.o" "./mbedtls/debug.o" "./mbedtls/des.o" "./mbedtls/dhm.o" "./mbedtls/ecdh.o" "./mbedtls/ecdsa.o" "./mbedtls/ecjpake.o" "./mbedtls/ecp.o" "./mbedtls/ecp_curves.o" "./mbedtls/entropy.o" "./mbedtls/entropy_poll.o" "./mbedtls/error.o" "./mbedtls/gcm.o" "./mbedtls/havege.o" "./mbedtls/hmac_drbg.o" "./mbedtls/md.o" "./mbedtls/md2.o" "./mbedtls/md4.o" "./mbedtls/md5.o" "./mbedtls/md_wrap.o" "./mbedtls/memory_buffer_alloc.o" "./mbedtls/oid.o" "./mbedtls/padlock.o" "./mbedtls/pem.o" "./mbedtls/pk.o" "./mbedtls/pk_wrap.o" "./mbedtls/pkcs11.o" "./mbedtls/pkcs12.o" "./mbedtls/pkcs5.o" "./mbedtls/pkparse.o" "./mbedtls/pkwrite.o" "./mbedtls/platform.o" "./mbedtls/ripemd160.o" "./mbedtls/rsa.o" "./mbedtls/sha1.o" "./mbedtls/sha256.o" "./mbedtls/sha512.o" "./mbedtls/ssl_cache.o" "./mbedtls/ssl_ciphersuites.o" "./mbedtls/ssl_cli.o" "./mbedtls/ssl_cookie.o" "./mbedtls/ssl_srv.o" "./mbedtls/ssl_ticket.o" "./mbedtls/ssl_tls.o" "./mbedtls/threading.o" "./mbedtls/timing.o" "./mbedtls/version.o" "./mbedtls/version_features.o" "./mbedtls/xtea.o" "./sl_crypto/aesdrv_authencr_aes.o" "./sl_crypto/aesdrv_authencr_crypto.o" "./sl_crypto/aesdrv_blockcipher_aes.o" "./sl_crypto/aesdrv_blockcipher_crypto.o" "./sl_crypto/aesdrv_common.o" "./sl_crypto/aesdrv_common_aes.o" "./sl_crypto/aesdrv_common_crypto.o" "./sl_crypto/cryptodrv.o" "./sl_crypto/sl_aes.o" "./sl_crypto/sl_ecp.o" "./sl_crypto/sl_entropy.o" "./sl_crypto/sl_sha1.o" "./sl_crypto/sl_sha256.o" "./sl_crypto/sl_timing.o" "./sl_crypto/slcl_aes.o" "./sl_crypto/slcl_ccm.o" "./sl_crypto/slcl_cmac.o" "./sl_crypto/slcl_device.o" "./sl_crypto/slcl_ecp.o" "./sl_crypto/slcl_sha1.o" "./sl_crypto/slcl_sha256.o" "./sl_crypto/slpal_freertos.o" "./sl_crypto/slpal_ucos3.o" "./sl_crypto/trng.o" "./src/InitDevice.o" "/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1P/EABI/stack.a" "/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1P/EABI/binstack.o" "/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1P/GCC/bgapi.a" "/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1P/EABI/binbootloader.o" -lm -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: Trial_SOC_TLS.hex'
	arm-none-eabi-objcopy -O ihex "Trial_SOC_TLS.axf" "Trial_SOC_TLS.hex"
	@echo ' '

	@echo 'Building bin file: Trial_SOC_TLS.bin'
	arm-none-eabi-objcopy -O binary "Trial_SOC_TLS.axf" "Trial_SOC_TLS.bin"
	@echo ' '

	@echo 'Building s37 file: Trial_SOC_TLS.s37'
	arm-none-eabi-objcopy -O srec "Trial_SOC_TLS.axf" "Trial_SOC_TLS.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "Trial_SOC_TLS.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Trial_SOC_TLS.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
