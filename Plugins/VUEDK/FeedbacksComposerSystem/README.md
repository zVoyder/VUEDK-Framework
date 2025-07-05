# Game Effects Cue System

## Description
GameEffectsCueSystem is a modular Unreal Engine plugin for composing, sequencing, and managing complex feedback effects (such as sounds, camera shakes, hit stops, and more) in response to gameplay events. It enables designers and developers to create reusable, data-driven effect cues that can be triggered from animations, code, or Blueprints.

## How to Install and Run
1. Copy or clone the `FeedbacksComposerSystem` folder into your project's `Plugins/VUEDK/` directory.
2. Enable the plugin in your Unreal Engine project via the Plugins window.
3. Regenerate project files and rebuild your project.
4. The plugin is now ready for use in both C++ and Blueprints.

## How to Use
- **Create a Cue Data Asset:**
  - In the Content Browser, create a new Data Asset of type `GameEffectsCueData`.
  - Assign or create a `GameEffectsCue` object inside the asset and add desired effects (e.g., `SoundFeedback`, `CameraShakeFeedback`, `HitStopFeedback`).
- **Triggering a Cue:**
  - Use the `GameEffectsCueManager` or `GameEffectsCueFactory` Blueprint nodes or C++ functions to start a cue by passing an instigator and the `GameEffectsCueData` asset.
  - You can also use the `GameEffectsCueNotify` in animation notifies to trigger cues from animation sequences.
- **Sequencing and Control:**
  - Cues can be started, stopped, resumed, interrupted, or advanced to next/previous effects via Blueprint or C++ API.
  - Each effect supports delays, and you can listen to cue events (started, stopped, finished, etc.) for custom logic.

## Folder Structure & Challenges
- `Public/` and `Private/`: C++ headers and source files.
  - `GameEffectsCue.h/cpp`: Core cue logic and sequencing.
  - `GameEffectsCueManager.h/cpp`: Static manager for starting cues.
  - `Factories/GameEffectsCueFactory.h/cpp`: Factory for creating cues from data assets.
  - `Data/GameEffectsCueData.h/cpp`: Data asset definition for cue composition.
  - `Effects/`: Contains effect types (e.g., `SoundFeedback`, `CameraShakeFeedback`, `HitStopFeedback`, `GameEffectBase`).
  - `GameEffectsCueNotify.h/cpp`: Animation notify for triggering cues from animations.
- **Challenge:** Designing a flexible, extensible system that supports sequencing, delays, and a variety of effect types, while remaining easy to use in both C++ and Blueprints.

