################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Fuzzy_Library/Fuzzy.c 

OBJS += \
./Fuzzy_Library/Fuzzy.o 

C_DEPS += \
./Fuzzy_Library/Fuzzy.d 


# Each subdirectory must supply rules for building sources it contributes
Fuzzy_Library/%.o Fuzzy_Library/%.su Fuzzy_Library/%.cyclo: ../Fuzzy_Library/%.c Fuzzy_Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Usuario/Documents/Workspace STM32/F446RE_LCD/Fuzzy_Library" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Fuzzy_Library

clean-Fuzzy_Library:
	-$(RM) ./Fuzzy_Library/Fuzzy.cyclo ./Fuzzy_Library/Fuzzy.d ./Fuzzy_Library/Fuzzy.o ./Fuzzy_Library/Fuzzy.su

.PHONY: clean-Fuzzy_Library

