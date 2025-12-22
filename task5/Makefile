SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=.o)
.DEFAULT_GOAL := default
default: shell clean
shell: main.o list.o input.o tree.o exec.o
	gcc $^ -o $@ -g -Wall -Wextra -O0
exec: list.o input.o tree.o exec.o
	gcc $^ -o $@ -g -Wall -Wextra -O0
%.o: %.c %.h
	gcc -c $< -o $@ -g -Wall -Wextra -O0
clean:
	rm -f $(TARGETS)
test:
	valgrind --leak-check=full -s --show-leak-kinds=all ./shell
.PHONY: clean default test
