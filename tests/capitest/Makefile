SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%,$(SOURCES))

CFLAGS += -I ../../include -L/home/robbe/Projects/ir/build/ -static

all: $(OBJECTS)

clean:
	rm ${OBJECTS}

%: %.c
	$(CXX) $(CFLAGS) -o $@ $< -lcarbon-ir
