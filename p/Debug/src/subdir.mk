################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/indexadorHash.cpp \
../src/indexadorInformacion.cpp \
../src/p.cpp \
../src/tokenizador.cpp 

OBJS += \
./src/indexadorHash.o \
./src/indexadorInformacion.o \
./src/p.o \
./src/tokenizador.o 

CPP_DEPS += \
./src/indexadorHash.d \
./src/indexadorInformacion.d \
./src/p.d \
./src/tokenizador.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


