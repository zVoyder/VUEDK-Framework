# Resource Attributes System

## Description
ResourceAttributesSystem is a modular Unreal Engine plugin for advanced resource attribute management. It enables flexible, data-driven handling of attributes such as health, stamina, mana, and more for actors. The system supports automatic regeneration, min/max values, event dispatching, and full Blueprint/C++ extensibility.

## How to Install and Run
1. Copy or clone the `ResourceAttributesSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Data Asset:** Create `UResourceAttributeData` assets to define attributes such as Health, Mana, etc.
- **Manager Component:** Add the `UResourceAttributesManager` component to actors that need to manage resource attributes.
- **Configuration:** Assign your data assets to the `StartingAttributes` property of the manager.
- **API:** Use the manager's Blueprint or C++ API to read, modify, or listen to events for attributes at runtime.

## Main Classes Overview

### UResourceAttributeData
This is a data asset class that defines the configuration for a resource attribute type (e.g., Health, Mana). It contains:
- `AttributeTag`: Unique gameplay tag identifying the attribute type.
- `StartingValue`, `DefaultMinValue`, `DefaultMaxValue`: Initial, minimum, and maximum values for the attribute.
- Regeneration settings: Flags and parameters to control if/how the attribute regenerates (e.g., `bHasRegeneration`, `DefaultRegenerationRate`, delays for regen after increment/decrement, etc.).
- This asset is created in the Content Browser and referenced by the manager to instantiate attributes with consistent settings.

### UResourceAttribute
Represents a single instance of a resource attribute for an actor (e.g., a character's current Health). It:
- Holds a reference to its `UResourceAttributeData` for configuration.
- Tracks current value, min/max, and regeneration state.
- Provides API to get/set/modify the value, start/stop regeneration, and query state (min/max/percent).
- Dispatches events (BlueprintAssignable) for value changes, min/max reached, regeneration started/stopped, etc.
- Handles regeneration logic, including delays and rate, based on its data asset.

### UResourceAttributesManager
A component that manages a set of resource attributes for an actor. It:
- Owns and initializes multiple `UResourceAttribute` instances, one for each `UResourceAttributeData` in `StartingAttributes`.
- Provides API to add/remove attributes, get/set/modify by tag, and retrieve all attributes.
- Acts as the main interface for gameplay code or Blueprints to interact with an actor's resource attributes.
- Ensures each attribute is uniquely identified by its gameplay tag and manages their lifecycles.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files for the plugin.
  - `Attributes/ResourceAttribute.h/cpp`: Core logic for individual resource attributes.
  - `ResourceAttributesManager.h/cpp`: Component that manages a set of resource attributes for an actor.
  - `Data/ResourceAttributeData.h/cpp`: Data asset definitions for attribute configuration.
  - `Factories/ResourceAttributesFactory.h/cpp`: Factory for creating attribute instances.
- **Challenge:** Ensuring data-driven flexibility while maintaining performance and event consistency, especially with regeneration logic and event dispatching.

## Known Issues & Credits
- **Known Issues:**
  - The system assumes each attribute has a unique gameplay tag; duplicate tags may cause unexpected behavior.
  - Regeneration logic may require tuning for edge cases (e.g., rapid value changes).
- **Credits:**
  - Developed by Denis Faraci.
