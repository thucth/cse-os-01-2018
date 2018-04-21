TARGET = app

INC = -Iinclude
LIB = -lpthread

SRC = src
INCLUDE = include
OBJ = obj

CC = gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

vpath %.c $(SRC)
vpath %.h $(INCLUDE)

MAKE = $(CC) $(INC) 

OBJ_FILE = $(addprefix $(OBJ)/, mem.o main.o)
HEADER = $(wildcard $(INCLUDE)/*.h)

all: $(OBJ_FILE)
	$(MAKE) $(LFLAGS) $(OBJ_FILE) -o $(TARGET) $(LIB)

$(OBJ)/%.o: %.c ${HEADER}
	@mkdir -p $(OBJ) # create folder obj if not exist
	$(MAKE) $(CFLAGS) $< -o $@

clean:
	rm -f obj/*.o $(TARGET)
	@echo "Clean successfully"

run:
	./$(TARGET)