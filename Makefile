CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -L. -Wl,-rpath=.
LIBS = -lPocoNetSSL -lPocoNet -lPocoJSON -lPocoFoundation


SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
MAIN_SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)


LIB_NAME = libbranch_comparator.so
BIN_NAME = branch_comparator

ARGS ?= sisyphus p10

.PHONY: all clean run

all: $(LIB_NAME) $(BIN_NAME)

$(LIB_NAME): $(OBJ)
	$(CXX) -shared -o $@ $^ $(LIBS)

$(BIN_NAME): $(MAIN_OBJ) $(LIB_NAME)
	$(CXX) $(LDFLAGS) -o $@ $< -lbranch_comparator $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -fPIC -c -o $@ $<

run: all
	@echo "Running with arguments: $(ARGS)"
	@LD_LIBRARY_PATH=. ./$(BIN_NAME) $(ARGS)

clean:
	rm -f $(OBJ) $(MAIN_OBJ) $(LIB_NAME) $(BIN_NAME)