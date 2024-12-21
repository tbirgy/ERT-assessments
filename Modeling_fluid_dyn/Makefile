CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lm

TARGET = wave_transport.exe
SOURCE = wave_transport.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	del $(TARGET)

.PHONY: all clean
