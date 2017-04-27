CC=gcc
CXX=g++
LINKS:=-lcomctl32 -lgdi32 -luser32
OBJS=main.o MainWin.o PaintWin.o
EXENAME=APCSP_CreatePT
FLAGS+=-municode
CXXFLAGS+=-std=c++14

ifeq ($(OS),Windows_NT)
	#CCFLAGS += -D WIN32
	BINARY_ARCH=i386
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		#CCFLAGS += -D AMD64
		OBJCOPY_FORMAT=pe-x86-64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			#CCFLAGS += -D AMD64
			OBJCOPY_FORMAT=pe-x86-64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			#CCFLAGS += -D IA32
			OBJCOPY_FORMAT=pe-i386
		endif
	endif
	EXENAME:=$(EXENAME).exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OBJCOPY_FORMAT=elf
		#CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OBJCOPY_FORMAT=elf
		#CCFLAGS += -D OSX
	endif
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_M),x86_64)
		OBJCOPY_FORMAT:=$(OBJCOPY_FORMAT)64-x86-64
		BINARY_ARCH=i386
		#CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_M)),)
		OBJCOPY_FORMAT:=$(OBJCOPY_FORMAT)32-i386
		BINARY_ARCH=i386
		#CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_M)),)
		#CCFLAGS += -D ARM
		OBJCOPY_FORMAT:=$(OBJCOPY_FORMAT)32-littlearm
		BINARY_ARCH=arm
	endif
	EXENAME:=$(EXENAME)
endif

#You may need -lfltk_forms -lfltk_images -lfltk_gl

.PHONY: all clean debug all_impl

all:
	$(eval CXXFLAGS+= -O2 -DNDEBUG)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl
	strip $(EXENAME)

#before make debug, MUST make clean first!
debug:
	$(eval CXXFLAGS+= -g -DDEBUG)
	$(eval export CFLAGS)
	$(eval export CXXFLAGS)
	+$(MAKE) all_impl

all_impl: $(EXENAME)

#WHEN ADDING A NEW CLASS
#ClassName.o: precompiled_headers.h.gch common.h ClassName.cpp ClassName.h
#	$(CXX) -c $(FLAGS) $(CXXFLAGS) $(WXCXXFLAGS) ClassName.cpp -o ClassName.o
#Note that each .o file must not only depend on its .cpp file, but also on all .h files it uses

$(EXENAME): $(OBJS)
	$(CC) $(FLAGS) -o $(EXENAME) $(OBJS) $(LINKS)

clean:
	rm -f ./*.o ./$(EXENAME) ./*.d ./*.gch

main.o: main.c
	$(CC) -c $(FLAGS) main.c -o main.o

MainWin.o: MainWin.c
	$(CC) -c $(FLAGS) MainWin.c -o MainWin.o

PaintWin.o: PaintWin.c
	$(CC) -c $(FLAGS) PaintWin.c -o PaintWin.o

#$(RESOURCES).o: $(RESOURCES)
#	@echo Converting $(*)
#	objcopy -I binary -O $(OBJCOPY_FORMAT) -B $(BINARY_ARCH) \
#	--redefine-sym _binary_$*_bin_start=$* \
#	--redefine-sym _binary_$*_bin_end=$*_end \
#	--redefine-sym _binary_$*_bin_size=$*_size_sym \
#	$(*) $(@)
#	@echo "extern const char" $(*)"[];" > $(*).h
#	@echo "extern const char" $(*)_end"[];" >> $(*).h
#	echo "extern const char" $(*)_size_sym"[];" >> $(*).h
#	@echo "#define $(*)_size ((int)$(*)_size_sym)" >> $(*).h
