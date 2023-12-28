CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
LIBS = $(shell $(PKGCONFIG) --libs gtk4) 

SRC = main.c customcalendar.c 

OBJS = $(SRC:.c=.o)

all: calendar


%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<


calendar: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f calendar

