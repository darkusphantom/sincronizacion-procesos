CC = gcc
CFLAGS = -Wall -g

# Lista de archivos fuente
SRCS = main.c

# Lista de archivos objeto
OBJS = $(SRCS:.c=.o)

# Nombre del ejecutable
TARGET = RUN

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(TARGET)