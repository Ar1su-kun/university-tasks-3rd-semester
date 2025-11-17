TARGETS = $(wildcard f*.c:.c=)
%: %.c
	gcc $< -Wall -g -o $@

clean:
	rm -f $(TARGETS)
