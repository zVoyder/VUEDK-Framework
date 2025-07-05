# Crosshair System

## Description
CrosshairSystem is a modular Unreal Engine plugin for advanced crosshair management. It allows you to easily display, switch, and customize crosshair widgets in your game, supporting payloads, events, and both Blueprint and C++ workflows. The system is designed for flexibility, extensibility, and seamless integration with player controllers and UI logic.

## How to Install and Run
1. Copy or clone the `CrosshairSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Subsystem Usage:**
  - Use the `UCrosshairSubsystem` to manage crosshair widgets for each local player.
  - Call functions like `SetDefaultCrosshairInViewport`, `SetCrosshairInViewport`, `ShowCrosshair`, `HideCrosshair`, and `CloseCrosshairInViewport` from C++ or Blueprints.
  - You can pass an optional payload to crosshair widgets for custom logic.
- **Component Usage:**
  - Add a `UCrosshairComponent` to your actor to control crosshairs directly from components.
  - Set the `CrosshairWidgetClass` and use the provided Blueprint-callable functions.
- **Widget Customization:**
  - Inherit from `UCrosshairWidget` to create custom crosshair widgets.
  - Override Blueprint events like `OnInit`, `OnStartOpening`, `OnStartClosing`, `OnCompleteOpening`, and `OnCompleteClosing` for animation and logic.
  - Use the payload system to pass data to your crosshair widget.
- **Utility Functions:**
  - Use `UCrosshairsUtility` static functions for global crosshair management.
- **Factory:**
  - Use `UCrosshairsFactory` to create crosshair widgets with payloads programmatically.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `Widgets/`: Base crosshair widget class and related logic.
  - `Utility/`: Static utility functions for crosshair management.
  - `Factories/`: Widget factory for crosshair creation.
  - `CrosshairComponent.h/cpp`: Actor component for crosshair control.
  - `CrosshairSubsystem.h/cpp`: Subsystem for player-based crosshair management.
  - `CrosshairSystemSettings.h/cpp`: Plugin settings and configuration.
- **Challenge:** Ensuring smooth switching and animation of crosshairs, supporting payloads, and providing robust Blueprint/C++ extensibility.

## Known Issues & Credits
- **Known Issues:**
  - Advanced crosshair logic (e.g., multiplayer sync, complex payloads) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
