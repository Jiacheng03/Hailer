
CC := g++
RM := rm -rf

SRC_PATH := .
TARGET_PATH := ./release
TARGET := $(TARGET_PATH)/test

SRCS := $(wildcard $(SRC_PATH)/*.cpp)
OBJS := $(patsubst $(SRC_PATH)/%.cpp, ./%.o, $(SRCS))

LIBS := -lpthread
CFLAGS := -std=c++14

all: clean build

debug: clean debug_build

build:
	test -d $(TARGET_PATH) || mkdir -p $(TARGET_PATH)
	$(CC) -c $(CFLAGS) $(SRCS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBS)
	$(RM) $(OBJS)

debug_build:
	test -d $(TARGET_PATH) || mkdir -p $(TARGET_PATH)
	$(CC) -c $(CFLAGS) $(SRCS) -g
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) $(LIBS) -g
	$(RM) $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)



