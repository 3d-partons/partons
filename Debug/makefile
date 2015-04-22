################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/utils/vector/subdir.mk
-include src/utils/stringUtils/subdir.mk
-include src/utils/parser/xml/subdir.mk
-include src/utils/parser/subdir.mk
-include src/utils/logger/subdir.mk
-include src/utils/fileUtils/subdir.mk
-include src/utils/subdir.mk
-include src/services/subdir.mk
-include src/modules/observable/DVCS/subdir.mk
-include src/modules/observable/subdir.mk
-include src/modules/math/subdir.mk
-include src/modules/gpd/subdir.mk
-include src/modules/evolQCD/subdir.mk
-include src/modules/cff/subdir.mk
-include src/modules/alphaS/subdir.mk
-include src/modules/subdir.mk
-include src/beans/observable/subdir.mk
-include src/beans/kinematic/subdir.mk
-include src/beans/gpd/subdir.mk
-include src/beans/cff/subdir.mk
-include src/beans/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libPARTONS.dylib

# Tool invocations
libPARTONS.dylib: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MacOS X C++ Linker'
	g++ -L/opt/local/lib -L/usr/lib -L/usr/lib/c++ -L"/opt/root-v5-32-00/lib" -Xlinker -rpath /opt/root-v5-32-00/lib -dynamiclib -o "libPARTONS.dylib" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) libPARTONS.dylib
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
