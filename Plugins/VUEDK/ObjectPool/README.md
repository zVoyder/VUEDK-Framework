# Object Pool System

## Description
ObjectPoolSystem is a modular Unreal Engine plugin for efficient actor pooling. It allows you to pre-instantiate and reuse actors, reducing runtime instantiation costs and improving performance, especially in scenarios with frequent spawning and destruction (e.g., projectiles, effects, AI agents).

## How to Install and Run
1. Copy or clone the `ObjectPool` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Basic Setup:**
  - Create a Data Asset of type `ActorPoolData` to define which actors to pool and their pool sizes.
  - Add an `ActorPool` or `PoolsManager` component to your actor, or use the `ObjectPoolSubsystem` for global pooling.
  - Assign your `ActorPoolData` asset to the pool component or manager.
- **Settings:**
  - The plugin provides a global settings asset (`ObjectPoolSettings`) accessible in **Edit > Project Settings > Object Pool Settings**.
  - In these settings, you can configure the `PoolsManagers` array, which lets you define multiple pool managers, each with its own set of pools and related game modes.
  - Each entry in `PoolsManagers` allows you to:
    - Specify which GameModes the pool manager is active for (`RelatedGamemodes`).
    - Define a list of pools (`PoolsData`), each with:
      - A gameplay tag (`PoolTag`) to identify the pool.
      - The actor class to pool (`ActorClass`).
      - Initial pool size (`InitialSize`).
      - Whether the pool is capped (`bIsCapped`) and its maximum size (`MaxSize`).
  - These settings are loaded at startup and used by the subsystem and managers to automatically create and manage pools for the specified game modes.
- **Spawning and Releasing:**
  - Use the pool's `SpawnPooledActor` function (Blueprint or C++) to retrieve an actor from the pool.
  - When finished, call `ReleasePooledActor` to return the actor to the pool for reuse.
  - Your pooled actors must either inherit from `PooledActorBase` **or** implement the `IPooledActor` interface. This enables the system to call key events for initialization and cleanup.
    - **OnPooledActorBeginPlay:** Called when the pooled actor is spawned or reused from the pool.
    - **OnPooledActorEndPlay:** Called when the pooled actor is returned to the pool or destroyed. Use this event to clean up, disable components, or reset the actor's state before it is reused.
    - **AssignActorPool:** Called to assign the owning pool to the actor.
    - **GetActorPool:** Returns the pool currently associated with the actor.
    - **ReleasePooledActor:** Call this to manually release the actor back to its pool.
    - **ClearPooledActor:** Called to clear the actor's state when it is returned to the pool. This is useful for resetting properties or components that should not persist between uses.
  - To spawn (acquire) an object from the pool, first get the pool using its tag (e.g. with `PoolsUtility::GetPool(PoolTag)`), then call `AcquireActor` on the pool to retrieve an available actor.
- **Advanced Usage:**
  - Customize pooling behavior by extending pool or actor base classes.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `ActorPool.h/cpp`: Core actor pool logic.
  - `PooledActorBase.h/cpp`: Base class for pooled actors.
  - `PoolsManager.h/cpp`: Manager for multiple pools.
  - `ObjectPoolSubsystem.h/cpp`: Subsystem for global pooling.
  - `Data/ActorPoolData.h`: Data asset for pool configuration.
  - `Factories/ObjectPoolFactory.h/cpp`: Blueprint function libraries for pool creation.
  - `Utility/PoolsUtility.h/cpp`: Helper utilities for pooling.
  - `Interfaces/PooledActor.h`: Interface for pooled actors.
- **Challenge:** Ensuring thread safety, minimizing runtime allocation, and supporting both Blueprint and C++ extensibility while maintaining performance.

## Known Issues & Credits
- **Known Issues:**
  - Advanced pooling scenarios (e.g., networked pooling, async spawning) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
