# TankTroubleRecreation

A C++ recreation of the classic Tank Trouble 2 web game, built for Steam. The goal is to faithfully recreate the original game, add Steam peer-to-peer multiplayer, polish the experience, and publish on the Steam store.

Built with SFML for rendering/input, Box2D for physics, libconfig++ for configuration, and a custom Log library. Prebuilt Windows binaries are available in the repo, or the code can be built from source using the instructions below.

---

## Prerequisites

Before building, make sure you have the following installed:

- **CMake 3.23+** — [cmake.org/download](https://cmake.org/download/)
- **Git** — required by CMake's FetchContent to download dependencies
- **A C++20-compatible compiler:**
  - Windows: [Visual Studio 2022+](https://visualstudio.microsoft.com/) (with "Desktop development with C++" workload — this also bundles CMake) or MinGW-w64
  - Linux/macOS: GCC 10+ or Clang 12+

> **Windows tip:** On Windows, you must run build commands from the **Developer Command Prompt** (or Developer PowerShell) that comes with Visual Studio, not regular PowerShell, so that the compiler and CMake are on your PATH. Search your Start menu for "Developer Command Prompt" or "Developer PowerShell".

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
cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5
```

> The `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` flag is required because the bundled SFML version predates newer CMake policy requirements.

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

## Running Prebuilt Binaries

If you just want to play without building from source, prebuilt Windows binaries are available:

```bash
cd bin/Release
./Game.exe
```

> **Important:** Run `Game.exe` from inside its directory — it looks for `./res/` relative to the working directory.

---

## Troubleshooting

**`cmake` is not recognized**
You need to run commands from the Developer Command Prompt/PowerShell that comes with Visual Studio, not regular PowerShell. Search your Start menu for "Developer Command Prompt" or "Developer PowerShell".

**CMake policy error mentioning `cmake_minimum_required`**
Add `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` to your configure command (see step 2 above).

**CMake can't find Git / FetchContent fails**
Make sure `git` is on your PATH. Run `git --version` to verify.

**Build fails on Windows with "could not find compiler"**
Open the Developer Command Prompt for Visual Studio, or pass the generator explicitly:
```bash
cmake -S . -B build -G "Visual Studio 17 2022"
```

**IO error reading settings.cfg**
You're running `Game.exe` from the wrong directory. Run it from inside `build/Game/Release/` (or `Debug/`), or `bin/Release/`, where the `res/` folder is present.

**Missing DLLs on Windows**
Make sure you're running `Game.exe` from inside `build/Game/Debug/` or `build/Game/Release/`, not from the repo root. The post-build step copies DLLs there.
