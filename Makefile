CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRCS = main.c graph.c tsp.c commands.c
OBJS = $(SRCS:.c=.o)
TARGET = pvv

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
