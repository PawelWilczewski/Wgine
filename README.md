# Wgine
3D game engine with the focus on efficient batch rendering and (incoming) PBR workflow. The project is at its very early stages - little is guaranteed at this point. Currently, only OpenGL is supported, which will temporarily be the main graphics API of the engine. Support for DX11, DX12 and Vulkan incoming.

![Preview](../assets/img/preview.png?raw=true)

# Platforms
Windows x64 is the only supported platform right now. Likely Linux support in the future. The core of the engine is independent of any OS.

# Cloning and Building
The project is developed using Visual Studio 2022 which is the recommended IDE.

To build the project, `cd` to your desired parent directory and use the following string of commands:

1. `git clone --recursive https://github.com/PawelWilczewski/Wgine & cd Wgine`
3. `git submodule update --init --force --remote --recursive`
4. `mkdir thirdparty\bin\premake & cd thirdparty\bin\premake`
5. `curl --location --output premake.zip --url https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-windows.zip`
6. `curl --location --output LICENSE --url https://raw.githubusercontent.com/premake/premake-core/master/LICENSE.txt`
7. `tar -xf premake.zip & del premake.zip`
8. `cd .. & cd .. & cd .. & GenerateProjects.bat`

This will recursively clone the project, update all of the submodules and download and unzip premake to the required directory.

Now, open `Wgine.sln` in Visual Studio and build the project.

Running [GenerateProjects.bat](GenerateProjects.bat) will regenerate project files at any time.

# Gallery

![Initial Lighting Setup](../assets/img/initial_lighting.png?raw=true)

# Credit
The initial project setup and some of the overall structure of the engine inspired by [the Hazel 2D Game Engine series](https://www.youtube.com/watch?v=JxIZbV_XjAs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT) by [The Cherno](https://www.youtube.com/c/TheChernoProject). Thank you to the author for creating such an amazing learning resource!
