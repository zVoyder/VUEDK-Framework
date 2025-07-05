# Hit Detection System

## Description
HitDetectionSystem is a modular Unreal Engine plugin for advanced hit detection, per-bone damage multipliers, and flexible hit zone management. It enables you to define custom hit zones (box, sphere, capsule), assign damage multipliers to bones, and handle hit events with Blueprint and C++ support. The system is designed for extensibility, performance, and easy integration into character-based games.

## How to Install and Run
1. Copy or clone the `HitDetectionSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Basic Setup:**
  - Add the `HitBoxManager` component to your character or actor.
  - Create a Data Asset of type `HitBoxBonesData` to define per-bone damage multipliers and assign it to the `HitBoxManager`.
  - Add hit zone components (`HitZoneBox`, `HitZoneSphere`, `HitZoneCapsule`) to your actor to define specific hit zones and configure their handlers for custom logic and damage multipliers.
- **Handling Hits:**
  - The system automatically listens for damage events and applies multipliers based on the hit bone or hit zone.
  - Use the BlueprintAssignable events (`OnHitBoxAnyDamage`, `OnHitBoxPointDamage`, `OnHitBoxRadialDamage`) to react to hits in Blueprints.
  - Use the `HitDetectionSubsystem` to broadcast or listen for global hit events.
- **Customization:**
  - Implement custom logic in C++ or Blueprints by extending hit zone handlers or subscribing to events.
  - Use the `HitZoneRedirectorMode` to control how hit zones map to bones or custom names.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `HitBoxManager.h/cpp`: Main component for managing hit detection and damage.
  - `Data/HitBoxBonesData.h`: Data asset for per-bone damage multipliers.
  - `HitZones/`: Hit zone components (`HitZoneBox`, `HitZoneSphere`, `HitZoneCapsule`) and handler logic.
  - `Interfaces/HitZone.h`: Interface for hit zone components.
  - `HitDetectionSubsystem.h/cpp`: Subsystem for global hit events.
  - `Data/HitZoneRedirectorMode.h`: Enum for hit zone redirection logic.
- **Challenge:** Supporting flexible hit zone setups, robust per-bone logic, and seamless Blueprint/C++ extensibility while maintaining performance.

## Known Issues & Credits
- **Known Issues:**
  - Advanced use cases (e.g., networked hit replication, custom damage types) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
