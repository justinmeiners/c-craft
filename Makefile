SDLINCLUDE=/usr/local/include
SDLLIB=/usr/local/lib

FLAGS=-O3
LIBS=-lSDL2 -framework OpenGL
IFLAGS=-I${SDLINCLUDE} -L${SDLLIB}

SOURCES=*.c

ccraft: ${SOURCES}
	gcc ${FLAGS} ${IFLAGS} ${LIBS} $^ -o $@
