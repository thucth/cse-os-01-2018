.PHONY: all, clean
# make all <=> make
# PHONY: conflict files

TARGET = app

HEADERS += \
	sum.h \
	sub.h

CSRCS += \
	main.c \
	sum.c \
	sub.c

CC = gcc

CFLAGS = -I.

OBJSDIR = ./build
OBJS = $(patsubst %.c, $(OBJSDIR)/%.o, $(CSRCS))
# replace pattern (pattern, replacement, array %)

$(OBJSDIR)/%.o: %.c $(HEADERS)
	mkdir -p $(OBJSDIR)
	$(CC) -c -o $@ $< $(CFLAGS)
# compile object files
# $@: truoc :
# $<: tat ca sau :

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)
# link object files

all:
	$(TARGET)

clean:
	rm -rf $(TARGET)
	rm -rf $(OBJSDIR)