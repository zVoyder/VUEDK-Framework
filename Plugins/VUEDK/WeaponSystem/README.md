# Weapon System

## Description
WeaponSystem is a modular Unreal Engine plugin for advanced weapon management. It supports firearms, melee weapons, and throwable weapons, providing a flexible framework for weapon logic, animation, crosshair, projectiles, and UI. The system is designed for extensibility, performance, and easy integration in both Blueprint and C++ workflows.

## How to Install and Run
1. Copy or clone the `WeaponSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use

### WeaponFirearms
- **WeaponFirearm**: Base class for all firearms. Supports shooting logic, ammo management, reloading, and fire modes.
- **ShootBarrel, ShootPoint**: Define the origin and direction of projectiles or traces.
- **Shooter**: Handles the firing logic, including spread, recoil, cooldown, and projectile spawning. Shooter can be used as a component or as a logic class, and is highly modular.
  - **Shooter Component**: Attach the Shooter component to your firearm or character to manage all aspects of shooting. It coordinates with barrels, points, handlers, and behaviours to execute the shot.
  - **How to use Shooter**: Add the Shooter component to your firearm actor. Configure its references to the barrel, shoot point, and handlers (recoil, spread, cooldown). Use Blueprint or C++ to call the shoot function, which will automatically process all logic and trigger the correct behaviour.
  - **Shooter Behaviours**: Shooter supports pluggable behaviours for different shot types:
    - **ShooterBehaviourBase**: Base class for all shooter behaviours.
    - **ShooterTraceBehaviour**: Implements hitscan/trace-based shooting (e.g., instant bullets, raycast weapons).
    - **ShooterPhysicProjectileBehaviour**: Implements physics-based projectile shooting (e.g., bullets, grenades, rockets). Spawns and configures projectiles.
    - You can create custom behaviours by inheriting from ShooterBehaviourBase and implementing your own shot logic.
  - **How to use Behaviours**: Assign the desired behaviour to the Shooter component (e.g., trace or projectile). The Shooter will delegate the shot execution to the selected behaviour, allowing you to switch between hitscan and projectile logic easily.
- **Handlers**: Modular classes for cooldown, recoil, spread, and shooter logic. Use or extend these for custom firearm behaviors.
- **Projectiles**: Includes base projectile, bullet, explosive, and grenade classes. Extend for custom projectile logic.
- **Montages**: FirearmMontagesManager manages animation montages for shooting, reloading, etc.
- **Crosshair**: WeaponCrosshairComponent and FirearmCrosshairWidget provide dynamic crosshair logic and UI.
- **How to use**: Add a WeaponFirearm to your character, configure the barrel, shooter, and handlers. Use Blueprint or C++ to trigger shooting, reloading, and ammo logic. Customize projectiles and crosshair as needed.

### WeaponMelees
- **WeaponMelee**: Base class for melee weapons. Supports attack states, combos, and hit detection.
- **Montages**: MeleeMontagesManager manages attack, defense, and buffer animation montages.
  - **Attack Montages**: Define the sequence of attack animations (combos). Each attack montage can have its own blend settings and notifies.
  - **Defensive Montage**: Used for block/parry/defense actions, with configurable cooldown and blend.
  - **Buffer Attack**: Supports buffered input for chaining attacks smoothly.
  - **How to use Montages**:
    - Configure the `AttackMontages` array in MeleeMontagesManager with your attack animation data (montage, blend, etc.).
    - Set the `DefensiveMontage` for block/parry actions and adjust `DefenseCooldown` as needed.
    - Use the `StartComboAttack`, `PlayNextAttackMontage`, and `EndComboAttack` methods to control attack sequences.
    - Use animation notifies (e.g., OnAttackBeginNotify, OnAttackFinishedNotify) to trigger hit detection, enable/disable hitboxes, and manage attack states.
    - The manager handles combo progression, buffering, defense state, and interruption logic automatically.
    - In Blueprint or C++, call the relevant methods on MeleeMontagesManager to start attacks, defend, or handle interrupts.
- **Crosshair Component**: WeaponMelee can use the `WeaponCrosshairComponent` to provide dynamic crosshair logic and UI for melee weapons, similar to firearms. This allows you to display and update crosshair widgets based on melee attack states or custom logic.
- **Notifies**: Custom anim notifies for attack state, buffer, damage hitbox, and defense state. Use these to trigger hit detection and state changes during animations.
- **Handlers**: Extendable logic for melee attack handling and state management.
- **How to use**: Add a WeaponMelee to your character, set up attack montages and notifies. Use Blueprint or C++ to trigger attacks and handle hit detection. Customize attack logic and states as needed.

### Thrower
- **Thrower**: Base class for throwable weapon logic. Manages throw input, cooldown, and spawn of throwable objects.
- **ThrowableBase, ThrowableGrenade**: Base and example classes for throwable objects. Extend for custom throwables (e.g., grenades, knives).
- **How to use**: Add a Thrower to your character, configure the throwable class and throw logic. Use Blueprint or C++ to trigger throws and manage cooldowns.

## Dependencies
This plugin requires the following VUEDK plugins to be enabled in your project:
- ObjectPool
- CrosshairSystem

Make sure all these plugins are enabled in your Unreal project for WeaponSystem to work correctly.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `Weapons/`: Base classes for WeaponBase, WeaponFirearm, WeaponMelee, and related data.
  - `Shooter/`: Shooter logic, barrels, points, handlers, projectiles, and behaviors.
  - `Thrower/`: Thrower logic and throwable objects.
  - `Montages/`: Animation montage managers, data, and notifies for firearms and melee.
  - `WeaponCrosshair/`: Crosshair component and widgets for weapon UI.
  - `Factories/`: Blueprint function libraries for weapon and handler creation.
- **Challenge:** Supporting modular weapon logic, animation, projectiles, and UI for different weapon types, while maintaining extensibility and performance in both Blueprint and C++.

## Known Issues & Credits
- **Known Issues:**
  - Advanced weapon logic (e.g., networked replication, custom projectile physics) may require custom extensions.
  - Compatibility is tested with Unreal Engine 5.x; earlier versions are not officially supported.
- **Credits:**
  - Developed by Denis Faraci.
