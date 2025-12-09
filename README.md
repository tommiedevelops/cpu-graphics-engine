<h1 align="left">CPU Graphics Engine</h1>

3D Graphics done on the CPU, not GPU. <em>"Why?"</em>, you might be asking. There are 3 main reasons:

1. Learn Graphics, then (GPU) Programming
- Graphics Programming involves learning Graphics and GPU Programming, both pretty complicated. Building or tinkering with a CPU Graphics Engine allows you to focus on the Graphics part first. It is particularly useful for learning how Vertex and Fragment Shaders work.
2. Graphics Experiments
- Shaders are notoriously hard to debug on the GPU, and much easier on the CPU because you can't use 'print' statements or debuggers. You can also do experiments with other parts of the pipeline which are not usually accessible on real GPUs. E.g. What if the view frustum near plane was actually a curved surface (like our eyes)?
3. Train your CS Muscles
- Building a Graphics Engine requires some heavy-duty Comp Sci knowledge inlcuding DS&A, Linear Algebra, Optimisation Techniques, Multithreading, understanding of memory and more. Graphics and Game Dev is very performance sensitive work!  
   
If you want to learn how it works, I've captured my conceptual understanding in [HOW_IT_WORKS.pdf](./docs/latex/main.pdf) and the broad system design in the [Data Flow Diagram](#data-flow-diagram).

## Features
- **Mesh Loading:** Import `.obj` files for 3D geometry stored as meshes.
- **Texture Support:** Load `.png` files to create custom textures for materials.
- **Scene Management:** Create, save and manage scenes with **Camera**, **Light** and **GameObject** structures 
- **Programmable Shaders:** Define custom **Vertex** and **Fragment** shaders as C function pointers. 
- **Interactive Rendering:** Control scenes at runtime via **SDL2 event handling** (keyboard, mouse, etc.)
- **Transparent Graphics Pipeline:** A fully documented, step-by-step pipeline that mirrors modern GPU design — ideal for learning and debugging.

## Demos
<img src="thumbnail.gif" align="center" width="60%">
<p><em>Stanford Bunny rendered with Phong shading and Utah Teapot rendered with Garaud shading & a brick texture applied</em></p>

## System Design
I use an **App/Core** architecture to separate application-specific user scripting from the static core library. 

### Core Components
- **Application**: API for user to inject custom scripting in the core render loop through V-table call-backs.
- **Game Math**: A math library with little to no dependencies used throughout the engine.
- **Platform**: Abstracts system calls and calls to SDL2 to supply time data, a window and input events.
- **Asset Manager**: API for user to load Meshes, Materials and Scenes into memory
- **Scene Manager**: API for user to create, modify, save and destroy Scene structures which contain GameObjects, Camera and Light structures.
- **Renderer**: Software Implementation of the Graphics Pipeline which supports custom Vertex and Fragment Shaders as C function pointers.

### Data Flow Diagram
<img src=data-flow.svg width="800">
<p><em>Going from bottom to top, the Asset Manager is first used to load `.obj` and `.png` files from system memory into program memory. Then, the Scene Manager (optionally) uses those Assets to create Scene, Camera and GameObjects structures, which are understood by the graphics pipeline. The Renderer then uses in-built and custom shaders alongside some additional processing steps to render the Scene and rasterize to the Window. The Window then displays the final pixel values using the SDL2 interface. The Window also emits events which can be consumed by the run-time to support input-handling and other forms of interactivity</em></p>

## Installation Instructions
Start by cloning the repo

```
git clone git@github.com:tommiedevelops/cpu-graphics-engine.git
```

Then use the following commands, depending on your system   
Windows (WSL):  
```
sudo apt update  
sudo apt install build-essential -y  
sudo apt install libsdl2-dev  
```

Mac:  
```
brew install gcc make  
brew install sdl2  
```

Verify dependencies:  
```
gcc --version  
make --version  
sdl2-config --version  
```

## Using the Engine
Video coming soon.

## Things you can do with the Engine
- Implement a Geometry / Tesselation Shader
- Implement a Scene Graph datastructure for managing the Scene
- Implement a Clip Space Sutherland-Hodgman Clipping (I use World Space)
- Create a small game using the engine
- Optimize any part of the engine (Multithread, Compute Shader, DS&A)
