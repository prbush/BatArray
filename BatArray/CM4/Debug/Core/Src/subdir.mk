################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/gnss.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/rtc.c \
../Core/Src/sdcard.c \
../Core/Src/sdmmc.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c 

OBJS += \
./Core/Src/gnss.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/rtc.o \
./Core/Src/sdcard.o \
./Core/Src/sdmmc.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o 

C_DEPS += \
./Core/Src/gnss.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/rtc.d \
./Core/Src/sdcard.d \
./Core/Src/sdmmc.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_NUCLEO_64 -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Middlewares/Third_Party/FatFs/src -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/BSP/STM32H7xx_Nucleo -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/philbush/STM32Cube/Repository/STM32Cube_FW_H7_V1.11.2/Drivers/CMSIS/Include -I"/Users/philbush/STM32CubeIDE/workspace_1.14.0/BatArray/BatArray/Common/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/gnss.cyclo ./Core/Src/gnss.d ./Core/Src/gnss.o ./Core/Src/gnss.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/rtc.cyclo ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/sdcard.cyclo ./Core/Src/sdcard.d ./Core/Src/sdcard.o ./Core/Src/sdcard.su ./Core/Src/sdmmc.cyclo ./Core/Src/sdmmc.d ./Core/Src/sdmmc.o ./Core/Src/sdmmc.su ./Core/Src/stm32h7xx_hal_msp.cyclo ./Core/Src/stm32h7xx_hal_msp.d ./Core/Src/stm32h7xx_hal_msp.o ./Core/Src/stm32h7xx_hal_msp.su ./Core/Src/stm32h7xx_it.cyclo ./Core/Src/stm32h7xx_it.d ./Core/Src/stm32h7xx_it.o ./Core/Src/stm32h7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su

.PHONY: clean-Core-2f-Src

