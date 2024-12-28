# Define the compiler and flags
CXX = clang++
CXXFLAGS = -std=c++11

# Define the target executable
TARGET = main

# Define the source files
SRCS = main.cpp

# Define the output files pattern
OUTPUT_PATTERN = output-*.wav

# Default target
all: clean $(TARGET)

# Clean up old output files
clean:
	rm -f $(OUTPUT_PATTERN) $(TARGET)

# Compile the source files
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
	@echo "Successfully compiled $(SRCS)"

# Run the program
run: $(TARGET)
	./$(TARGET) $(WAV_FILE) output-
	@echo "Program executed successfully"

# Phony targets
.PHONY: all clean run