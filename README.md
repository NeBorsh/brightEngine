# brightEngine
> a small engine made in C++ using glfw, running on OpenGL. It's actually hard to say anything about him here...
------------------
# Building engine
> [!NOTE]
> I compiled the engine unfortunately using msvc, but I think it can be compiled using other compilers

To compile this miracle, you will need [xmake](https://xmake.io/), then clone the repository, and follow these steps:
- `1:` Temporarily move the folders with **models** and **shaders** somewhere, as well as the **shadowtext.cpp** file
- `2:` Leave in the folder for now only the xmake config and the src folder, then write `xmake build`
- `3:` Agree to install libraries via the package manager xmake and wait for the download, after which the project will build itself
- `4:` If the project was successfully built, move the folders with **shaders** and **models** to the folder with the executable file
- `5:` Run the executable file, or in the project console, enter the `xmake run`

> [!TIP]
> If you want to test shadows, you need to copy the code from `shadowtext.cpp` to `mine.cpp`, then run the project
------------------
# Roadmap
- [ ] implement more advanced and convenient functionality for interacting with the engine
- [ ] Add the ability to create and interact with the UI in every possible way
- [ ] Add the ability to work with sound in the engine
- [ ] Implementation of PBR in the engine
------------------
# Screenshots

![pikcha](https://i.ibb.co/wrNQ3dsd/2025-08-27-055735.png)
