CC = gcc
CFLAGS = -O3

TARGET = yabc

all:
	$(CC) $(CFLAGS) -o $(TARGET) yabc.c