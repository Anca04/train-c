CC=gcc
CFLAGS=-Wall -g -Wextra

# define targets
TARGETS=tema1

build: $(TARGETS)

tema1: tema1.c
	$(CC) $(CFLAGS) tema1.c -o tema1

run: $(TARGETS)
	./$(TARGETS)

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
