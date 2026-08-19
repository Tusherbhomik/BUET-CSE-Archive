#!/bin/bash

# Ray Tracing Assignment Validation Script
echo "======================================"
echo "Ray Tracing Assignment Validator"
echo "======================================"

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

ERRORS=0
WARNINGS=0

echo -e "\n${YELLOW}Checking required files...${NC}"

# Check for main source files
if [ -f "corrected_main.cpp" ] || [ -f "*main.cpp" ]; then
    echo -e "${GREEN}✓${NC} Main source file found"
else
    echo -e "${RED}✗${NC} Main source file missing (should be STUDENTID_main.cpp)"
    ERRORS=$((ERRORS + 1))
fi

# Check for header file
if [ -f "classes.hpp" ] || [ -f "*classes.h*" ]; then
    echo -e "${GREEN}✓${NC} Header file found"
else
    echo -e "${RED}✗${NC} Header file missing (should be STUDENTID_classes.h)"
    ERRORS=$((ERRORS + 1))
fi

# Check for intersection methods
if [ -f "intersection.cpp" ] || grep -q "intersect" *.cpp 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Intersection methods found"
else
    echo -e "${RED}✗${NC} Intersection methods missing"
    ERRORS=$((ERRORS + 1))
fi

# Check for bitmap handling
if [ -f "bitmap_image.hpp" ] || [ -f "stb_image.h" ]; then
    echo -e "${GREEN}✓${NC} Bitmap handling file found"
else
    echo -e "${RED}✗${NC} Bitmap image handling missing"
    ERRORS=$((ERRORS + 1))
fi

# Check for scene file
if [ -f "scene.txt" ]; then
    echo -e "${GREEN}✓${NC} Scene file found"
else
    echo -e "${YELLOW}⚠${NC} Scene file missing - create scene.txt for testing"
    WARNINGS=$((WARNINGS + 1))
fi

# Check for Makefile
if [ -f "Makefile" ] || [ -f "makefile" ]; then
    echo -e "${GREEN}✓${NC} Makefile found"
else
    echo -e "${YELLOW}⚠${NC} Makefile missing - will need manual compilation"
    WARNINGS=$((WARNINGS + 1))
fi

echo -e "\n${YELLOW}Checking code structure...${NC}"

# Check for required classes
if grep -q "class.*Sphere" *.cpp *.h* 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Sphere class found"
else
    echo -e "${RED}✗${NC} Sphere class missing"
    ERRORS=$((ERRORS + 1))
fi

if grep -q "class.*Triangle" *.cpp *.h* 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Triangle class found"
else
    echo -e "${RED}✗${NC} Triangle class missing"
    ERRORS=$((ERRORS + 1))
fi

if grep -q "class.*Floor" *.cpp *.h* 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Floor class found"
else
    echo -e "${RED}✗${NC} Floor class missing"
    ERRORS=$((ERRORS + 1))
fi

if grep -q "class.*GeneralQuadric\|class.*General" *.cpp *.h* 2>/dev/null; then
    echo -e "${GREEN}✓${NC} General Quadric class found"
else
    echo -e "${RED}✗${NC} General Quadric class missing"
    ERRORS=$((ERRORS + 1))
fi

# Check for required functions
if grep -q "capture\|intersect" *.cpp 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Ray tracing functions found"
else
    echo -e "${RED}✗${NC} Ray tracing functions missing"
    ERRORS=$((ERRORS + 1))
fi

if grep -q "PointLight\|SpotLight" *.cpp *.h* 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Lighting classes found"
else
    echo -e "${RED}✗${NC} Lighting classes missing"
    ERRORS=$((ERRORS + 1))
fi

echo -e "\n${YELLOW}Checking OpenGL integration...${NC}"

if grep -q "glut\|GL\|glu" *.cpp *.h* 2>/dev/null; then
    echo -e "${GREEN}✓${NC} OpenGL/GLUT integration found"
else
    echo -e "${RED}✗${NC} OpenGL/GLUT integration missing"
    ERRORS=$((ERRORS + 1))
fi

