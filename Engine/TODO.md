## Colors meaning
- <span style="color:yellow;">Yellow</span> = WIP
- <span style="color:red;">Red</span> = Blocked
## Notes 

## Next
GUI first implementation/integration. We need to have a fast way to change settings and instantiate objects.

# Rendering
- [x] Skybox
- [x] The scene should automatically call Draw on renderables
- [x] better shader/material customization (improve renderer flexibility with shaders)
- [ ] Material should be an abstract class (in the far future)
- [ ] Lights (phong)
- [ ] Light types (spotlight, point light, directional)
- [ ] PostProcessing
- [ ] AntiAliasing
- [ ] RayTracing (experiment) / Maybe this needs a Renderer interface.
- [ ] SSAO
- [ ] Terrain
- [ ] Vegetation (grass and bushes with no imported mesh) [video](https://www.youtube.com/watch?v=R-bjXOEQyX8&t=65s)

# Physics
- [ ] Physics engine integration (bullet3?)

# Editor
- [ ] <span style="color:yellow;">Editor to Runtime workflow</span>
- [ ] <span style="color:yellow;">Basic imgui editor layout</span>
- [ ] Virtual filesystem / Resource folder into binary folder / whatever to load assets from disk without hardcoded relative paths
- [ ] Create Prefabs in the editor, dynamically add the attached scripts, Drag&Drop them into scene
- [ ] Mouse click objects selection (check out ogldev video)
- [ ] Basic gizmos
- [ ] Debugging and profiling utils

# Engine Utils
- [ ] <span style="color:yellow;">assimp integration</span>
- [ ] animations (via assimp import)
- [x] scene manager
- [ ] GLTF for easily storing scenes
- [ ] Networking (experiment)
- [ ] LoD
- [ ] Game UI
- [x] Resource Manager + Memory management like for GameObjects
- [ ] Particle System
- [x] Event System

# Audio
- [ ] audio components
- [ ] Engine api instead of sdl
- [ ] 3D/directional audio (experiment)
- [ ] raytraced audio (experiment)