CC		= g++
LIB_DIR	= lib
SRC_DIR	= src
OBJ_DIR	= build
LIB		= $(wildcard $(LIB_DIR)/*.hpp)
SRC		= $(wildcard $(SRC_DIR)/*.cpp)
OBJ		= $(addprefix $(OBJ_DIR)/,$(notdir $(patsubst %.cpp,%.o,$(SRC))))
LFLAGS	= -g -Wall -I$(LIB_DIR) -pthread -O5
TARGET	= main

.PHONY: clean

all: $(OBJ_DIR)/$(TARGET).o $(OBJ)
	$(CC) $(LFLAGS) $^ -o $(TARGET)

$(OBJ_DIR)/$(TARGET).o: $(TARGET).cpp $(LIB)
	$(CC) $(LFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(LIB)
	$(CC) $(LFLAGS) -c $< -o $@

run:
	make all
	./main

clean:
	rm -r build/*.* $(TARGET) 2> /dev/null || exit 0