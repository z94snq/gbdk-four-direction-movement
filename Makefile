#
# Simple Makefile that compiles all .c and .s files in the same folder
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
GBDK_HOME = C:/gbdk/

LCC = $(GBDK_HOME)bin/lcc 

# You can uncomment the line below to turn on debug output
# LCC = $(LCC) -debug

# You can set the name of the .gb ROM file here
PROJECTNAME    = rom

BINS	    = $(PROJECTNAME).gb
CSOURCES   := $(wildcard src/*.c) $(wildcard src/graphics/*.c)
ASMSOURCES := $(wildcard *.s)

all:	$(BINS)

# Compile and link all source files in a single call to LCC
$(BINS):	$(CSOURCES) $(ASMSOURCES)
	$(LCC) -Iheaders -Wm-yC -o $@ $(CSOURCES) $(ASMSOURCES)

clean:
ifeq ($(OS),Windows_NT)
	del /Q *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm 2>nul || exit 0
else
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm
endif

