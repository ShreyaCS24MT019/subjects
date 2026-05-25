CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS_TASK1 = -lssl -lcrypto
LDFLAGS_TASK2 = -lssl -lcrypto -pthread

TASK1_SRC = blockchain_task1.cpp
TASK1_TARGET = blockchain_task1

TASK2_SRC = blockchain_task2.cpp
TASK2_TARGET = blockchain_task2

all: task1 task2

task1: $(TASK1_SRC)
	$(CXX) $(CXXFLAGS) -o $(TASK1_TARGET) $(TASK1_SRC) $(LDFLAGS_TASK1)

task2: $(TASK2_SRC)
	$(CXX) $(CXXFLAGS) -o $(TASK2_TARGET) $(TASK2_SRC) $(LDFLAGS_TASK2)

run_task1: task1
	./$(TASK1_TARGET)

run_task2: task2
	./$(TASK2_TARGET)

clean:
	rm -f $(TASK1_TARGET) $(TASK2_TARGET)

.PHONY: all task1 task2 run_task1 run_task2 clean