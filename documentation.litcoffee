# Mojo Picon Zombie Hunter

## Project Purpose
The primary goal of ZombieHunter is to showcase a modular approach to game development. By maintaining a strict **separation of concerns**, the project avoids monolithic code. It utilizes distinct systems for gameplay orchestration, scene management, math utilities, and saving/loading to ensure the codebase remains scalable and maintainable.

---

## System Descriptions

### Application & Core Loop
* **Entry & Application:** Manages the program lifecycle. This includes raylib window initialization, high-level context setup, and the primary game loop orchestration.

### Game Management & Scene System
* **SceneManager:** A centralized state manager that facilitates seamless switching between different game screens, such as the Main Menu, Gameplay, and HUD overlays.
* **GameManager:** The primary logic coordinator. It handles entity initialization, health monitoring, and game-over conditions while delegating specific tasks to sub-systems.

### Player & Gameplay Entities
* **Player:** Manages complex input processing, movement physics, collision detection, and shooting behavior.
* **SauceMaker:** A specialized base class for attack logic. It provides shared behavior for the player and other actors to ensure consistent combat mechanics across the board.

### Enemies & NPCs
* **Zombie:** Features basic AI logic to track and move toward the player, alongside damage-dealing and hit-detection routines.
* **Sheep:** A non-hostile entity used for testing varied mechanics and environmental interactions.

### Items & Inventory
* **Item System:** Utilizes an extensible architecture with a base `Item` class and specific subclasses (like `Item_Health`) for modular behavior.
* **Inventory:** Manages the collection and storage of items, bridging the gap between gameplay world-space and UI screen-space.

### UI & HUD
* **HUD:** Decouples visual feedback from game logic. It handles the rendering of health bars, inventory slots, and real-time on-screen feedback.

### Saving & Loading
* **SaveService:** Implements state persistence by abstracting the serialization of player progress, inventory contents, and game statistics to external files.

### Utilities
* **MojoPiconMath:** A custom math library for vector calculations, clamping, and geometric helpers to keep game code clean.
* **SpriteFilePaths:** A centralized configuration file to manage asset paths, preventing hard-coded strings throughout the logic.

---

## Code Architecture

The project is built on a hierarchical flow designed for clarity:

1.  **Application:** Initial setup and main loop.
2.  **SceneManager:** Determines which "scene" is currently active.
3.  **GameManager:** Orchestrates the active logic within the gameplay scene.
4.  **Sub-Systems:** Player, Enemies, UI, and Inventory communicate via public interfaces rather than global variables.

.
