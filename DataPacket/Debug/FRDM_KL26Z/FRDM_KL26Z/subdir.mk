################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FRDM_KL26Z/FRDM_KL26Z/adc.c \
../FRDM_KL26Z/FRDM_KL26Z/gpio.c \
../FRDM_KL26Z/FRDM_KL26Z/pit_kl26z.c 

OBJS += \
./FRDM_KL26Z/FRDM_KL26Z/adc.o \
./FRDM_KL26Z/FRDM_KL26Z/gpio.o \
./FRDM_KL26Z/FRDM_KL26Z/pit_kl26z.o 

C_DEPS += \
./FRDM_KL26Z/FRDM_KL26Z/adc.d \
./FRDM_KL26Z/FRDM_KL26Z/gpio.d \
./FRDM_KL26Z/FRDM_KL26Z/pit_kl26z.d 


# Each subdirectory must supply rules for building sources it contributes
FRDM_KL26Z/FRDM_KL26Z/%.o: ../FRDM_KL26Z/FRDM_KL26Z/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D"CPU_MKL26Z128VLH4" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MKL26Z4/include" -I"C:\Freescale\KSDK_1.2.0\platform\devices\MKL26Z4\startup" -I"C:\Freescale\KSDK_1.2.0/platform/utilities/inc" -I"C:\Freescale\KSDK_1.2.0/platform/utilities/src" -I"C:\Freescale\KSDK_1.2.0\platform\osa\inc" -I"C:\Freescale\KSDK_1.2.0\platform\hal\inc" -I"C:\Freescale\KSDK_1.2.0\platform\drivers\inc" -I"C:\Freescale\KSDK_1.2.0\platform\system\inc" -I"C:\Freescale\KSDK_1.2.0" -I"C:\Freescale\KSDK_1.2.0/platform/system/inc" -I"C:\Freescale\KSDK_1.2.0/platform/drivers/src/rtc" -I"C:\Freescale\KSDK_1.2.0/platform/system/src/clock" -I"C:\Freescale\KSDK_1.2.0/platform/system/src/clock/MKL26Z4" -I"C:/kds_workspace/DataPacket/Board" -I"C:\kds_workspace\DataPacket\FRDM_KL26Z" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


