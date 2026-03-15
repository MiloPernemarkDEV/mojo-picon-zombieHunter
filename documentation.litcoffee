# Mojo Picon Zombie Hunter

## System Descriptions

### Application & Core Loop
* **Entry & Application:** Manages the program lifecycle. This includes raylib window initialization, high-level context setup, and the primary game loop orchestration.

### Game Management & Scene System
* **SceneManager:** A centralized state manager that calls all special functions.
* **Singleton GameManager:** The primary gameplay coordinator handling things such as enemy waves. 

### Player & Gameplay Entities
* **Player:** Manages complex input processing, movement physics, collision detection, and shooting behavior.
* **SauceMaker:** A specialized base class similar to an entity class.

### Enemies & NPCs
* **Zombie:** Features basic AI logic to track and move toward the player, alongside damage-dealing and hit-detection routines.
* **Sheep:** A non-hostile entity used for testing varied mechanics and environmental interactions.

### Items & Inventory
* **Item System:** Utilizes an extensible architecture with a base `Item` class and specific subclasses (like `Item_Health`) for modular behavior.
* **Inventory:** Manages the collection and storage of items, bridging the gap between gameplay world-space and UI screen-space.

### UI & HUD
* **HUD:** Decouples visual feedback from game logic. It handles the rendering of health bars, inventory slots, and real-time on-screen feedback.

### Saving & Loading
* **SaveService:** Implements state persistence by serializing and deserializing data with C++ I/O libraries.

### Utilities
* **MojoPiconMath:** A custom math library for vector calculations, clamping, and geometric helpers to keep game code clean.
* **SpriteFilePaths:** A centralized configuration file to manage asset paths, preventing hard-coded strings throughout the logic.
