# Checkpoint System

## Project Description
CheckpointSystem is a modular Unreal Engine plugin for managing checkpoints in your game. It provides a flexible way to set, retrieve, and save player checkpoints, supporting both Blueprint and C++ workflows. The system is designed for easy integration and extensibility, allowing you to track player progress and restore positions efficiently.

## Installation & How to Run
1. Copy or clone the `CheckpointSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Initialization:**
  - Add the `UCheckpointsManager` component to your desired Actor (e.g., the player controller or character).
  - Optionally, initialize the manager using `UCSUtility::Init()` if you want to use the static utility functions.
- **Setting a Checkpoint:**
  - Call `TrySetCheckpoint` (from the manager or via `UCSUtility`) with a unique key and the desired transform.
- **Retrieving a Checkpoint:**
  - Use `TryGetCheckpoint` to get a checkpoint by key, or `TryGetLastCheckpoint` to get the most recent one.
- **Saving/Loading:**
  - Use `CreateSaveData` to generate a save object, and `LoadSaveData` to restore checkpoints from saved data.
- **Blueprint Support:**
  - All main functions are exposed to Blueprints for easy integration.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `CheckpointsManager.h/cpp`: Core logic for managing checkpoints.
  - `Utility/CSUtility.h/cpp`: Static utility functions for global checkpoint access.
  - `Data/CheckpointIndexData.h`: Struct for checkpoint keys.
  - `Data/SaveData/CheckpointsSaveData.h`: Struct for checkpoint save data.
- **Challenge:** Ensuring robust serialization and deserialization of checkpoint data, and supporting both Blueprint and C++ workflows seamlessly.

## Known Issues & Credits
- **Known Issues:**
  - Advanced checkpoint logic (e.g., multi-level or networked checkpoints) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
