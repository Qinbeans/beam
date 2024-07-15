target = main
cppstd = -std=c++2a
cflags = -Iinclude ${cppstd}
lflags =
cxx = g++
build_dir = build
objects_dir = build/objects
game_objects_dir = ${objects_dir}/game_objects
# wild card game_objects/*.cpp recursively add all .cpp files in this directory and its subdirectories
objects = $(patsubst %.cpp, %.o, $(wildcard game_objects/*.cpp))

OS := $(shell uname)
ifeq ($(OS),Windows_NT)
    target = main.exe
	lflags = -Llib/win -lraylib -lopengl32 -lgdi32 -lwinmm -lshell32
	cxx = clang++
endif
ifeq ($(OS),MSYS_NT-10.0-22631)
	target = main.exe
	lflags = -Llib/win -lraylib -lopengl32 -lgdi32 -lwinmm -lshell32
	cxx = clang++
endif
ifeq ($(OS),Darwin)
    lflags = -Llib/mac -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    cxx = clang++
endif
ifeq ($(OS),Linux)
    lflags = -Llib/linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# if DEBUG is defined, add debug flags
ifeq ($(DEBUG),1)
	cflags += -g -DDEBUG
endif

.PHONY: all clean run archive

all: main

main: mkdir_build main.o event.o ${objects}
	${cxx} ${cflags} -o ${build_dir}/${target} ${objects_dir}/*.o  ${game_objects_dir}/*.o ${lflags}

main.o: main.cpp
	${cxx} ${cflags} -c main.cpp -o ${objects_dir}/main.o

event.o: event/event.cpp
	${cxx} ${cflags} -c event/event.cpp -o ${objects_dir}/event.o

game_objects/%.o: game_objects/%.cpp
	${cxx} ${cflags} -c $< -o ${objects_dir}/$@

mkdir_build:
	mkdir -p ${objects_dir}/game_objects

run: main
	${build_dir}/${target}

archive:
	tar -czvf game2.tar.gz .vscode/ include/ lib/ makefile main.cpp event/ game_objects/ .clangd readme.md assets/

clean:
	rm -rf ${build_dir}