################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Geometry.cpp \
../src/InputHandler.cpp \
../src/Program.cpp \
../src/RenderEngine.cpp \
../src/ShaderTools.cpp \
../src/main.cpp 

OBJS += \
./src/Geometry.o \
./src/InputHandler.o \
./src/Program.o \
./src/RenderEngine.o \
./src/ShaderTools.o \
./src/main.o 

CPP_DEPS += \
./src/Geometry.d \
./src/InputHandler.d \
./src/Program.d \
./src/RenderEngine.d \
./src/ShaderTools.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/ugb/adrian.bathan/Desktop/cpsc589/boilerplate/boilerplate/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


