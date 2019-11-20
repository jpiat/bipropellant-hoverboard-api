SRC_DIR := ./
SRC_DIR_PROTOCOL := ./hbprotocol
INC_DIR := ./
INC_DIR_PROTOCOL := ./hbprotocol
OBJ_DIR := ./obj
OBJ_DIR_TEST := ./obj_tests
SRC_FILES_CPP := $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES_C := $(wildcard $(SRC_DIR)/*.c)
SRC_FILES_PROTOCOL_C := $(wildcard $(SRC_DIR_PROTOCOL)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES_CPP))
OBJ_FILES += $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES_C))
OBJ_FILES += $(patsubst $(SRC_DIR_PROTOCOL)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES_PROTOCOL_C))
CPPFLAGS := -I$(INC_DIR) -I$(INC_DIR_PROTOCOL) -O2 -g -std=c++11 -fPIC
CFLAGS := -I$(INC_DIR) -I$(INC_DIR_PROTOCOL) -O2 -g -fPIC


default: libhoverboardapi.a 

clean:
	rm -Rf $(OBJ_DIR)
	rm -Rf $(OBJ_DIR_TEST)
	rm libhoverboardapi.a

libhoverboardapi.a : ${OBJ_FILES}
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR_PROTOCOL)/%.c
	mkdir -p $(OBJ_DIR)
	gcc $(CFLAGS) -c -o $@ $<
