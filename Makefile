#directory
PBERRDIR=../PBErr
PBMATHDIR=../PBMath
GSETDIR=../GSet
SHAPOIDDIR=../Shapoid
BCURVEDIR=../BCurve
GTREEDIR=../GTree
PBJSONDIR=../PBJson

# Build mode
# 0: development (max safety, no optimisation)
# 1: release (min safety, optimisation)
# 2: fast and furious (no safety, optimisation)
BUILDMODE=1

# 0: monolith version, the GBSurface is rendered toward a TGA image
# 1: GTK version, the GBSurface is rendered toward a TGA image or 
#    a GtkWidget
BUILDWITHGRAPHICLIB=0

include $(PBERRDIR)/Makefile.inc

ifeq ($(BUILDWITHGRAPHICLIB),1)
	GTKPARAM=`pkg-config --cflags gtk+-3.0` -DBUILDWITHGRAPHICLIB=$(BUILDWITHGRAPHICLIB)
	GTKLINK=`pkg-config --libs gtk+-3.0`
	CAIROPARAM=`pkg-config --cflags cairo`
	CAIROLINK=`pkg-config --libs cairo`
endif

INCPATH=-I./ -I$(PBERRDIR)/ -I$(PBMATHDIR)/ -I$(GSETDIR)/ -I$(SHAPOIDDIR)/ -I$(BCURVEDIR)/ -I$(PBJSONDIR)/ -I$(GTREEDIR)/
BUILDOPTIONS=$(BUILDPARAM) $(INCPATH) $(GTKPARAM) $(CAIROPARAM) -ggdb

# compiler
COMPILER=gcc

#rules
all : main

main: main.o pberr.o bcurve.o shapoid.o pbmath.o gset.o genbrush.o pbjson.o gtree.o Makefile 
	$(COMPILER) main.o pberr.o bcurve.o shapoid.o pbmath.o gset.o genbrush.o pbjson.o gtree.o $(LINKOPTIONS) -o main $(GTKLINK) $(CAIROLINK)

main.o : main.c $(PBERRDIR)/pberr.h $(SHAPOIDDIR)/shapoid.h $(SHAPOIDDIR)/shapoid-inline.c $(PBMATHDIR)/pbmath.h $(PBMATHDIR)/pbmath-inline.c $(GSETDIR)/gset.h $(GSETDIR)/gset-inline.c $(BCURVEDIR)/bcurve.h $(BCURVEDIR)/bcurve-inline.c genbrush.h genbrush-GTK.h genbrush-inline.c genbrush-inline-GTK.c Makefile
	$(COMPILER) $(BUILDOPTIONS) -c main.c

genbrush.o : genbrush.c genbrush-GTK.c genbrush.h genbrush-GTK.h genbrush-inline-GTK.c genbrush-inline.c $(BCURVEDIR)/bcurve.h $(BCURVEDIR)/bcurve-inline.c $(SHAPOIDDIR)/shapoid.h $(SHAPOIDDIR)/shapoid-inline.c $(PBMATHDIR)/pbmath.h $(PBMATHDIR)/pbmath-inline.c $(GSETDIR)/gset.h $(GSETDIR)/gset-inline.c $(PBERRDIR)/pberr.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c genbrush.c

pbjson.o : $(PBJSONDIR)/pbjson.c $(PBJSONDIR)/pbjson-inline.c $(PBJSONDIR)/pbjson.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(PBJSONDIR)/pbjson.c

gtree.o : $(GTREEDIR)/gtree.c $(GTREEDIR)/gtree.h $(GTREEDIR)/gtree-inline.c Makefile $(GSETDIR)/gset-inline.c $(GSETDIR)/gset.h $(PBERRDIR)/pberr.c $(PBERRDIR)/pberr.h
	$(COMPILER) $(BUILDOPTIONS) -c $(GTREEDIR)/gtree.c

bcurve.o : $(BCURVEDIR)/bcurve.c $(BCURVEDIR)/bcurve-inline.c $(BCURVEDIR)/bcurve.h $(SHAPOIDDIR)/shapoid.h $(SHAPOIDDIR)/shapoid-inline.c $(PBMATHDIR)/pbmath.h $(PBMATHDIR)/pbmath-inline.c $(GSETDIR)/gset.h $(GSETDIR)/gset-inline.c $(PBERRDIR)/pberr.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(BCURVEDIR)/bcurve.c

shapoid.o : $(SHAPOIDDIR)/shapoid.c $(SHAPOIDDIR)/shapoid.h $(SHAPOIDDIR)/shapoid-inline.c $(PBMATHDIR)/pbmath.h $(PBMATHDIR)/pbmath-inline.c $(GSETDIR)/gset.h $(GSETDIR)/gset-inline.c $(PBERRDIR)/pberr.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(SHAPOIDDIR)/shapoid.c

pberr.o : $(PBERRDIR)/pberr.c $(PBERRDIR)/pberr.h Makefile
	$(COMPILER) $(BUILDOPTIONS) -c $(PBERRDIR)/pberr.c

pbmath.o : $(PBMATHDIR)/pbmath.c $(PBMATHDIR)/pbmath-inline.c $(PBMATHDIR)/pbmath.h Makefile $(PBERRDIR)/pberr.h
	$(COMPILER) $(BUILDOPTIONS) -c $(PBMATHDIR)/pbmath.c

gset.o : $(GSETDIR)/gset.c $(GSETDIR)/gset-inline.c $(GSETDIR)/gset.h Makefile $(PBERRDIR)/pberr.h
	$(COMPILER) $(BUILDOPTIONS) -c $(GSETDIR)/gset.c

clean : 
	rm -rf *.o main

valgrind :
	valgrind -v --track-origins=yes --leak-check=full --gen-suppressions=yes --show-leak-kinds=all ./main
	
unitTest :
	main > unitTest.txt; diff unitTest.txt unitTestRef.txt
