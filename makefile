CC = gcc
CFLAGS = -Wall -g

# Lista de archivos fuente
SRCS = main.c file1.c file2.c

# Lista de archivos objeto
OBJS = $(SRCS:.c=.o)

# Nombre del ejecutable
TARGET = mi_programa

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(TARGET)