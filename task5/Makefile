SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=.o)
shell: main.o list.o input.o
	gcc $^ -o $@ -g
tree: list.o input.o tree.o
	gcc $^ -o $@ -g
%.o: %.c %.h
	gcc -c $< -o $@ -g
clean:
	rm -f $(TARGETS)
.PHONY: clean
