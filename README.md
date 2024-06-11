# Arcade Game Collection

Welcome to the Arcade game collection! This project features four classic arcade games: Tetris, Asteroids, Breakout, and Pacman. Whether you’re a fan of retro gaming or just looking for some nostalgic fun, this collection has something for everyone.

Made using SDL2 and C++.

## Controls
- Arrow keys for movement and selection.
- 'A' key for actions.
- 'S' key for cancelling.

## Features
### Tetris
- Tetris is currently under development and will be added soon.

### Pacman
- Enjoy multiple levels with all the classic features of the original Pacman game.
- Collect super pellets, avoid ghosts, and navigate through the maze.

### Breakout
- Three different levels, each with unique brick shapes.
- Break the bricks using the paddle and ball.

### Asteroids
- Blast through space, avoiding asteroids and collecting power-ups.
- Screen shake effects when hitting asteroids.

## Additional Features
- Animated sprites and buttons.
- Letter animations.
- Color gradients.
- more...

[![Arcade](https://i9.ytimg.com/vi_webp/lptdxB8ehoU/mq1.webp?sqp=CPzLoLMG-oaymwEmCMACELQB8quKqQMa8AEB-AH-CYAC0AWKAgwIABABGEMgTChlMA8%3D&rs=AOn4CLDzEm781OpOeK0Din1fBy5wXhCRtQ&retry=4)](https://youtu.be/lptdxB8ehoU "Arcade")

## Project Structure
This is a Core/App project architecture. There are two included projects - one called Core, and one called Arcade(App). Premake is used to generate project files.
Core builds into a static library and is meant to contain common code intended for use in multiple games. Arcade builds into an executable and links the Core static library.

## Building the Project
The project uses Premake 5 as the build system. (Windows only support!)
1. Open CMD and type `git clone --recursive https://github.com/AhmedYAbbas/Arcade` to clone the repository. 
2. Navigate to the `Scripts` folder.
3. Run `Setup-Windows` to set up the Visual Studio solution.
4. Build and run the solution to start playing!