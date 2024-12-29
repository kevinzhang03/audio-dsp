# Define the compiler and flags
CXX = clang++
CXXFLAGS = -std=c++11 -Wall

# Define the target executable
TARGET = main.out

# Define the source files
SRCS = main.cpp

# Default target
all: clean $(TARGET)

# Clean up old output files
clean:
	rm -f output-*.wav $(TARGET)
	rm -rf ./output-wav

# Compile the source files
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
	@echo "Successfully compiled $(SRCS)"

# Run the program
run: $(TARGET)
	./$(TARGET) $(WAV_FILE) ${OUTPUT_DIR}
	@echo "Program executed successfully"

# Phony targets
.PHONY: all clean run
