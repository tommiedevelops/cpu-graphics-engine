.obj models: https://drive.google.com/drive/folders/1wxrPdNxu02eeTgZSTUKd_N_un35_3PKn?usp=drive_link

WSL:
git clone git@github.com:tommiedevelops/c-software-rasterizer.git

Installing dependencies: 

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

Commands to switch branches:

git checkout remotes/origin/tech-talk/depth-demo
git checkout remotes/origin/tech-talk/material-demo
git checkout remotes/origin/tech-talk/phong-shading-demo
git checkout remotes/origin/tech-talk/projection-demo
git checkout remotes/origin/tech-talk/rotation-demo
git checkout remotes/origin/tech-talk/texture-demo
git checkout remotes/origin/tech-talk/triangle-rasterization
git checkout remotes/origin/tech-talk/wireframe-demo
