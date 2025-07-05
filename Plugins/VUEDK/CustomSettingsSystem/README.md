# Custom Settings System

## Description
CustomSettingsSystem is a modular Unreal Engine plugin for advanced game and input settings management. It provides a flexible framework to define, modify, and save custom game options, including support for input modifiers, dead zones, scalars, and negation. The system is designed for extensibility, Blueprint/C++ integration, and robust user settings workflows.

## How to Install and Run
1. Copy or clone the `CustomSettingsSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Game User Settings:**
  - Use `UCustomGameUserSettings` to access and modify custom game options.
  - Call utility functions from `UCustomGameUserSettingsUtility` for reading, writing, and applying settings.
- **Input Modifiers:**
  - Define and configure input modifiers (dead zone, scalar, negate) using the provided classes in the `InputsModifiers` folder.
  - Extend or customize input logic by inheriting from `UInputModifierSettingsBase`.
- **Settings Data:**
  - Store and retrieve custom option data using the `CustomOptionData` struct.
- **Blueprint Support:**
  - All main functions are exposed to Blueprints for easy integration in UI and gameplay logic.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `CustomGameUserSettings.h/cpp`: Core logic for custom user settings.
  - `Utility/CustomGameUserSettingsUtility.h/cpp`: Helper functions for settings management.
  - `InputsModifiers/`: Input modifier classes (dead zone, scalar, negate, base).
  - `Data/CustomOptionData.h`: Struct for custom option data.
  - `CSSSettings.h/cpp`: Plugin settings and configuration.
- **Challenge:** Ensuring robust serialization of custom settings, supporting both input and game options, and providing seamless Blueprint/C++ extensibility.

## Known Issues & Credits
- **Known Issues:**
  - Advanced settings logic (e.g., per-user profiles, networked settings) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
