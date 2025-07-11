# VUEDK Framework

## Description
VUEDK is a modular framework for Unreal Engine, designed to provide ready-to-use systems that are easy to integrate into your game projects. Each system is contained in its own folder and can be used independently or in combination with others.

## Installation and Setup
1. Copy the `VUEDK` folder into your project's `Plugins` directory.
2. Open your project with Unreal Engine. The plugins will be detected automatically.
3. Enable the desired plugins from the Unreal Engine plugins menu.

## How to Use
- Each system is independent and can be enabled or disabled as needed.
- Inside each plugin folder, you will find a `README.md` file with detailed instructions on how to use and integrate that specific system.

## Folder Structure and Challenges
- Each subfolder represents a distinct system/plugin (e.g., `AISystem`, `BetterUI`, `InventorySystem`, etc.).
- Inside each folder you will find:
  - A `.uplugin` file for plugin configuration
  - `Source/`, `Content/`, `Resources/` folders for code, assets, and resources
  - A `README.md` file with specific documentation
- The modularity allows you to include only the systems you need, but be mindful of dependencies between plugins.

## Systems Overview
- **AISystem**: Simple manager for AI characters and behaviors.
- **BetterDragSystem**: Enhanced drag & drop for UI objects.
- **BetterUI**: Advanced and customizable UI components.
- **CheckpointSystem**: Checkpoint management and game state saving.
- **CrosshairSystem**: Crosshair management and customization.
- **CursorHandlerSystem**: Advanced cursor and mouse interaction management.
- **CustomSettingsSystem**: Custom game and user settings system.
- **DialogueSystem**: Branching and interactive dialogue management.
- **DismembererSystem**: Customizable meshes dismemberment system.
- **FeedbacksComposerSystem**: Centralized feedbacks system.
- **HitDetectionSystem**: Advanced hit detection system for gameplay mechanics.
- **InputsGlyphsSystem**: Dynamic input key visualization.
- **InputsHandlerSystem**: Centralized game input management.
- **InventorySystem**: Modular and customizable inventory system.
- **ObjectPool**: Object pooling for performance optimization.
- **QuestSystem**: Quest, objective, and progress management.
- **ResourceAttributesSystem**: Resource and attribute management (e.g., health, mana, stamina).
- **RPGSystem**: RPG components (stats, levels, abilities).
- **SaveSystem**: Game data save/load system.
- **WeaponSystem**: Weapon, ammo, and firing mechanics management.
- **VUEDKCore**: Common modules and utilities.

For detailed documentation of each system, refer to the `README.md` file inside the respective plugin folder.

## Example Project
For a practical demonstration of how to integrate and utilize the VUEDK systems within a game environment, you can refer to the official example project.
This prototype serves as a reference implementation, showcasing the interaction between various modules in a minimal yet functional game scenario.

🔗 [ProjectEK – GitHub Repository](https://github.com/VUEDK-Templates/VUEDK-EK_Template)

## Known Issues and Credits
- Some systems may have dependencies on each other: check the specific documentation.
- Developed by Denis Faraci.
