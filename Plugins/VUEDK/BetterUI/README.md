# BetterUI

## Description
BetterUI is a modular Unreal Engine plugin designed to enhance and standardize UI development. It provides advanced menu management, widget selection/focus logic, switch operations, and extensible interfaces for custom UI behaviors. The system is built for flexibility, Blueprint/C++ extensibility, and improved user experience in complex UI scenarios.

## Installation & How to Run
1. Copy or clone the `BetterUI` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Menu Management:**
  - Use the `UMenuManager` component to register, open, close, and toggle menus using gameplay tags.
  - Menus should inherit from `UMenuWidget` to support open/close events and Z-ordering.
- **Widget Selection & Focus:**
  - Inherit from `UBetterWidget` to get advanced selection, focus, and event logic.
  - Implement `IBetterSelectableWidget` and/or `IBetterFocusableWidget` for custom selection/focus behaviors.
- **Switch Operations:**
  - Use `UBetterSwitchOperation` to manage switching between widgets in a `UWidgetSwitcher`.
  - Create switch operations via `UBetterUIOperationsFactory` Blueprint nodes or C++.
- **Settings & Utility:**
  - Configure default UI behaviors in `UBetterUISettings`.
  - Use `UBetterUIUtility` for project info and helper functions.
- **Events:**
  - Bind to events in `UBetterUIEventsHandlerSubsystem` for back/confirm/select navigation.
- **Extend via Interfaces:**
  - Implement interfaces like `IBetterSettingsPanel`, `IBetterSettingsCustomOption`, and `IBetterSwitcherObject` for custom UI logic.

## Dependencies
This plugin requires the following VUEDK plugins to be enabled in your project:
- CustomSettingsSystem
- InputsGlyphsSystem

Make sure all these plugins are enabled in your Unreal project for BetterUI to work correctly.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `Widgets/`: Core widget classes (e.g., `BetterWidget`, `MenuWidget`).
  - `Components/`: Menu manager and related components.
  - `Operations/`: Switch operation logic.
  - `Interfaces/`: Extensible interfaces for custom UI behaviors.
  - `Factories/`: Blueprint function libraries for creating operations.
  - `Utility/`: Helper utilities and settings.
- **Challenge:** Ensuring seamless integration between C++ and Blueprint, and supporting flexible UI patterns while maintaining ease of use.

## Known Issues & Credits
- **Known Issues:**
  - Some advanced UI behaviors may require custom C++ extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
