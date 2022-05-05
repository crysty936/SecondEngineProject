# SecondEngineProject
A framework I am developing to learn and experiment with Graphics and Engine Architecture.

Features:
- Multithreaded renderer architecture - Command driven renderer, loading assets is done on a different thread, complete separation in plan.
- Custom pure C++ Unreal-like delegates implemented using templates.
- Timers system.

## 
Libs:
- GLFW
- Glad
- EASTL
- GTest
- Assimp
- stb_image

Instructions:
1. Go into one of the Test Projects(Only 3DTest is working at the moment - Flappy Test is based on an older engine version).
2. Create a "Build" or any name directory for the Solution.
3. CMD into the new directory and run "cmake ..".
4. Open up the solution and set the 3DTest as the start-up project and run.

Requirements:
- Visual Studio - at least version 2019 with C++ Module
- CMake
- Python
