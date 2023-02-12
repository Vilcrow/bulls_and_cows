CC = gcc
CFLAGS = -Wall -g
SRCMODULES = bulls_and_cows.c
OBJMODULES = $(SRCMODULES:.c=.o)

%.o: %.c %.h
			$(CC) $(CFLAGS) -c $< -o $@

bulls_and_cows: $(OBJMODULES)
				$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
		$(CC) -MM $^ > $@

run: bulls_and_cows
				./bulls_and_cows

clean:
	rm *.o bulls_and_cows

