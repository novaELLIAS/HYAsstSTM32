################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/SIM7020Commander/AT.c \
../Core/Src/SIM7020Commander/AT_Onenet_LWM2M.c \
../Core/Src/SIM7020Commander/General_Command.c \
../Core/Src/SIM7020Commander/utility_basic.c 

C_DEPS += \
./Core/Src/SIM7020Commander/AT.d \
./Core/Src/SIM7020Commander/AT_Onenet_LWM2M.d \
./Core/Src/SIM7020Commander/General_Command.d \
./Core/Src/SIM7020Commander/utility_basic.d 

OBJS += \
./Core/Src/SIM7020Commander/AT.o \
./Core/Src/SIM7020Commander/AT_Onenet_LWM2M.o \
./Core/Src/SIM7020Commander/General_Command.o \
./Core/Src/SIM7020Commander/utility_basic.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/SIM7020Commander/AT.o: ../Core/Src/SIM7020Commander/AT.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SIM7020Commander/AT.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/SIM7020Commander/AT_Onenet_LWM2M.o: ../Core/Src/SIM7020Commander/AT_Onenet_LWM2M.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SIM7020Commander/AT_Onenet_LWM2M.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/SIM7020Commander/General_Command.o: ../Core/Src/SIM7020Commander/General_Command.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SIM7020Commander/General_Command.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/SIM7020Commander/utility_basic.o: ../Core/Src/SIM7020Commander/utility_basic.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/SIM7020Commander/utility_basic.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

