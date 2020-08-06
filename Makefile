#OBJS specifies which files to compile as part of the project
OBJS = src/*.c lib/stds/src/*.c lib/stds/lib/structures/src/*.c

#CC specifies which compiler we're using
CC = gcc

#INCLUDE_PATHS specifies the additional include paths we'll need

#LIBRARY_PATHS specifies the additional library paths we'll need

ifeq ($(OS), Windows_NT)
	INCLUDE_PATHS = -IC:\MinGW\include\SDL2
	LIBRARY_PATHS = -LC:\MinGW\lib
endif 

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresszzes all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -Werror -Wfloat-conversion

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = 0

ifeq ($(OS), Windows_NT)
	LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
else
	LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
endif

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Game

#This is the target that compiles our executable
#
# -g -O -c generates .o files.
# -shared -o
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)