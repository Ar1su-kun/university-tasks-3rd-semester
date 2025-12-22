SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=.o)
.DEFAULT_GOAL := default
default: tree clean
shell: main.o list.o input.o
	gcc $^ -o $@ -g -Wall -Wextra -O0
tree: list.o input.o tree.o
	gcc $^ -o $@ -g -Wall -Wextra -O0
%.o: %.c %.h
	gcc -c $< -o $@ -g -Wall -Wextra -O0
clean:
	rm -f $(TARGETS)
.PHONY: clean default
