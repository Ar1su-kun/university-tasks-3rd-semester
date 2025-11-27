SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=)
all: $(TARGETS)
%: %.c
	gcc $< -Wall -o $@
clean:
	rm -f $(TARGETS)
.PHONY: all clean
