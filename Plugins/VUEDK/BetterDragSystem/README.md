# Better Drag System

## Description
BetterDragSystem is a plugin for Unreal Engine that provides an advanced, extensible drag-and-drop system for UMG widgets.
It enables more flexible and feature-rich drag-and-drop interactions, supporting custom payloads, visual feedback, and key-based drop actions.

## How to Install and Run
1. **Clone or copy** the `BetterDragSystem` folder into your project's `Plugins/VUEDK/` directory.
2. **Enable the plugin** in your Unreal Engine project via the Plugins window.
3. **Regenerate project files** and rebuild your project.
4. Use the provided Blueprint nodes and C++ interfaces to integrate advanced drag-and-drop functionality into your widgets.

## How to Use
- To start a drag-and-drop operation, you must call the `DetectBetterDragDropOperation` function (via Blueprint or C++). This function detects when a drag should begin, based on your input logic.
- To receive drag-and-drop events (such as OnBetterDragDrop, OnBetterDragEnter, OnBetterDragLeave), your widget must either:
  - Inherit from `BetterDropReceiverWidget`, **or**
  - Implement the `IBetterDragDropReceiver` interface in your custom widget class.
- Only widgets that inherit or implement the interface will receive the drag-and-drop events and be able to handle drops.

## Folder Structure & Challenges
- `Public/` and `Private/`: Contains all C++ headers and source files for the plugin, including:
  - `Interfaces/`: Drag-and-drop receiver interfaces for widgets.
  - `Factories/`: Blueprint function libraries for creating drag operations.
  - `Widgets/`: Base widget classes for drag receivers.
  - `Utility/`: Helper utilities for drag-and-drop logic.
  - `BetterDragDropOperation.h/cpp`: Core drag operation logic.
  - `BetterDragDropSubsystem.h/cpp`: Subsystem for managing drag state.
- **Challenge:** Integrating with Unreal's input and widget system while maintaining extensibility and Blueprint support.

## Known Issues & Credits
- **Known Issues:**
  - Some advanced drag-and-drop features may require custom widget logic or C++ extensions.
  - Compatibility is tested with Unreal Engine 5.X; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
