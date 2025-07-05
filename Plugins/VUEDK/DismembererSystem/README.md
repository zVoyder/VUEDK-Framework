# Dismemberer System

## Description
DismembererSystem is a modular Unreal Engine plugin for advanced character dismemberment and blood effects. It provides a flexible framework to handle skeletal mesh dismemberment, spawn blood decals and particles, and manage limb detachment with both Blueprint and C++ support. The system is designed for extensibility, performance, and easy integration into character-based games.

## How to Install and Run
1. Copy or clone the `DismembererSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Basic Setup:**
  - Add the Dismemberer component or logic to your character blueprint or C++ class.
  - Create Data Assets of type DismemberableAvatarData and DismembererBloodsData in the Content Browser. 
  - Assign these Data Assets to the Dismemberer component (in the details panel or via Blueprint/C++ logic).
- **Dismemberment:**
  - Call the dismemberment functions (e.g., `DismemberLimb`) from Blueprint or C++ to detach limbs at runtime.
  - The system will handle mesh separation, spawn blood decals, and trigger blood particle effects.
- **Customization:**
  - Customize blood decals and particles by editing or replacing assets in the `Content/Art/` folder.
  - Extend or override logic in C++ for advanced behaviors (e.g., custom limb logic, pooling, or effects).

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `Dismemberer.h/cpp`: Core dismemberment logic.
  - `DismemberedLimb.h/cpp`: Detached limb management.
  - `BloodDecal.h/cpp`, `BloodDecalsPool.h/cpp`: Blood decal spawning and pooling.
  - `BloodParticlesNiagaraCallbackHandler.h/cpp`: Niagara blood particle integration.
  - `Data/`: Data assets for skeletons and blood types.
  - `Utility/`: Helper utilities for the system.
- `Content/Art/`: Materials, textures, and Niagara systems for blood effects.
- **Challenge:** Ensuring performance with pooling, supporting complex skeletal meshes, and providing robust Blueprint/C++ extensibility.

## Known Issues & Credits
- **Known Issues:**
  - Advanced dismemberment (e.g., multi-mesh, networked games) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
