# Computer Graphics Sessional (CSE 410)

![Language](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![Library](https://img.shields.io/badge/library-OpenGL%20%7C%20GLUT-orange.svg)
![Status](https://img.shields.io/badge/status-Completed-green.svg)

This repository contains my implementation of the **Computer Graphics Sessional course (CSE 410)**.  
The projects range from basic OpenGL simulations to building a complete **Rasterization pipeline** and a **Recursive Ray Tracer** from scratch using **C++**.

---

## 📂 Project Modules

### 1. OpenGL Simulation & Animation (Offline 1)

Implemented 3D simulations and fully controllable camera movements using the **OpenGL (GLUT)** library.

#### ✨ Features
- **Fully Controllable Camera**  
  Translation and rotation in 3D space (Pitch, Yaw, Roll).
- **3D Bouncing Ball**  
  Physics-based simulation of a ball bouncing inside a cube with gravity, velocity decay, and collision detection.
- **Analog Clock**  
  Real-time clock animation synchronized with system time.

| Camera Demo | Bouncing Ball | Analog Clock |
| :---: | :---: | :---: |
| ![Camera Demo](Offline1(OpenGL)/demo.gif) | ![Ball Demo](Offline1(OpenGL)/balldemo.gif) | ![Clock Demo](Offline1(OpenGL)/clock.gif) |
| *2d axis rotation* | *Physics-based collision & gravity* | *Real-time synchronization* |

---

### 2. Rasterization Pipeline (Offline 2)

Developed a **software-based graphics pipeline from scratch (without OpenGL)** to understand how 3D scenes are rendered onto a 2D screen.

#### 🧠 Core Concepts
- **Modeling Transformation**  
  Translation, rotation (Rodrigues’ formula), and scaling matrices.
- **View & Projection Transformation**  
  Manual implementation of `gluLookAt` and `gluPerspective`.
- **Hidden Surface Removal**  
  Implemented the **Z-buffer algorithm** for depth testing and clipping.

#### 🖼 Output
- Generates a `.bmp` image
- Stores Z-buffer depth values

| Rendered Output |
| :---: |
| ![Raster Output](Offline2(Resterization)/out.bmp) |
| *Generated using custom Z-buffer algorithm* |

---

### 3. Ray Tracing (Offline 3)

Implemented a **Recursive Ray Tracer** to generate realistic images with advanced lighting and shading effects.

#### 🌟 Features
- **Phong Lighting Model**  
  Ambient, Diffuse, and Specular reflection components.
- **Recursive Reflection**  
  Multi-level reflections for mirror-like surfaces.
- **Shadows**  
  Shadow ray casting to detect occluded pixels.
- **Texture Mapping**  
  Applied textures (e.g., checkerboard floor) to geometric surfaces.

| Ray Tracing Demo | Texture Mapping |
| :---: | :---: |
| ![Ray Tracer](Offline3(Ray%20Tracing)/Output_1.bmp) | ![Texture](Offline3(Ray%20Tracing)/texture2_texture.bmp) |
| *Spheres with reflection & Phong lighting* | *Texture mapping on floor plane* |

---

## 🚀 How to Run

### 🔧 Prerequisites

**Linux**
- `g++`
- `freeglut3-dev`
- `libglu1-mesa-dev`
- `mesa-common-dev`

**Windows**
- Visual Studio or MinGW
- GLUT headers and libraries

---

### 🐧 Installation (Linux)

```bash
sudo apt-get update
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

---

### ▶️ Running Offline 1 (OpenGL)

```bash
cd "Offline1(OpenGL)"
g++ main.cpp -o demo -lglut -lGLU -lGL
./demo
```

---

### ▶️ Running Offline 2 (Rasterization)

```bash
cd "Offline2(Resterization)/2005046"
g++ 2005046.cpp -o main
./main
```

---

### ▶️ Running Offline 3 (Ray Tracer)

```bash
cd "Offline3(Ray Tracing)"
# Note: Ensure scene.txt is present in this directory
g++ 2005046_main.cpp 2005046_intersection.cpp -o raytracer -lglut -lGLU -lGL
./raytracer
```

---

## 📜 License

This project is developed **for educational purposes** as part of the  
**CSE 410 – Computer Graphics Sessional** course at **BUET**.

