# Makefile for Ray Tracing Assignment

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -O2 -g

# Libraries for different platforms
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LIBS = -lGL -lGLU -lglut -lm
endif
ifeq ($(UNAME_S),Darwin)
    LIBS = -framework OpenGL -framework GLUT -lm
endif

# Windows (MinGW)
ifdef OS
    LIBS = -lopengl32 -lglu32 -lfreeglut -lm
endif

# Source files
MAIN_SOURCE = corrected_main.cpp
HEADERS = classes.hpp bitmap_image.hpp
INTERSECTION = intersection.cpp

# Output executable
TARGET = raytracer

# Default target
all: $(TARGET)

$(TARGET): $(MAIN_SOURCE) $(HEADERS) $(INTERSECTION)
	@echo "Compiling Ray Tracing Assignment..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_SOURCE) $(LIBS)
	@echo "Compilation successful!"
	@echo "Run with: ./$(TARGET)"

# Clean target
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET) *.o *.bmp
	@echo "Clean complete!"

# Run target
run: $(TARGET)
	@echo "Starting Ray Tracer..."
	./$(TARGET)

# Test target - compile and run with a simple test
test: $(TARGET)
	@echo "Testing compilation..."
	@if [ -f scene.txt ]; then \
		echo "scene.txt found, starting test..."; \
		./$(TARGET); \
	else \
		echo "Warning: scene.txt not found!"; \
		echo "Please create scene.txt file before running."; \
	fi

# Debug target
debug: CXXFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Compile the program (default)"
	@echo "  clean   - Remove compiled files and output images"
	@echo "  run     - Compile and run the program"
	@echo "  test    - Compile and test with scene.txt"
	@echo "  debug   - Compile with debug symbols"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Controls when running:"
	@echo "  Arrow Keys    - Move camera"
	@echo "  Page Up/Down  - Move up/down"
	@echo "  1/2          - Rotate left/right"
	@echo "  3/4          - Look up/down"
	@echo "  5/6          - Tilt clockwise/counterclockwise"
	@echo "  0            - Capture ray traced image"
	@echo "  ESC          - Exit"

# Installation check
check-deps:
	@echo "Checking dependencies..."
	@command -v $(CXX) >/dev/null 2>&1 || { echo "Error: g++ not found!"; exit 1; }
	@echo "Compiler: OK"
	@ldconfig -p | grep -q libGL || echo "Warning: OpenGL libraries may not be installed"
	@ldconfig -p | grep -q libglut || echo "Warning: GLUT libraries may not be installed"
	@echo "Dependency check complete!"

.PHONY: all clean run test debug help check-deps