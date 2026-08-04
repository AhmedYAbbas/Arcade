# Arcade

A collection of classic arcade games — Pacman, Breakout, and Asteroids, with Tetris in development — built from scratch in C++ with SDL2.

[![Watch the trailer](https://img.youtube.com/vi/pxXukngprM4/hqdefault.jpg)](https://www.youtube.com/embed/pxXukngprM4)

## Games

| Game | Status | Description |
| --- | --- | --- |
| **Pacman** | Playable | Multiple levels with the classic maze, super pellets, and ghost AI. |
| **Breakout** | Playable | Three levels, each with a unique brick layout. |
| **Asteroids** | Playable | Space combat with power-ups and screen-shake on impact. |
| **Tetris** | In development | Not yet playable. |

### Engine features
- Animated sprites, buttons, and letter animations
- Color gradients
- Screen shake and other juice effects

## Project Structure

The project follows a Core/App architecture, split into two subprojects generated with [Premake 5](https://premake.github.io/):

- **Core** — a static library with engine code shared across all games (rendering, input, animation, etc.), built from `Core/`.
- **Arcade** — the game executable, built from `Arcade/`, containing each game's scenes and assets and linking against Core.

Third-party dependencies (SDL2) live under `Core/Vendor/`.

## Building

Requires [Premake 5](https://premake.github.io/) (bundled under `Vendor/Binaries/Premake/`).

### Windows
1. Clone the repository.
2. From `Scripts/`, run `Setup-Windows.bat` to generate a Visual Studio 2022 solution.
3. Open `Arcade.sln` and build the `Arcade` project.
4. Use the **Release** configuration for the best performance — **Debug** is significantly slower.

### Linux
1. Clone the repository.
2. From `Scripts/`, run `Setup-Linux.sh` to generate Makefiles via GNU Make (`gmake2`).
3. Build with `make` from the repository root (add `config=release_x64` for an optimized build).

## Controls
| Key | Action |
| --- | --- |
| Arrow keys | Move / navigate menus |
| `A` | Confirm / action |
| `S` | Cancel / back |

## License
Released into the public domain under [The Unlicense](UNLICENSE.txt).
