# Inputs Handler System

## Description
InputsHandlerSystem is a modular Unreal Engine plugin for advanced input management. It allows you to define, switch, and prioritize input mapping contexts for gameplay and UI, supporting both keyboard/mouse and gamepad. The system is designed for flexibility, Blueprint/C++ extensibility, and seamless integration with Unreal's Enhanced Input system.

## How to Install and Run
1. Copy or clone the `InputsHandlerSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Configuration:**
  - Open the project settings and configure the Inputs Handler System Settings.
  - Define your Common, Gameplay, and UI input mapping contexts, set their priorities, and choose the context behaviour (Switch or Priority).
  - Optionally, set excluded contexts for UI mode and customize input modes for gameplay and UI (mouse cursor, input flush, etc.).
- **Subsystem Usage:**
  - Use the `UInputsHandlerSubsystem` (available per local player) to switch between gameplay and UI input modes via `SetModeGameplay()` and `SetModeUI()` (Blueprint or C++).
  - Add or remove mapping contexts at runtime using the provided functions.
  - Listen to `OnGameplayMappingContextSet` and `OnUIMappingContextSet` events for context changes.
  - Query the current input mode and whether a gamepad is in use.
- **Utility Functions:**
  - Use `UInputsHandlerUtility` static functions for global access to input mode switching and gamepad checks.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `InputsHandlerSubsystem.h/cpp`: Main subsystem for input context management.
  - `InputsHandlerSystemSettings.h/cpp`: Project settings and configuration.
  - `Utility/InputsHandlerUtility.h/cpp`: Static utility functions for input management.
  - `Data/`: Structs and enums for input modes, context behaviour, etc.
- **Challenge:** Supporting flexible context switching, robust gamepad/mouse detection, and seamless Blueprint/C++ extensibility while maintaining performance and compatibility with Enhanced Input.

## Known Issues & Credits
- **Known Issues:**
  - Advanced use cases (e.g., multiplayer input sync, custom device support) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
