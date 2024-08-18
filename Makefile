CC = g++

CFLAGS = -std=c++20 -Wall

SRCS = $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)

TARGET = sputify.out

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)