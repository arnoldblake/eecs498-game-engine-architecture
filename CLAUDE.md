# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an **EECS498 Game Engine** - a C++20 text-based game engine project with SDL3 integration. The engine loads game scenes from JSON files, manages actors/entities using a template system, and runs a main game loop (Render → Input → Update).

## Build and Development

### Build Commands

```bash
# Initial setup (clone with submodules)
git clone --recursive https://github.com/[repo]/eecs498-game-engine.git
cd eecs498-game-engine

# Configure (Windows/MSVC)
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"

# Build
cmake --build . --config Release

# Run
./Release/eecs498-game-engine.exe
```

### Key Build Details

- **Minimum CMake:** 3.16
- **C++ Standard:** C++20 (`-std=c++20`)
- **Dependencies:** SDL3 (graphics), GLM (math), RapidJSON (JSON parsing) - all included as git submodules or headers
- **Output:** Executable copies `resources/` directory to build folder (required for game data)
- **Platform:** Primary Windows support; Linux/Mac supported via conditional compilation (`#ifdef _WIN32`)

### Clangd Configuration

The `.clangd` file already exists and is configured for C++20. Language server support is enabled with proper include paths.

## Architecture and Code Organization

### Core Components

**Engine (src/Engine.hpp, src/Engine.cpp)**
- Central game loop: `GameLoop()` → Render() → Input() → Update()
- Manages `actors` vector, collision detection, dialogue processing
- Stores game state: `health`, `score`, `hardcoded_map[50][26]`
- Loads configs from `resources/game.config` and `resources/rendering.config` (JSON)

**SceneDB (src/SceneDB.hpp, src/SceneDB.cpp)**
- Loads scenes from JSON files in `resources/scenes/`
- Parses actor definitions and creates Actor instances using TemplateDB
- Scene format: JSON with `actors` array containing template references and position overrides

**TemplateDB (src/TemplateDB.hpp, src/TemplateDB.cpp)**
- Loads reusable actor templates from `resources/actor_templates/*.json`
- Provides actor definitions to SceneDB (view character, blocking, dialogue, etc.)
- Template format: JSON with `view`, `x`, `y`, `blocking`, and optional dialogue fields

**Actor (src/Engine.hpp)**
- Represents game entities with: `id`, `actor_name`, `view` (char), `position` (glm::ivec2), `velocity`, `blocking` flag
- Two dialogue types: `nearby_dialogue` (triggered when adjacent), `contact_dialogue` (triggered on same tile)
- Dialogue can contain special commands: `"health down"`, `"score up"`, `"you win"`, `"game over"`, `"proceed to [N]"` (load level_N.scene)

### Data Flow

```
main.cpp → Engine::GameLoop()
  ├── Load game.config (game metadata)
  ├── Load rendering.config (viewport size)
  ├── TemplateDB::loadTemplates() → resources/actor_templates/
  ├── SceneDB::loadScene() → resources/scenes/[scene].scene
  │   └── Create Actors from templates + scene overrides
  └── Loop: Render() → Input() → Update()
```

### Game Map and Collision

- **Map dimensions:** 50 wide × 26 tall (hardcoded character grid)
- **Blocking tiles:** `'b'` marks walls; out-of-bounds treated as open
- **Collision detection:** `movement_blocked()` checks for walls and blocking actors
- **Rendering:** Camera centers on player; viewport size from `rendering.config`

## Configuration and Data Files

All configuration files are JSON format and located in `resources/`:

### game.config
```json
{
  "initial_scene": "level_0.scene",
  "game_start_message": "...",
  "game_over_good_message": "...",
  "game_over_bad_message": "..."
}
```

### rendering.config
```json
{
  "x_resolution": 800,
  "y_resolution": 600
}
```

### Scene Files (resources/scenes/\*.scene)
```json
{
  "actors": [
    {
      "template": "player",
      "name": "player",
      "x": 25, "y": 13,
      "view": "@",
      "velocity": {"x": 0, "y": 0},
      "blocking": true,
      "nearby_dialogue": "...",
      "contact_dialogue": "..."
    }
  ]
}
```

### Actor Templates (resources/actor_templates/\*.json)
```json
{
  "view": "n",
  "x": 0, "y": 0,
  "blocking": false,
  "nearby_dialogue": "Hello!",
  "contact_dialogue": "Hi there!"
}
```

## Common Development Tasks

### Adding a New Actor Type
1. Create `resources/actor_templates/[actor_name].json` with `view`, `x`, `y`, `blocking` fields
2. Reference in scene JSON: `"template": "[actor_name]"` with position override `x`, `y`

### Adding a New Scene/Level
1. Create `resources/scenes/level_N.scene` with actor definitions
2. Reference in dialogue: `"proceed to [N]"` triggers loading
3. Update `game.config` `initial_scene` if it's the starting level

### Modifying Game Loop Logic
- **Rendering:** `Engine::Render()` - outputs map, actors, dialogue
- **Input processing:** `Engine::Input()` - reads commands (n/s/e/w/quit)
- **State updates:** `Engine::Update()` - moves actors, checks collisions, processes dialogue

### Adding New Dialogue Commands
Edit `Engine::find_dialogue_command()` to parse new special commands beyond: `health down`, `score up`, `you win`, `game over`, `proceed to [N]`

## Dependencies and Third-Party Code

- **SDL3 (git submodule):** Graphics, window management, input
- **SDL_ttf, SDL_mixer, SDL_image (git submodules):** Font rendering, audio, image loading
- **GLM (src/include/glm/):** Vector/matrix math (header-only)
- **RapidJSON (src/include/rapidjson/):** JSON parsing (header-only)

## Important Implementation Notes

- **Working Directory:** `src/main.cpp` sets working dir to executable location (Windows) to locate resources
- **JSON Parsing:** Uses RapidJSON (no exceptions); check parse errors with `.HasParseError()`
- **Actor IDs:** Auto-assigned by `SceneDB::next_actor_id` (static counter)
- **Map Storage:** Initialized with `memset()` and `'b'` for walls
- **No Physics:** Simple velocity-based movement; actors bounce off walls
- **No Audio/Graphics:** Currently terminal-only; SDL3 is integrated but not used for rendering

## Known Limitations

- Hardcoded map dimensions (50×26)
- No formal test suite
- Terminal-based rendering (no graphics output)
- Simple tile-based collision detection
- Single-threaded execution
