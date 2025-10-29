<h1 align="left">CPU Graphics Engine</h1>
<p align="left"><em>A modular, real-time CPU Graphics Engine written in C.</em></p>

This project is designed as an educational exploration of how modern GPU graphics pipelines work — built mostly from scratch to simulate the core stages that power games, VFX, and real-time rendering system

<p align="center">
  <img src="thumbnail.gif" alt="CPU Graphics Engine rendering" width="420"/><br>
  <em>Stanford Bunny rendered with Phong shading on the CPU.</em>
</p>

---

## Features
- **Mesh Loading:** Import `.obj` files for 3D geometry stored as meshes.
- **Texture Support:** Load `.png` files to create custom textures for materials.
- **Scene Management:** Create, save and manage scenes with **Camera**, **Light** and **GameObject** structures 
- **Programmable Shaders:** Define custom **Vertex** and **Fragment** shaders as C function pointers. 
- **Interactive Rendering:** Control scenes at runtime via **SDL2 event handling** (keyboard, mouse, etc.)
- **Transparent Graphics Pipeline:** A fully documented, step-by-step pipeline that mirrors modern GPU design — ideal for learning and debugging. *(See [Pipeline Diagram](#) for details.)* 

## Architecture Diagram
```mermaid
flowchart LR
    A[User Input] --> B[Scene Manager]
    B --> C[Camera]
    B --> D[Light]
    B --> E[GameObject]

    E --> F[Renderer]
    F --> G[Vertex Shader]
    G --> H[Clipping & Projection]
    H --> I[Rasterizer]
    I --> J[Fragment Shader]
    J --> K[Framebuffer]
    K --> L[SDL2 Window Output]

    style B stroke:#333,stroke-width:1px
```

## Shaders Implemented (With Demos)
**TODO**
- Fragment: Garaud, Phong, Blinn-Phong, Flat, Smooth, Toon,
- Vertex: Ripple (sin), Twist, Breating / Pulsing, Fish Eye, Quantize positions 


# How it works
* See HOW_IT_WORKS.pdf which details the maths, algorithms etc.

# Installation instructions

1. Clone the reposiotry
2. Ensure you have dependencies installed
3. Run the following commands to get started
4. Instructions for Custom Scenes, Shaders, .obj files, .png files, etc.

Windows (WSL):  
sudo apt update  
sudo apt install build-essential -y  
sudo apt install libsdl2-dev  

Mac:  
brew install gcc make  
brew install sdl2  

Verify dependencies:  
gcc --version  
make --version  
sdl2-config --version  
