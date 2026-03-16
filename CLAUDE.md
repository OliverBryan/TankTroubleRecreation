# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

C++ recreation of Tank Trouble 2 — a 2-player competitive tank game with maze-based combat, bullet physics, and configurable controls. Uses SFML for rendering/input, Box2D for physics, libconfig++ for settings, and a custom Log library.

The end goal is to publish on the Steam store with peer-to-peer multiplayer (via Steam's multiplayer API) and additional features from the original game.

## Build Commands

```bash
# Configure (fetches dependencies via CMake FetchContent)
cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5

# Build
cmake --build build --config Debug    # or Release

# Run
./build/Game/Debug/Game.exe           # or Release
```

Requires CMake 3.23+ and a C++20 compiler. Dependencies (SFML 2.5.1, Box2D v2.4.2, libconfig++, Log) are fetched automatically. On Windows, DLLs and `res/` are copied to the output directory by post-build steps.

Prebuilt binaries are available in `bin/Debug/` and `bin/Release/`.

## Architecture

**Game loop** (`Game/src/Main.cpp`): Fixed 60 TPS update with variable rendering. Event → tick → render cycle through `StateManager`.

**StateManager / State / Component**: Hierarchical state system. `StateManager` holds named states (e.g. "game"), each `State` holds `Component` instances. Components implement `tick()`, `render()`, and `handleEvent()`.

**Environment** (core game component): Owns the Box2D world, two `Tank` objects, bullets array, `Maze`, and `ContactListener`. Manages round lifecycle including death detection and reset timing.

**Tank**: Player-controlled entity with Box2D body. Movement via direct velocity setting (not forces). Fires bullets at calculated offset positions. Configurable keybinds (forward/back/left/right/fire) and colors per player.

**Maze**: Loads from `.dat` files (format: `gridX gridY hasLeftWall hasTopWall` per line, 0=wall present). Batch-rendered via SFML `VertexArray` with triangles. Provides wall rectangles for collision setup.

**Collisions** (`ContactListener`): Handles bullet-wall reflections (with corner phasing prevention), bullet-tank kills, and optional tank-tank/bullet-bullet collisions via Box2D contact callbacks.

**Config**: Wraps libconfig++ to parse `Game/res/settings.cfg`. Template `getSetting<T>()` accessor. Exits on parse failure.

**Resources**: Singleton texture/font cache. Loads from `res/resources.dat` manifest and `res/Roboto-Thin.ttf`.

## Physics

- Box2D world with zero gravity, scale factor of 100 (game pixels = Box2D units × 100)
- Collision categories (bitmasks): Wall `0x0001`, Bullet `0x0002`, Player1 `0x0004`, Player2 `0x0008`
- Bullet reflection uses wall normal detection with corner-case handling to prevent phasing
- `NOMINMAX` defined on Windows to avoid macro conflicts

## Configuration

`Game/res/settings.cfg` controls gameplay: keybinds, tank colors, collision toggles (`tankCollisions`, `bulletCollisions`), bullet lifetime (`bulletTime`, `infiniteBulletTime`), post-round delay, VSync, FPS logging, and debug bounds drawing.

## MapScripts

Standalone utility (`MapScripts/`) that generates random 9×9 maze `.dat` files.
