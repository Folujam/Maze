<h1 align:"center">The Maze Game</h1>
# 🚦 Raycaster Traffic Game  

A retro-style **raycasting 3D maze game** built with **C** and **SDL2**, where you must navigate through a maze, avoid obstacles, and obey (or risk ignoring!) traffic lights.  

This project is a **learning experiment in computer graphics, collision detection, and gameplay systems**, and is still **in active development**. Expect rough edges and incomplete features as the world, maze layouts, and game stages are being built out.  

---

## 🎮 Gameplay Concept  

- **First-person view** powered by a Wolfenstein-like raycasting engine.  
- **Obstacles**  
  - Colliding deducts **–2 points**.  
  - Passing safely rewards **+3 points**.  
- **Traffic lights**  
  - If you move while **green**, you’re safe.  
  - If you move while **red**, you lose **–2 points**.  
- **Score system** tracks your progress as you explore.  
- **Minimap** shows the maze and object placement.  

Future plans include:  
- 🧱 Full maze layouts with floor, ceiling, and walls.  
- 🚪 Doors to next stages.  
- 🌍 More immersive 3D world-building.  

---

## 🛠️ Installation & Dependencies  

### 1. Install Required Libraries  

This project uses **SDL2** (Simple DirectMedia Layer).  
Before building the game, make sure SDL2 is installed.  

#### On Ubuntu / Debian:  
```bash
sudo apt update
sudo apt install build-essential libsdl2-dev

#### On Fedora:
sudo dnf install SDL2-devel gcc make

#### On macOS:
brew install sdl2

#### Windows:
pacman -S mingw-w64-x86_64-SDL2

clone repo, compile in Maze/src/ (include these args -lSDL2 -lm ), then run  

## ⚠️ Development Status  

- ✅ Raycasting engine (walls, minimap, obstacles) implemented.  
- ✅ Basic gameplay rules (traffic lights, score system, collisions).  
- 🔄 Obstacles visible in minimap **and** 3D.  
- 🔄 Score tracking integrated.  
- 🚧 World-building (floor, ceiling, doors, maze layout, stage progression) **incomplete**.  
- 🚧 Graphics are placeholders (solid colors, no textures yet).  

> ⚡ This game is still in **active development**. Expect bugs, missing features, and constant iteration.  

## 🧑‍💻 Author  

Built by **Jam** — a backend enthusiast exploring computer graphics and game development.  
