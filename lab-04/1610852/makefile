.PHONY: all, install, clean

TARGET = app

CC = gcc
CXX = g++

CFLAGS = -I./include
CXXFLAGS = -I./include

HEADER_SOURCES = \
	ex1.h \

C_SOURCES = main.c \
	ex1.c \

CPP_SOURCES = \

OBJS_DIR = ./build

OBJS := $(patsubst %.c, $(OBJS_DIR)/%.o, $(C_SOURCES))
OBJS += $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(CPP_SOURCES))

$(TARGET): $(OBJS)
	@echo "[LINK] $@"
	@$(CC) -o $@ $(OBJS)
	@echo "Link successfully"

$(OBJS_DIR)/%.o: %.c $(HEADER_SOURCES)
	@echo "[CC] $@"
	@mkdir -p $(OBJS_DIR)
	@$(CC) -c -o $@ $< $(CFLAGS)

$(OBJS_DIR)/%.o: %.cpp $(HEADER_SOURCES)
	@echo "[CXX] $@"
	@mkdir -p $(OBJS_DIR)
	@$(CXX) -c -o $@ $< $(CXXFLAGS)

all: $(TARGET)

install:
	cp -rf $(TARGET) /usr/local/bin

clean:
	@echo "cleaning..."
	@echo "remove ./build/"
	@rm -rf $(OBJS_DIR)
	@echo "remove $(TARGET)"
	@rm -rf $(TARGET)
	@echo "Clean successfully"