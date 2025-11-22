SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=)
%: %.c
	gcc $< -Wall -o $@
clean:
	rm -f $(TARGETS)
.PHONY: clean