if grep -q "keyboardListener\|specialKeyListener" *.cpp 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Camera controls found"
else
    echo -e "${RED}✗${NC} Camera controls missing"
    ERRORS=$((ERRORS + 1))
fi

echo -e "\n${YELLOW}Checking assignment requirements...${NC}"

# Check for proper file naming
if ls *[0-9][0-9][0-9][0-9][0-9][0-9][0-9]* >/dev/null 2>&1; then
    echo -e "${GREEN}✓${NC} Student ID prefix found in filenames"
else
    echo -e "${RED}✗${NC} Files should be prefixed with your 7-digit student ID"
    ERRORS=$((ERRORS + 1))
fi

# Check for vector operations
if grep -q "Vector3D\|cross\|dot\|normalize" *.cpp *.h* 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Vector operations implemented"
else
    echo -e "${RED}✗${NC} Vector operations missing"
    ERRORS=$((ERRORS + 1))
fi

# Check for recursion level handling
if grep -q "recursion_level\|level" *.cpp 2>/dev/null; then
    echo -e "${GREEN}✓${NC} Recursion level handling found"
else
    echo -e "${RED}✗${NC} Recursion level handling missing"
    ERRORS=$((ERRORS + 1))
fi

echo -e "\n${YELLOW}Compilation test...${NC}"

# Try to compile if possible
if [ -f "Makefile" ] && command -v make >/dev/null 2>&1; then
    echo "Attempting to compile..."
    if make clean >/dev/null 2>&1 && make >/dev/null 2>&1; then
        echo -e "${GREEN}✓${NC} Compilation successful"
        if [ -f "raytracer" ] || [ -f "raytracer.exe" ]; then
            echo -e "${GREEN}✓${NC} Executable created"
        fi
    else
        echo -e "${RED}✗${NC} Compilation failed"
        ERRORS=$((ERRORS + 1))
    fi
elif ls *.cpp >/dev/null 2>&1 && command -v g++ >/dev/null 2>&1; then
    echo "Attempting manual compilation..."
    if g++ -std=c++11 *.cpp -lGL -lGLU -lglut -lm -o test_compile 2>/dev/null; then
        echo -e "${GREEN}✓${NC} Manual compilation successful"
        rm -f test_compile
    else
        echo -e "${RED}✗${NC} Manual compilation failed"
        ERRORS=$((ERRORS + 1))
    fi
else
    echo -e "${YELLOW}⚠${NC} Cannot test compilation (missing tools)"
    WARNINGS=$((WARNINGS + 1))
fi

echo -e "\n======================================"
echo "VALIDATION SUMMARY"
echo "======================================"

if [ $ERRORS -eq 0 ]; then
    echo -e "${GREEN}✓ All critical requirements met!${NC}"
else
    echo -e "${RED}✗ $ERRORS critical issues found${NC}"
fi

if [ $WARNINGS -gt 0 ]; then
    echo -e "${YELLOW}⚠ $WARNINGS warnings (non-critical)${NC}"
fi

echo -e "\n${YELLOW}Next steps:${NC}"
if [ $ERRORS -eq 0 ]; then
    echo "1. Test with the provided scene.txt file"
    echo "2. Verify all camera controls work (keys 1-6, arrows, page up/down)"
    echo "3. Capture test images (press '0')"
    echo "4. Check image quality and compare with sample outputs"
    echo "5. Rename files with your student ID prefix"
    echo "6. Create submission zip file"
else
    echo "1. Fix the critical issues listed above"
    echo "2. Re-run this validation script"
    echo "3. Test compilation and functionality"
fi

echo -e "\n${YELLOW}Assignment Requirements Checklist:${NC}"
echo "□ File I/O and scene loading"
echo "□ Camera control (6DOF movement)"
echo "□ OpenGL rendering of basic shapes"
echo "□ Ray-object intersection (sphere, triangle, quadric, floor)"
echo "□ Phong lighting model"
echo "□ Point lights and spot lights"
echo "□ Shadow calculation"
echo "□ Recursive reflection"
echo "□ Bitmap image output"
echo "□ Memory management"
echo "□ Student ID prefixed filenames"

exit $ERRORS