CC = gcc
CFLAGS = -g
TARGET = test
OBJS = oss.c user.c
.SUFFIXES: .c .o

.DEFAULT_GOAL : $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	/bin/rm -f *.o $(TARGET)
