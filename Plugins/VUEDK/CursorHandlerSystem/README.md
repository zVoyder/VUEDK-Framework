# Cursor Handler System

## Description
CursorHandlerSystem is a modular Unreal Engine plugin for advanced cursor management. It enables seamless switching and customization of mouse and gamepad cursors, supporting custom widgets, sensitivity settings, and analog stick input. The system is designed for flexibility, extensibility, and easy integration with player controllers and UI logic.

## How to Install and Run
1. Copy or clone the `CursorHandlerSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Subsystem Usage:**
  - The `UCursorHandlerSubsystem` is automatically available for each local player.
  - It manages the display and switching of mouse and gamepad cursor widgets based on input device.
  - Sensitivity and widget classes can be configured in the project settings under "Cursor Handler System Settings".
- **Customization:**
  - Assign your custom cursor widgets (for mouse and gamepad) in the settings using `GamepadCursorWidget` and `MouseCursorWidget`.
  - Adjust analog stick sensitivity with `StickCursorSensitivity`.
  - Choose which analog stick controls the cursor with `CursorStick`.
- **Events and Logic:**
  - The subsystem automatically detects input device changes and switches the active cursor widget.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `CursorHandlerSystemSettings.h/cpp`: Settings and configuration.
  - `CursorHandlerSubsystem.h/cpp`: Core subsystem logic for cursor management.
  - `CursorHandlerSystem.h/cpp`: Module definition and logging.
- **Challenge:** Ensuring smooth and responsive cursor switching, supporting both mouse and gamepad input, and providing robust Blueprint/C++ extensibility.

## Known Issues & Credits
- **Known Issues:**
  - Advanced cursor logic may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
