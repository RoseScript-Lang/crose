# Variables to make the script easy to update
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
TARGET  = crose
SRC     = main.c

# The default 'target' that runs when you just type 'make'
all: $(TARGET)

# Rule to link the program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Rule to clean up the build files
clean:
	rm -f $(TARGET)
