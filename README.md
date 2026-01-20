# Graph

Graph is a C++ application that renders mathematical functions using OpenGL.
It supports interactive visualization of functions and a simple GUI for function input.
This project is currently in developement and additionnal features such as Cmake build with functions and axis intersection points calculation are coming soon.

This is my first OpenGL project which I did for learning graphics programing and software architecture.

Thanks to Victor Gordan and Joey de Vries for their amazing tutorials that helped me learn this stuff.


## Features

- OpenGL-based rendering
- Function plotting (such as x^2, sin(x), sqrt(x) ... )
- Grid and axis rendering
- ImGui-based user interface


## Screenshots

![images/screenshot.png](images/screenshot.png)


## Build Instructions

For now only visual studio build


### Requirements

- C++17 or newer
- OpenGL-capable GPU


### Third-Party Dependencies

The following libraries are used:

- GLFW
- GLAD
- GLM
- ImGui
- stb_image
- exprtk
- FreeType

All third-party licenses are listed in
[`THIRD_PARTY_LICENSES.md`](THIRD_PARTY_LICENSES.md).
