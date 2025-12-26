# Working On
- [ ] scene manager
## Notes 
Added some improvements to the render loop. Now the cubes are instantiating a new shader each.
The ResManager should solve this issue. After that, check that the renderer is not binding the shaders after the first!
Also Material should gradually become a virtual class, then maybe create a couple of "default" instances to plug&play.

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
- [ ] Editor to Runtime workflow
- [ ] Basic imgui editor layout
- [ ] Create Prefabs in the editor, dynamically add the attached scripts, Drag&Drop them into scene
- [ ] Mouse click objects selection (check out ogldev video)
- [ ] Basic gizmos
- [ ] Debugging and profiling utils

# Engine Utils
- [ ] assimp integration
- [ ] animations (via assimp import)
- [ ] scene manager
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