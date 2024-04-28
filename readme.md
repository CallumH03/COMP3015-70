# Palm Tree Beach
This my COMP3015 70% Submission in which we was tasked with expanding on our CW1 by implementing atleast 1 shader from a set list, in which I chose edge detection as I believe it gave a cool look and style to the beach. I also implemented toon shading based on feedback from CW1.

## Visual Studio + Operating System
What I used to create this program:

Visual Studio version: Microsoft Visual Studio Community 2022 (64-bit)
(Version 17.7.5)

Operating System version: Windows 10 Pro (Version 22H2)

## How My Program Works
My OpenGL C++ program utilizing the project template, GLAD, GLFW, and GLM libraries brings custom 3D models into the program. With blinnphong lighting, lighting, fog, toon shading and edge detection allowing for a unique looking beach.

Using GLSL for handling vertex and fragment shading, these define how the geometry are rendered with lighting calculations based on BlingPhong shading, and edge detection and fog.

My CPP file hands the rendering loop, this includes binding appropriate shaders, setting up uniforms, properties for the lighting and fog, and using fbo.

The code is commented to allow for easier understanding

## How To Run
To run simply
- Place OpenGL folder into your C:Drive/Users/Public
- Next open BuildEXE and Run PalmTreeBeach.exe

## Video Explanation
Youtube:
