# Night Sky Grass Plains OpenGL & GLSL Scene
## COMP3015 Games Graphics Pipelines Coursework 1
### Development Hardware
 - IDE: Visual Studio 2022
 - OS: Windows 11
### Overview "How does it work?"
The Night Sky Grass Plains OpenGL & GLSL Scene program contains a scene which can be described by the project's title. While being viewed, the camera will rotate around the scene in order to give the user a better view of the entire scene. The scene also contains fog, which will dynamically change in depth.
### Architecture "How does your code fit together ..."
#### Overview
This program contains 3rd party code written specifically for the related module by the organisation running it. That is, the project template base project. Expectable libraries are also present in this program. None of the aforementioned code is directly discussed; only that which has been written by the author of this program is discussed.
#### Scenebasic Uniform Header & CPP Files
The Scenebasic Uniform .h and .cpp files contain all code that is run on the CPU. Scenebasic Uniform itself loads models into the scene and sets uniform variables that the shader code is dependent upon.
#### Shaders
The basic_uniform.vert and basic_uniform.frag files are shader programs. They contain all the code that is run on the GPU in order to render the 3 dimensional graphics. The vertex shader is the first shader program to run. The vertex shader sends any relevant information to the fragment shader, which runs after.
##### Vertex
The vertex shader is used in order to process the coordinates of the vertices of objects that are loaded into the scene. Exceptionally, within this program, the vertex shader is also used to implement vertex lighting. Normally, lighting is implemented within the fragment shader, which is also the case for the majority of lights in this program's scene.
##### Fragment
The fragment shader is used in order to process the colours applied to objects, and by extension their manifestations, such as lighting and texturing.
### Demonstrations
//YouTube Video Link
### References
