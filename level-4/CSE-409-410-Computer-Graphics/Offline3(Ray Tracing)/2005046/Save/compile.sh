#!/bin/bash

echo "Compiling Ray Tracing Assignment..."
echo

# Detect OS and set appropriate libraries
UNAME_S=$(uname -s)
if [[ "$UNAME_S" == "Linux" ]]; then
    LIBS="-lGL -lGLU -lglut -lm"
    OS_NAME="Linux"
elif [[ "$UNAME_S" == "Darwin" ]]; then
    LIBS="-framework OpenGL -framework GLUT -lm"
    OS_NAME="macOS"
else
    LIBS="-lGL -lGLU -lglut -lm"
    OS_NAME="Unknown"
fi

echo "Detected OS: $OS_NAME"
echo "Using libraries: $LIBS"
echo

# Compile with appropriate flags
g++ -std=c++11 -Wall -O2 -o raytracer main.cpp $LIBS

if [ $? -eq 0 ]; then
    echo
    echo "========================================"
    echo "COMPILATION SUCCESSFUL!"
    echo "========================================"
    echo
    echo "Executable: ./raytracer"
    echo
    echo "Controls:"
    echo "  Arrow Keys: Move camera"
    echo "  Page Up/Down: Move up/down"
    echo "  1/2: Rotate left/right"
    echo "  3/4: Look up/down"
    echo "  5/6: Tilt clockwise/counterclockwise"
    echo "  0: Capture ray traced image"
    echo "  ESC: Exit"
    echo
    echo "Run with: ./raytracer"
    echo "========================================"
    echo
    
    # Make executable if it isn't already
    chmod +x raytracer
    
    echo "Ready to run! Type './raytracer' to start."
else
    echo
    echo "========================================"
    echo "COMPILATION FAILED!"
    echo "========================================"
    echo
    echo "Please check:"
    echo "1. All required files are present:"
    echo "   - main.cpp"
    echo "   - classes.hpp"
    echo "   - intersection.cpp"
    echo "   - bitmap_image.hpp"
    echo
    echo "2. OpenGL/GLUT libraries are installed:"
    
    if [[ "$UNAME_S" == "Linux" ]]; then
        echo "   Ubuntu/Debian: sudo apt-get install freeglut3-dev libglu1-mesa-dev"
        echo "   Fedora/RHEL: sudo dnf install freeglut-devel mesa-libGLU-devel"
        echo "   Arch: sudo pacman -S freeglut glu"
    elif [[ "$UNAME_S" == "Darwin" ]]; then
        echo "   macOS: Should be included with Xcode Command Line Tools"
        echo "   If missing: xcode-select --install"
    fi
    
    echo
    echo "3. g++ compiler is available:"
    echo "   Check with: g++ --version"
    echo "========================================"
fi