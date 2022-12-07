#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/gba_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary data
# GRAPHICS is a list of directories containing files to be processed by grit
#
# All directories are specified relative to the project directory where
# the makefile is found
#
#---------------------------------------------------------------------------------
TARGET		:= $(notdir $(CURDIR))
BUILD		:= build
SOURCES		:= source
INCLUDES	:= include
DATA		:= font
MUSIC		:=
RESOURCES   := resources
IMAGES      := $(RESOURCES)/images
SPRITES		:= $(IMAGES)/sprites
BACKGROUNDS := $(IMAGES)/backgrounds
MAPS 		:= $(RESOURCES)/maps

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

OLVL	?= -O3

CFLAGS	:=	-g -Wall $(OLVL)\
		-mcpu=arm7tdmi -mtune=arm7tdmi\
 		-fomit-frame-pointer\
		-ffast-math \
		$(ARCH)

CFLAGS	+=	$(INCLUDE)

CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions -Wno-register

ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $*.map)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lmm -lgba


#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBGBA)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------


ifneq ($(BUILDDIR), $(CURDIR))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
SPRITEFILES :=  $(addsuffix .c, $(foreach dir,$(SPRITES),$(notdir $(basename $(wildcard $(dir)/*.png)))))
BGFILES 	:=  $(addsuffix .c, $(foreach dir,$(BACKGROUNDS),$(notdir $(basename $(wildcard $(dir)/*.png)))))
MAPFILES	:=	$(addsuffix .c, $(foreach dir,$(MAPS),$(notdir $(basename $(wildcard $(dir)/*.tmx)))))


ifneq ($(strip $(MUSIC)),)
	export AUDIOFILES	:=	$(foreach dir,$(notdir $(wildcard $(MUSIC)/*.*)),$(CURDIR)/$(MUSIC)/$(dir))
	BINFILES += soundbank.bin
endif

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_BIN := $(addsuffix .o,$(BINFILES))

export OFILES_SOURCES := $(CFILES:.c=.o) $(CPPFILES:.cpp=.o) $(SFILES:.s=.o)

export OFILES := $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD): maps
	@[ -d $@ ] || mkdir -p $@
	$(MAKE) BUILDDIR=`cd $(BUILD) && pwd` -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo cleaning...
	@rm -fr $(BUILD)/* $(TARGET).elf $(TARGET).gba

clean-bg:
	cd $(SOURCES) && rm $(BGFILES)

clean-sprites:
	cd $(SOURCES) && rm $(SPRITEFILES)

clean-maps:
	cd $(SOURCES) && rm -f $(MAPFILES)

run: $(BUILD)
	@mgba-qt $(OUTPUT).gba

debug: clean
	OLVL=" -O0" make

debugger: debug
	@echo ---------------------
	@echo Waiting for debugger!
	@echo ---------------------
	@mgba-qt --gdb $(OUTPUT).gba

#---------------------------------------------------------------------------------
# This rule processes all of the image files into source files
#---------------------------------------------------------------------------------
%.c : $(SPRITES)/%.png
	grit "$<" -ftc -gt -gB8 -Mw2 -Mh2 -o "$(SOURCES)/$(basename $@)"

sprites: clean-sprites $(SPRITEFILES)

%.c : $(BACKGROUNDS)/%.png
	grit "$<" -ftc -mLs -gt -gT'FF00E1' -gB8 -Mw2 -Mh2 -pu16 -o "$(SOURCES)/$(basename $@)"

backgrounds: clean-bg $(BGFILES)

mklevel:
	@echo building mklevel...
	@cd $(RESOURCES)/mklevel/ && make

%.c	: $(MAPS)/%.tmx
	$(RESOURCES)/mklevel/mklevel "$<" "$(SOURCES)/$(basename $@)"

maps: mklevel clean-maps $(MAPFILES)

#---------------------------------------------------------------------------------
else

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(OUTPUT).gba	:	$(OUTPUT).elf

$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SOURCES) : $(HFILES)

#---------------------------------------------------------------------------------
# The bin2o rule should be copied and modified
# for each extension used in the data directories
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# rule to build soundbank from music files
#---------------------------------------------------------------------------------
soundbank.bin soundbank.h : $(AUDIOFILES)
#---------------------------------------------------------------------------------
	@mmutil $^ -osoundbank.bin -hsoundbank.h

#---------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension
#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)



-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
