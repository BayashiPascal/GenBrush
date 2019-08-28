# Build mode
# 0: development (max safety, no optimisation)
# 1: release (min safety, optimisation)
# 2: fast and furious (no safety, optimisation)
BUILD_MODE=1

all: main
	
# 0: monolith version, the GBSurface is rendered toward a TGA image
# 1: GTK version, the GBSurface is rendered toward a TGA image or 
#    a GtkWidget
BUILDWITHGRAPHICLIB=1

# Makefile definitions
MAKEFILE_INC=../../../PBMake/Makefile.inc
include $(MAKEFILE_INC)

# Dependencies of the executable (*.o)
main_DEP=

# Dependencies of the executable (*.h)
main_INC_H=\
	$(ROOT_DIR)/GenBrush/genbrush.h \
	$(ROOT_DIR)/PBMath/pbmath.h

# Rules to make the executable
repo=genbrush
main: \
		main.o \
		$(main_DEP) \
		$(genbrush_EXE_DEP)
	$(COMPILER) `echo "$(main_DEP) $(genbrush_EXE_DEP) main.o" | tr ' ' '\n' | sort -u` $(LINK_ARG) $(genbrush_LINK_ARG) -o main 
	
main.o: \
		main.c \
		$(main_INC_H) \
		Makefile
	$(COMPILER) $(BUILD_ARG) $(genbrush_BUILD_ARG) `echo "$(genbrush_INC_DIR)" | tr ' ' '\n' | sort -u` -c main.c
	
