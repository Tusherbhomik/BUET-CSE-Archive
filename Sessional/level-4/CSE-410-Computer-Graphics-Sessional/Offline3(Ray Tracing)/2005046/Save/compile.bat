@echo off
echo Compiling Ray Tracing Assignment...
echo.

rem Windows compilation with proper flags
g++ -std=c++11 -Wall -O2 -o raytracer.exe main.cpp -lopengl32 -lglu32 -lfreeglut -lm

if %ERRORLEVEL% equ 0 (
    echo.
    echo ========================================
    echo COMPILATION SUCCESSFUL!
    echo ========================================
    echo.
    echo Executable: raytracer.exe
    echo.
    echo Controls:
    echo   Arrow Keys: Move camera
    echo   Page Up/Down: Move up/down  
    echo   1/2: Rotate left/right
    echo   3/4: Look up/down
    echo   5/6: Tilt clockwise/counterclockwise
    echo   0: Capture ray traced image
    echo   ESC: Exit
    echo.
    echo Run with: raytracer.exe
    echo ========================================
) else (
    echo.
    echo ========================================
    echo COMPILATION FAILED!
    echo ========================================
    echo.
    echo Please check:
    echo 1. All required files are present:
    echo    - main.cpp
    echo    - classes.hpp  
    echo    - intersection.cpp
    echo    - bitmap_image.hpp
    echo 2. OpenGL/GLUT libraries are installed
    echo 3. MinGW/g++ is properly configured
    echo.
    echo For help, visit: https://www.opengl-tutorial.org/miscellaneous/building-your-own-c-application/
    echo ========================================
)

pause