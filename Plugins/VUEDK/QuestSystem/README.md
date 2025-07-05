# Quest System

## Description
QuestSystem is a modular Unreal Engine plugin for advanced quest and task management. It allows you to define, track, and update quests and their tasks, supporting sequential and parallel quest structures, custom filters, and full Blueprint/C++ extensibility. The system is designed for flexibility, performance, and easy integration into any Unreal project.

## How to Install and Run
1. Copy or clone the `QuestSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Creating Quests and Tasks:**
  - Create quest data assets (e.g., `QuestData`, `QuestLogData`, `QuestEntryData`) in the Content Browser.
  - Define tasks using data assets like `TaskData`, `CountTaskData`, and assign them to quests.
  - Use `QuestBase`, `QuestParallel`, or `QuestSequencial` classes to structure your quests (sequential or parallel tasks).
- **Quest Management:**
  - Add a `QuestManager` component to your player controller, character, or a dedicated manager actor.
  - Use Blueprint or C++ functions to add, start, update, and complete quests and tasks.
  - Use `TaskAchiever` components to track and update task progress based on in-game events.
- **Quest Log and Filters:**
  - Use `QuestLogData` and `QuestFilterData` to manage quest logs and apply filters (e.g., by status or type).
  - Save and load quest progress using `QuestLogSaveData` and related structs.
- **Advanced:**
  - Extend or override quest/task logic by inheriting from base classes (`QuestBase`, `TaskBase`).
  - Use utility classes (`QSFactory`, `QSUtility`) for helper functions and quest/task creation.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `UObjects/Quests/`: Core quest classes (`QuestBase`, `QuestParallel`, `QuestSequencial`).
  - `UObjects/Tasks/`: Task classes (`TaskBase`, `CountTask`, `SingleTask`).
  - `Components/`: Components for quest and task management (`QuestManager`, `TaskAchiever`).
  - `Data/`: Data assets, enums, and structs for quests, tasks, filters, and logs.
  - `Utility/`: Helper utilities and factories for quest/task creation and management.
- **Challenge:** Supporting flexible quest structures (sequential/parallel), robust task tracking, and extensibility in both Blueprint and C++ while maintaining performance and usability.

## Known Issues & Credits
- **Known Issues:**
  - Advanced quest logic (e.g., networked multiplayer, custom task types) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
