
all: send recv

send: send.c
	gcc $^ -o $@ -lpthread

recv: recv.c
	gcc $^ -o $@ -lpthread

clean:
	-rm send recv send

.PHONY: all
