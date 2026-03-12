# TankTroubleRecreation

A C++ recreation of the classic Tank Trouble 2 web game, built with SFML. Prebuilt windows binaries are available in the repo, or the code can be built from scratch using the instructions below.

---

## Prerequisites

Before building, make sure you have the following installed:

- **CMake 3.23+** — [cmake.org/download](https://cmake.org/download/)
- **Git** — required by CMake's FetchContent to download dependencies
- **A C++20-compatible compiler:**
  - Windows: [Visual Studio 2022](https://visualstudio.microsoft.com/) (with "Desktop development with C++" workload) or MinGW-w64
  - Linux/macOS: GCC 10+ or Clang 12+

> **Note:** All library dependencies (SFML, Box2D, libconfig, Log) are fetched automatically by CMake at configure time. You do not need to install them manually.

---

## Build Instructions

### 1. Clone the repository

```bash
git clone https://github.com/OliverBryan/TankTroubleRecreation.git
cd TankTroubleRecreation
```

### 2. Configure the project

```bash
cmake -S . -B build
```

This will download all dependencies (SFML, Box2D, libconfig++, Log). It may take a few minutes on first run.

Visual Studio users can alternatively open the root folder directly in Visual Studio (File → Open → Folder) — it will detect CMakeLists.txt automatically.

### 3. Build

```bash
cd build

# Debug build
cmake --build . --config Debug

# Release build
cmake --build . --config Release
```

### 4. Run

After a successful build, the executable and all required DLLs/resources are placed together in:

```
build/Game/Debug/Game.exe      (Debug build)
build/Game/Release/Game.exe    (Release build)
```


> **Windows note:** The post-build step automatically copies all required `.dll` files alongside the executable, so you can run it directly without any additional setup.

---

## Controls

Controls are configurable in `Game/res/settings.cfg`. The defaults are:

| Action | Player 1 | Player 2 |
|--------|----------|----------|
| Move Forward | `↑` | `E` |
| Move Back | `↓` | `D` |
| Turn Left | `←` | `S` |
| Turn Right | `→` | `F` |
| Fire | `M` | `Q` |

---

## Troubleshooting

**CMake can't find Git / FetchContent fails**
Make sure `git` is on your PATH. Run `git --version` to verify.

**Build fails on Windows with "could not find compiler"**
Open the Developer Command Prompt for Visual Studio, or pass the generator explicitly:
```bash
cmake -S . -B build -G "Visual Studio 17 2022"
```

**Missing DLLs on Windows**
Make sure you're running `Game.exe` from inside `build/Game/Debug/` or `build/Game/Release/`, not from the repo root. The post-build step copies DLLs there.
