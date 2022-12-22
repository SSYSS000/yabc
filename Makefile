CFLAGS = -Wall -Wextra
TARGET = yabc

release: CFLAGS += -DNDEBUG -O3
release: exec

debug: CFLAGS += -Og -g
debug: exec

exec:
	$(CC) $(CFLAGS) -o $(TARGET) yabc.c

install: release
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)
