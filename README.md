<h1 align="left">CPU Graphics Engine</h1>
<p align="left"><em>A modular, real-time CPU Graphics Engine written in C.</em></p>

This project is an educational exploration of how modern GPU graphics pipelines work. It's built mostly from scratch to simulate the core stages that power games, VFX, and real-time rendering system.

<p align="center">
  <img src="thumbnail.gif" alt="CPU Graphics Engine rendering" width="420"/><br>
  <em>Stanford Bunny rendered with Phong shading</em>
</p>

## Features
- **Mesh Loading:** Import `.obj` files for 3D geometry stored as meshes.
- **Texture Support:** Load `.png` files to create custom textures for materials.
- **Scene Management:** Create, save and manage scenes with **Camera**, **Light** and **GameObject** structures 
- **Programmable Shaders:** Define custom **Vertex** and **Fragment** shaders as C function pointers. 
- **Interactive Rendering:** Control scenes at runtime via **SDL2 event handling** (keyboard, mouse, etc.)
- **Transparent Graphics Pipeline:** A fully documented, step-by-step pipeline that mirrors modern GPU design — ideal for learning and debugging. *(See [Pipeline Diagram](#) for details.)* 

## Architecture
### Design Philosophy
To separate user scripting and backend rendering logic, the engine uses an **App/Core architecture**.

### Components
- **App Layer** – Provides the API surface for scripting, game loops, and scene logic.  
- **Core Layer** – Implements low-level graphics pipeline and system operations.  
- **Asset Manager** – API for managing lifecycle of material, mesh, scene, and texture data shared between App and Core.
- **Scene Manager** - API for handling Scene creation and update rules

### User Flow Summary

```mermaid
flowchart LR
A["Create / Load Scene</br> via Scene Manager"] --> B["Create / Load Assets</br>via Asset Manager"]
B --> C["GameObjects created and</br>added to Scene"]
C --> D["Update Scene in</br>Render Loop"]
D --> E["Core renders Scene</br>to screen"]
```


## Data Flow Diagram

```mermaid
flowchart TD

APP["**App**</br>Custom Scripting, Game Loop"]

SM["**Scene Manager**</br>Manages Scene runtime"]

AM["**Asset Manager**</br>Material, Mesh, Scene, Texture structures"]

R["**Renderer**</br>Renders Scene to FrameBuffer"]

F["**FrameBuffer**</br>Depth Test & presents to Window"]

W["**Window**</br>Handles Hardware & Events"]

APP -.Update Scene.-> SM
APP -.Load Assets.-> AM
AM -.Scene, Mesh & Material Data.-> SM
SM -.Scene Data.-> R
R -.Pixel Data.-> F
F --> W
W -.Input Events.->APP

%% ===== STYLES =====
classDef Application fill:#111827,stroke:#475569,stroke-width:1px,rx:12,ry:12;
classDef Core fill:#0b1220,stroke:#3f3f46,stroke-width:1px,rx:12,ry:12;

class APP,AM,SM Application;
class R,F,W Core;	

subgraph Application
	SM
	AM
	APP
end 

subgraph Core
	R
	W
	F
end 
    style Application fill:transparent,stroke:#4b5563,stroke-width:2px,color:#f8fafc
    style Core fill:transparent,stroke:#4b5563,stroke-width:2px,color:#f8fafc
```
<p align=left>
The architecture follows a linear data flow. User input data is first processed by the App, which updates the Scene via the Scene Manager API. The Scene Manager organizes scene structures (camera, lights, game objects) and forwards rendering data to the Renderer, which produces the final frame displayed on screen through the FrameBuffer and Window.
</p>

## Shaders Implemented (With Demos)
**TODO**
- Fragment: Garaud, Phong, Blinn-Phong, Flat, Smooth, Toon,
- Vertex: Ripple (sin), Twist, Breating / Pulsing, Fish Eye, Quantize positions 


## How it works
* See [HOW_IT_WORKS.pdf](./docs/latex/main.pdf) which details the maths, algorithms etc.

## Installation instructions

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
