# Build mode
# 0: development (max safety, no optimisation)
# 1: release (min safety, optimisation)
# 2: fast and furious (no safety, optimisation)
BUILD_MODE?=0

all: main
	
# 0: monolith version, the GBSurface is rendered toward a TGA image
# 1: GTK version, the GBSurface is rendered toward a TGA image or 
#    a GtkWidget
BUILDWITHGRAPHICLIB=0

# Makefile definitions
MAKEFILE_INC=../PBMake/Makefile.inc
include $(MAKEFILE_INC)

# Rules to make the executable
repo=genbrush
$($(repo)_EXENAME): \
		$($(repo)_EXENAME).o \
		$($(repo)_EXE_DEP) \
		$($(repo)_DEP)
	$(COMPILER) `echo "$($(repo)_EXE_DEP) $($(repo)_EXENAME).o" | tr ' ' '\n' | sort -u` $(LINK_ARG) $($(repo)_LINK_ARG) -o $($(repo)_EXENAME) 
	
$($(repo)_EXENAME).o: \
		$($(repo)_DIR)/$($(repo)_EXENAME).c \
		$($(repo)_INC_H_EXE) \
		$($(repo)_EXE_DEP)
	$(COMPILER) $(BUILD_ARG) $($(repo)_BUILD_ARG) `echo "$($(repo)_INC_DIR)" | tr ' ' '\n' | sort -u` -c $($(repo)_DIR)/$($(repo)_EXENAME).c
	
