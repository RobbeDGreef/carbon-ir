SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%,$(SOURCES))

CFLAGS += -I ../../include -L/home/robbe/Projects/ir/build/ -static

all: $(OBJECTS)

clean:
	rm ${OBJECTS}

%: %.c
	# Notice how we use CXX compiler instead of C compiler. We only do this because carbon is a C++ library with
	# a c api wrapper. The most is C++ code and uses the C++ standard library. This means we have to link against
	# the standard C++ library too and the easiest way to do this is to let a C++ compiler do the linking for you. 
	$(CXX) $(CFLAGS) -o $@ $< -lcarbon-ir
