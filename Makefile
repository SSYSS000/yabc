CFLAGS = -O3
TARGET = yabc

.PHONY: all

all:
	$(CC) $(CFLAGS) -o $(TARGET) yabc.c

install: all
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)
