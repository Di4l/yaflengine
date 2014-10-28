################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../fzbase.cpp \
../fzexecution.cpp \
../fzfunction.cpp \
../fzmodel.cpp \
../fzrules.cpp \
../fzset.cpp \
../fzval.cpp 

OBJS += \
./fzbase.o \
./fzexecution.o \
./fzfunction.o \
./fzmodel.o \
./fzrules.o \
./fzset.o \
./fzval.o 

CPP_DEPS += \
./fzbase.d \
./fzexecution.d \
./fzfunction.d \
./fzmodel.d \
./fzrules.d \
./fzset.d \
./fzval.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/media/sf_prj/yaflengine/src/ini_file" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


