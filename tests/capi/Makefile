SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%,$(SOURCES))

CFLAGS += -I ../../include -L/home/robbe/Projects/ir/ -static

all: $(OBJECTS)

clean:
	rm ${OBJECTS}

%: %.c
	$(CC) $(CFLAGS) -o $@ $< -lcarbon-ir -lstdc++
