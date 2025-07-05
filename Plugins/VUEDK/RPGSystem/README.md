# RPG System

## Description
RPGSystem is a modular Unreal Engine plugin for advanced RPG mechanics, including inventory, item generation, gear, stats, and UI utilities. It provides a flexible framework for creating, managing, and comparing RPG items, handling inventories (with support for gear and stats), and generating items with procedural stats. The system is designed for extensibility, performance, and seamless integration with both Blueprint and C++ workflows.

## How to Install and Run
1. Copy or clone the `RPGSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use

### Inventory Management
- **RPGInventoriesManager**: Manages multiple inventories per character or globally. Supports equipment slots, item stacking, and stat containers. Exposes Blueprint functions to get rarity/stat data by ID (`GetItemRarityByID`, `GetItemStatByID`).
  - *Usage*: Add as a component to your character or a global manager. Set up `ItemsStats` and `ItemsRarities` data assets to provide stat and rarity definitions. Use its Blueprint functions to retrieve stat/rarity data for items at runtime.
- **RPGInventoriesUtility**: Blueprint function library for common inventory and item operations. Allows you to:
  - Get the inventories manager (`GetRPGInventoriesManager`).
  - Manage equipment comparison (set target equipment/slot, request/cancel comparison, check comparison state, get compared item/slot).
  - Retrieve rarity/stat data by ID (`GetItemRarityByID`, `GetItemStatByID`).
  - Initialize the utility with a compare handler subsystem (`Init`).
  - *Usage*: Call these static functions from Blueprints to interact with inventories, compare gear, and fetch stat/rarity data. Use `Init` to set up the compare handler subsystem at game start.
- **RPGInventoryCompareHandlerSubsystem**: Subsystem for advanced comparison logic between equippable items, used by RPGInventoriesUtility to provide comparison features in UI and gameplay.
  - *Usage*: Automatically used by the utility; you can extend it for custom comparison logic or to add new comparison rules.

### Item System
- **RPGItemData / RPGGearItemData**: Data assets to define items and gear. Allow you to:
  - Enable/disable rarity (`bUseRarity`).
  - Use a predefined rarity (`bUsePredefinedRarity`, `PredefinedRarityLevelData`).
  - For gear, set a base multiplier (`GearBaseMultiplier`).
  - *Usage*: Create these assets in the Content Browser to define new items/gear. Assign them to inventory slots or use them in item generation.
- **RPGItem / RPGGearItem**: Runtime item classes, supporting stats, rarity, gear properties, and stacking.
  - *Usage*: Instantiated at runtime when items are added to inventories. Extend these classes for custom item logic.
- **RPGGearItemStatsContainer**: Manages and compares gear item stats.
  - *Usage*: Used internally for stat comparison and display; extend for custom stat logic.

### UI Utilities
- **RPGWidgetsUtility**: Blueprint library for advanced UI drawing functions:
  - `DrawCustomPolygon`: Draws custom polygons.
  - `DrawPolygon`: Draws regular polygons with n sides.
  - `DrawPolygonWithProgress`: Draws polygons with per-side progress (e.g., stat indicators).
  - `DrawLabelsAroundPolygon`: Draws labels around polygons (e.g., stat names).
  - *Usage*: Call from UMG widgets' OnPaint or custom UI logic to render advanced shapes and stat displays.

### Factories and Item Generation
- **RPGFactory**: Blueprint/C++ functions to create items, gear, inventories, and other RPG assets.
  - *Usage*: Use in Blueprints or C++ to spawn new items, inventories, or gear dynamically.
- **RPGItemsGenerator**: Procedural item generator, supporting custom stat operations (`RPGItemStatOperation`).
  - *Usage*: Use to generate random or procedural items with custom stats and rarity. Extend for custom generation rules.

### Subsystems
- **RPGInventoryCompareHandlerSubsystem**: Handles comparison logic for equippable items, integrated with utilities and UI.
  - *Usage*: Used by the system for all item comparison features; extend for advanced comparison needs.

## Folder Structure
- `Public/` and `Private/`: C++ headers and source files.
  - `RPGInventory/`: Inventory management, items, gear, stats, and utility classes.
    - `Items/`: Item and gear logic.
    - `Stats/`: Stat containers and operations.
    - `Utility/`: Inventory utility functions.
  - `RPGItemsGeneration/`: Item generator and stat operation logic.
  - `UI/`: UI utility classes for widgets and item display.
  - `Factories/`: Factories for creating RPG objects.
  - `StatsSystem/`:
    - `Data/`: Stat data assets (`CoreStatData`, `SpecialStatData`, `StatDataBase`).
    - `Containers/`: Stat containers for core and special stats (`CoreStatsContainer`, `SpecialStatsContainer`, `StatsContainer`).
    - `CharacterStats/`: Character stat bridge and integration with equipment (`CharacterStats`).
    - `StatsBridgeBase.h/cpp`: Base class for stat bridges, stat calculation, and events.
    - `StatOperation.h/cpp`, `BridgeStatOperation.h/cpp`: Stat operation logic and extension points.
    - `Utility/StatsUtility.h/cpp`: Blueprint function library for stat access.

## Challenges
- Supporting complex RPG item logic, procedural generation, stat comparison, and extensibility in both Blueprint and C++ while maintaining performance and usability.
- Supporting flexible stat calculation, equipment integration, and extensibility in both Blueprint and C++ while maintaining performance and usability.

## Known Issues & Credits
- **Known Issues:**
  - Advanced RPG logic (e.g., multiplayer sync, custom stat formulas) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.X; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.

---

# StatsSystem

## Description
StatsSystem is a modular system for managing, calculating, and displaying character and item statistics in RPGSystem. It supports core stats, special stats, stat containers, stat operations, and bridges for integrating stats with equipment and gameplay logic. The system is designed for extensibility, Blueprint/C++ integration, and advanced stat calculation workflows.

## How to Use
- **Stat Data Assets:**
  - Define stats using `CoreStatData` (for main stats, e.g., Health, Attack) and `SpecialStatData` (for special/unique stats, e.g., Crit Chance, Elemental Power). Each stat has an ID, code name, description, default value, and value range.
- **Stat Containers:**
  - Use `CoreStatsContainer` and `SpecialStatsContainer` to store and manage stat values. Add, remove, copy, and compare stats. Retrieve stat values as float, int, or string.
- **Stats Bridge:**
  - Use `StatsBridgeBase` (or derived classes like `CharacterStats`) as a component to bridge between stat containers and gameplay logic. It manages core/special stats, calculates full stat values, and exposes Blueprint events for stat recalculation.
  - Use `CalculateAllStatsValues()` to recalculate all stats, and access full stat values via `GetFullStatsValues()` or `GetFullStatValueAsFloat/Int/String()`.
  - Listen to `OnCalculatedCoreStatsValues` and `OnCalculatedFullStatsValues` for stat update events.
- **Stat Operations:**
  - Use `StatOperation` and `BridgeStatOperation` to define custom stat calculation logic (e.g., modifiers, multipliers). Extend these classes for advanced stat operations.
- **Stat Utility:**
  - Use `StatsUtility` Blueprint functions to get the stats bridge for a player pawn or controller.
- **Equipment Integration:**
  - Use `CharacterStats` with an `Equipment` component to automatically update stats when equipping/unequipping items. Stats from equipped items are added to the character's stats and recalculated.

## Dependencies
This plugin requires the following VUEDK plugins to be enabled in your project:
- InventorySystem
- SaveSystem
- BetterDragSystem

Make sure all these plugins are enabled in your Unreal project for RPGSystem to work correctly.

## Folder Structure & Challenges
- `Data/`: Stat data assets (`CoreStatData`, `SpecialStatData`, `StatDataBase`).
- `Containers/`: Stat containers for core and special stats (`CoreStatsContainer`, `SpecialStatsContainer`, `StatsContainer`).
- `CharacterStats/`: Character stat bridge and integration with equipment (`CharacterStats`).
- `StatsBridgeBase.h/cpp`: Base class for stat bridges, stat calculation, and events.
- `StatOperation.h/cpp`, `BridgeStatOperation.h/cpp`: Stat operation logic and extension points.
- `Utility/StatsUtility.h/cpp`: Blueprint function library for stat access.
- **Challenge:** Supporting flexible stat calculation, equipment integration, and extensibility in both Blueprint and C++ while maintaining performance and usability.

## Known Issues & Credits
- **Known Issues:**
  - Advanced stat logic (e.g., networked stat sync, custom stat formulas) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
