---
title: 'Arcade'
date: "2024-06-11"
cover: 
    image: Images/arcade.png
    alt: 'Arcade'
    caption: 'Arcade'
tags: ["C++", "SDL2", "Premake"]
categories: [Programming]
---

# Arcade Game Collection

Welcome to the Arcade game collection! This project features four classic arcade games: Tetris, Asteroids, Breakout, and Pacman. Whether you’re a fan of retro gaming or just looking for some nostalgic fun, this collection has something for everyone.

Made using SDL2 and C++.

***

## Project Structure
This is a Core/App project architecture. There are two included projects - one called Core, and one called Arcade(App). Premake is used to generate project files.
Core builds into a static library and is meant to contain common code intended for use in multiple games. Arcade builds into an executable and links the Core static library.
***
## Building the Project
The project uses Premake 5 as the build system. (Only supports x64 Windows for now!)
1. Clone the repository.
2. Navigate to the `Scripts` folder.
3. Run `Setup-Windows` to set up the Visual Studio solution.
4. Build and run the solution to start playing!
5. If you're having performance issues, make sure you're on the `Release` configuration and not the `Debug` configuration.
***

## Controls
- Arrow keys for movement and selection.
- 'A' key for actions.
- 'S' key for cancelling.
***

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


[![Watch the video](https://img.youtube.com/vi/pxXukngprM4/hqdefault.jpg)](https://www.youtube.com/embed/pxXukngprM4)

***
