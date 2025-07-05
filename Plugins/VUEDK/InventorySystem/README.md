# Inventory System

## Description
InventorySystem is a modular Unreal Engine plugin for advanced inventory management. It supports multiple inventory types (classic, tetris, equipment), item stacking, drag-and-drop, item dropping in the world, and full Blueprint/C++ extensibility. The system is designed for flexibility, performance, and easy integration into any Unreal project.

## How to Install and Run
1. Copy or clone the `InventorySystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Inventory Types:**
  - Use `ClassicInventory` for non-grid-based slot inventories (like RPGs).
  - Use `TetrisInventory` for tetris-style inventories (items with size/shape, like Resident Evil).
  - Use `Equipment` for equippable item slots (weapons, armor, etc.).
- **Creating an Inventory:**
  - Add an inventory component (e.g., `ClassicInventory`, `TetrisInventory`, or `Equipment`) to your character, controller, or actor.
  - Configure inventory size, slot data, and allowed item types in the component details or via Blueprint/C++.
- **Adding Items:**
  - Create item data assets (e.g., `ClassicItemData`, `TetrisItemData`) in the Content Browser.
  - Use the inventory's `AddItem` function (Blueprint or C++) to add items by referencing the data asset.
  - Items can be stacked, split, or merged depending on their data settings.
- **Drag and Drop:**
  - Use the provided `ItemWidget` and drag-and-drop operations for UI item movement.
  - For Tetris inventories, use the base widgets: `WBP_EquipSlot`, `WBP_TetrisInventoryGrid`, `WBP_TetrisSlot`, `WBP_TetrisEquipSlot`, `WBP_TetrisItem`, `DD_TetrisDragOperation`, and `WBP_DraggedTetrisItem` to quickly build and customize your inventory UI.
  - Implement custom drag logic or visuals by extending `ItemWidget`, Tetris widgets, or drag operation classes.
- **Dropping Items in the World:**
  - Use `DropItemOperation` or `ItemDropActor` to spawn dropped items in the game world.
  - Configure drop logic (e.g., spawn location, physics) in Blueprint or C++.
- **Saving and Loading:**
  - Use the provided save data structs (e.g., `InventoryBaseSaveData`, `ClassicInventorySaveData`, `TetrisInventorySaveData`) to serialize and restore inventory state.
- **Advanced:**
  - Use `InventoriesManager` to manage multiple inventories per actor or globally.
  - Use utility classes (`ISInventoriesUtility`, `TetrisInventoriesUtility`) for helper functions and conversions.
  - Extend or override inventory/item logic by inheriting from base classes (`InventoryBase`, `ItemBase`).

## Dependencies
This plugin requires the following VUEDK plugins to be enabled in your project:
- SaveSystem
- BetterDragSystem

Make sure all these plugins are enabled in your Unreal project for InventorySystem to work correctly.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `Base/`: Core inventory and item logic, drop operations, managers, and save data.
  - `ClassicInventory/`: Classic inventory and item classes, data, and save logic.
  - `TetrisInventory/`: Tetris inventory, item, slot, data, events handler, and save logic.
  - `EquipmentSystem/`: Equipment slots, equip logic, preconditions, and data.
  - `Factories/`: Blueprint function libraries for creating items and inventories.
  - `Utility/`: Helper utilities for inventories and tetris logic.
  - `Widgets/`: UI widgets for items and inventory slots.
- **Challenge:** Supporting multiple inventory types, drag-and-drop, item stacking, and extensibility in both Blueprint and C++ while maintaining performance and usability.

## Known Issues & Credits
- **Known Issues:**
  - Advanced inventory logic (e.g., networked multiplayer, custom stacking rules) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
