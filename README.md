# SecondEngineProject
A framework used for learning and experimenting.

Features:
- RHI Abstraction: Multi Rendering API Support: OpenGl, D3D11
- Custom pure C++ Unreal-like delegates implemented using templates.
- Native Win32 API and Windows Loop Handling
- Forward Rendering
- Deferred Rendering
- Directional Lights
- Point Lights
- Cascaded Shadow Maps
- Normal Mapping
- Parallax Mapping
- Bloom
- Scene graph with inline edit using ImGui
- Deferred Decals with Carmack reverse stencil
- Integration with RenderDoc(Press R to automatically capture and launch RenderDoc UI)

Note: Not all of these features may be fully functional at any given commit. That's to allow faster iteration. The commits at which these features are functional are named accordingly.

## 
Libs:
- Glad
- EASTL
- GTest
- Assimp
- stb_image
- imgui

Instructions:
1. Go into one of the Test Projects(Only 3DTest is working at the moment - Flappy Test is based on an older engine version).
2. Create a "Build" or any name directory for the Solution.
3. CMD into the new directory and run "cmake ..".
4. Open up the solution and set the 3DTest as the start-up project and run.

Requirements:
- Visual Studio - at least version 2019 with C++ Module
- CMake
- Python
