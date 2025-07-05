# Save System

## Description
SaveSystem is a modular Unreal Engine plugin for advanced save/load management. It supports multiple save slots, auto-save, custom save data, filtering by type, and full Blueprint/C++ extensibility. The system is designed for flexibility, performance, and easy integration into any Unreal project.

## How to Install and Run
1. Copy or clone the `SaveSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Save/Load Operations:**
  - Use the `SaveManager` component or the `SaveSubsystem` to manage save and load operations.
  - Create custom save data by inheriting from `SaveData` or `DefaultSaveGame`.
  - Use Blueprint or C++ functions to save, load, delete, or enumerate save slots.
  - Use the `AutoSaveManager` for automatic periodic saves.
- **Slots System:**
  - The system supports multiple save slots. Each slot is identified by a unique name (FName) and stores its own save data and metadata.
  - **SlotInfoData** and **SlotInfoItem** are used to store and display metadata for each slot (e.g., timestamp, playtime, description). All slot metadata is managed in a `SlotInfos` object.
  - Slots can be created, deleted, and enumerated. The most recent slot is automatically selected if the current one is deleted.
  - Slots are stored in a dedicated directory and can be filtered or sorted by date, name, or custom criteria.
- **SaveGame & SharedSaveGame:**
  - The system distinguishes between regular save games (per-slot) and a shared save game (global data shared across slots).
  - **DefaultSaveGame** is the base class for both regular and shared saves. It stores the slot name and a map of actor transforms, but can be extended for custom data.
  - The shared save game is always stored in a special slot (e.g., `SharedSaveFile`). Use `SharedSave` and `LoadSharedSlot` to save/load global data.
  - When saving or loading, you can choose to operate on a specific slot (regular save) or the shared slot (shared save). The system manages both types in parallel if needed.
- **AutoSaveManager:**
  - The `AutoSaveManager` automates periodic saving based on a configurable interval.
  - It is initialized with a reference to the `SaveManager` and auto-save settings (`AutoSaveManagerData`).
  - You can pause and unpause auto-saving at runtime (e.g., during cutscenes or loading screens).
  - The auto-save timer uses the world timer manager to schedule saves. When triggered, it calls the save operation on the current slot or shared slot as configured.
  - Auto-save settings (interval, enabled/disabled, etc.) are configurable in the project settings.
- **Saver Components:**
  - You can use the provided `Saver` component (or extend it, e.g. with `TransformSaver`) to easily hook into the save/load process for any Actor or Component.
  - The `OnPrepSave` event is triggered before saving: here you can cast the `UDefaultSaveGame` to your custom save game class and set your data (variables, arrays, etc.) directly on it.
  - The `OnLoadGameCompleted` event is triggered after loading: here you can cast the loaded `UDefaultSaveGame` to your custom class and restore your variables/components from the saved data.
  - The following events are also available for advanced workflows:
    - `OnPrepLoad`: triggered before loading, for pre-load logic.
    - `OnPrepareSave`, `OnPrepareLoad`, `OnPrepareSharedSave`, `OnPrepareSharedLoad`: BlueprintNativeEvents for custom logic in C++/Blueprint.
    - `OnSaveGameCompleted`, `OnLoadGameCompleted`, `OnSharedSaveGameCompleted`, `OnSharedLoadGameCompleted`: triggered after save/load operations (regular/shared).
    - `OnBeginWithNewSaveGame`, `OnBeginWithLoadedSharedSaveGame`, `OnBeginWithNewSharedSaveGame`: triggered at the start of a new or loaded (shared) save game.
  - This workflow allows you to keep your save logic modular and reusable, and to save/load any custom data structure by simply implementing the logic in these events (either in Blueprint or C++).
  - Example: Use `OnPrepSave` to write your actor's health, inventory, or custom state to the save game; use `OnLoadGameCompleted` to restore them after loading.
  - **TransformSaver:** This component automatically saves the world transform (position, rotation, scale) of the actor it is attached to during save, and restores the actor's transform on load. You do not need to write custom logic for transform persistence—just add the component to your actor.
- **Using the SaveMasterID:**
  - The SaveSystem uses a `SaveMasterID` to set a master identifier that will be used by all Saver components to generate their SaveID.
  - By default, the SaveID is generated based on the map name where the actor is located. However, if you use sublevels, the map name will always be the main level, which can cause issues with unique SaveIDs for actors in sublevels.
  - To solve this, you can call `SetSaveMasterID` (on the SaveManager) when changing sublevel or context. All Saver components will then use this master ID to generate their SaveID, ensuring correct and unique identification across levels and sublevels.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `SaveManager.h/cpp`, `SaveSubsystem.h/cpp`: Core save/load logic and subsystem.
  - `AutoSaveManager.h/cpp`: Auto-save management.
  - `SaveSystemSettings.h/cpp`: Plugin settings and configuration.
  - `Components/Savers/`: Saver components for custom save logic (e.g., `Saver`, `TransformSaver`).
  - `Data/`: Save data, slot info, enums, and structs.
  - `Interfaces/Saveable.h`: Interface for custom saveable objects.
  - `Utility/`: Helper utilities for save slots and operations.
- **Challenge:** Supporting flexible save data structures, auto-save, filtering, and extensibility in both Blueprint and C++ while maintaining performance and usability.

## Known Issues & Credits
- **Known Issues:**
  - Advanced save logic (e.g., networked saves, custom serialization) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci (VUEDK, Inc.).
  - Thanks to the Unreal Engine community for feedback and suggestions.
