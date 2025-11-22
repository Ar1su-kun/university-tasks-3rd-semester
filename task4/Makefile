SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=)
%: %.c
	gcc $< -Wall -g -fsanitize=address -o $@

clean:
	rm -f $(TARGETS)
.PHONY: clean
