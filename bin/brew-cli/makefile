# On Debian / Ubuntu / Mint download SDL 2.0 and NASM from the repository:
# sudo apt-get install libsdl2-dev
# sudo apt-get install libsdl2-ttf-dev
# sudo apt-get install libsdl2-net-dev
# sudo apt-get install libsdl2-2.0-0
# sudo apt-get install libsdl2-ttf-2.0-0
# sudo apt-get install libsdl2-net-2.0-0
# sudo apt-get install nasm

ifeq ($(TARGET),brew-none-elf)
TARGET = brew
endif

TARGET ?= brew

SDL_LIB = -L/usr/include -lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_net -ldl -lm
LBITS := $(shell getconf LONG_BIT)
#LBITS = 32

VPATH = ../../src ../../include

OBJ_CORE = bbmain.o bbexec.o bbeval.o bbdata.o bbasmb.o
ifeq ($(TARGET),brew)
LIB_CORE = -lm
else
LIB_CORE = -ldl -lm -lz
endif

OBJ_SDL_EXTRA = bbcmos.o bbccli.o bbcvdu.o bbcvtx.o flood.o SDL2_gfxPrimitives.o
LIB_SDL_EXTRA = -lSDL2 -lSDL2main -lSDL2_ttf -lstb

#OBJ_CLI_EXTRA = bbccon.o bbccos.o bbcpico.c
OBJ_CLI_EXTRA = bbccon.o bbccos.o
LIB_CLI_EXTRA =

ifeq ($(TARGET),brew)
CXX = brew-none-elf-gcc
CXX_FLAGS = -Wall -I ../../include -g -Og
OBJ_CORE += bbbrew.o
OBJCOPY = brew-none-elf-objcopy
EXE_EXT = .elf
ASM_POSTFIX = _brew
else
CXX = gcc
CXX_FLAGS = -Wall -pthread -I ../../include -g -Og -D__BREW__ -U__linux__ -Wno-unused-result
OBJCOPY = objcopy
EXE_EXT =
ASM_POSTFIX = _x86_64
endif
CXX_FLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections -Xlinker -Map=bbcb.map


OBJ = $(OBJ_CORE) $(OBJ_CLI_EXTRA)
LIB = $(LIB_CORE) $(LIB_CLI_EXTRA)

ifeq ($(TARGET),brew)
all: bbcb$(EXE_EXT) bbcb.bin
else
all: bbcb$(EXE_EXT)
endif

clean:
	-rm *.o
	-rm bbcb$(EXE_EXT)
	-rm bbcb.bin
	-rm bbcb.map

build: clean all

bbmain.o: bbmain.c BBC.h
	$(CXX) $(CXX_FLAGS) -c -freg-struct-return -ffast-math -fno-finite-math-only $< -o $@

bbexec.o: bbexec.c BBC.h
	$(CXX) $(CXX_FLAGS) -c -freg-struct-return -ffast-math -fno-finite-math-only $< -o $@

bbccon.o: bbccon.c bbccon.h BBC.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

bbccos.o: bbccos.c bbccon.h BBC.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

bbpico.o: bbpico.c bbccon.h BBC.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

bbeval.o: bbeval.c BBC.h
	$(CXX) $(CXX_FLAGS) -Wno-array-bounds -c -freg-struct-return -ffast-math -fno-finite-math-only $< -o $@

bbasmb.o: bbasmb$(ASM_POSTFIX).c BBC.h
	$(CXX) $(CXX_FLAGS) -c -Os $< -o $@

bbcmos.o: bbcmos.c bbcsdl.h
	$(CXX) $(CXX_FLAGS) -Wno-unused-result -Wno-array-bounds -c -msse2 $< -o $@

bbccli.o: bbccli.c bbcsdl.h
	$(CXX) $(CXX_FLAGS) -Wno-unused-result -c $< -o $@

bbcvdu.o: bbcvdu.c bbcsdl.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

bbcvtx.o: bbcvtx.c bbcsdl.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

bbbrew.o: bbbrew.c
	$(CXX) $(CXX_FLAGS) -c $< -o $@

bbcsdl.o: bbcsdl.c bbcsdl.h version.h
	$(CXX) $(CXX_FLAGS) -Wno-unused-result -c $< -o $@

SDL2_gfxPrimitives.o: SDL2_gfxPrimitives.c SDL2_gfxPrimitives.h
	$(CXX) $(CXX_FLAGS) -c $< -o $@

SDL2_rotozoom.o: SDL2_rotozoom.c SDL2_rotozoom.h SDL2_gfxPrimitives.h SDL_stbimage.h stb_image.h dr_mp3.h
	$(CXX) $(CXX_FLAGS) -c -fPIC $< -o $@

flood.o: flood.c
	$(CXX) $(CXX_FLAGS) -c $< -o $@

ifeq ($(TARGET),brew)
bbdata.o: ../../src/bbdata_x86_64.nas
	brew-none-elf-as ../../src/bbdata_brew.s -o bbdata.o
else
ifeq ($(LBITS),64)
bbdata.o: ../../src/bbdata_x86_64.nas
	nasm -f elf64 -s ../../src/bbdata_x86_64.nas -o bbdata.o
else
bbdata.o: ../../src/bbdata_x86_32.nas
	nasm -f elf32 -s ../../src/bbdata_x86_32.nas -o bbdata.o
endif
endif

bbcb$(EXE_EXT): $(OBJ)
	$(CXX) $(CXX_FLAGS) $(OBJ) -L . -o bbcb$(EXE_EXT) $(LIB)

bbcb.bin: bbcb$(EXE_EXT)
	$(OBJCOPY) --strip-all --output-target binary bbcb$(EXE_EXT) bbcb.bin

