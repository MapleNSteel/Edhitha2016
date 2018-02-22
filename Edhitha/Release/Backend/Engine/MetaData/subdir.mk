################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Backend/Engine/MetaData/metadata.cpp 

C_SRCS += \
../Backend/Engine/MetaData/gpsData.c 

OBJS += \
./Backend/Engine/MetaData/gpsData.o \
./Backend/Engine/MetaData/metadata.o 

CPP_DEPS += \
./Backend/Engine/MetaData/metadata.d 

C_DEPS += \
./Backend/Engine/MetaData/gpsData.d 


# Each subdirectory must supply rules for building sources it contributes
Backend/Engine/MetaData/%.o: ../Backend/Engine/MetaData/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/local/include/boost -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Backend/Engine/MetaData/%.o: ../Backend/Engine/MetaData/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/usr/local/include/boost -I/usr/local/include/opencv -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


