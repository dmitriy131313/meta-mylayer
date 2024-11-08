EXECUTABLE=xbox360virtual_gamepad_daemon.bin

#CC:=gcc
#CXX:=g++

CFLAGS   ?=
LDFLAGS  ?=
CPPFLAGS ?=
LIBS     ?=
INCLUDE  ?=

LIBS += -L/usr/lib -lsuinput
LIBS += -L/usr/lib -lstdc++

INCLUDE += "-I."

SOURCES_C = $(wildcard *.c)
SOURCES_CPP = $(wildcard *.cpp)

OBJECTS_C   := $(SOURCES_C:%.c=%.o)
OBJECTS_CPP := $(SOURCES_CPP:%.cpp=%.o)

.PHONY : clean

$(EXECUTABLE): $(OBJECTS_CPP) $(OBJECTS_C)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

%.o: %.c
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDE) $< -o $@

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDE) $< -o $@

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDE) $< -o $@

all: $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) *.o