CC=emcc
INCLUDE=
LIB=
LIB64=
LIBFILE=
GLMLIB64=
GLMLIBFILE=
JSLIB=
CFLAGS=-std=c++14 -Os
TARGET=printf.js
myVrDir=
API_O=
.PHONY: default all clean

default: $(TARGET)
	echo ummm $(TARGET)

all: default

HEADERS=$(wildcard ../include/*.h)

%.o: ./%.cpp $(HEADERS)
	$(CC) -o $@ $< $(CFLAGS) $(INCLUDE)
	
OBJECTS=$(patsubst ./%.cpp, %.o, $(wildcard ./*.cpp))
	
.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) --bind -s DISABLE_EXCEPTION_CATCHING=0 $(OBJECTS) -o $@

clean:
	Objects $(OBJECTS) \
	Target $(TARGET) \
	Headers $(HEADERS) \
	Include $(INCLUDE)