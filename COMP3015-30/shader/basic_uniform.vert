#version 460

//input vertex attributes
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

//Output variables passed to fragment shader
out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;


//Uniform matrices
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;


void main()
{
    //pass texture coordinates to frag shader
    TexCoord=VertexTexCoord;

    //Transform and normalize normal
    Normal = normalize(NormalMatrix*VertexNormal);
    //Transform vertex position
    Position = (ModelViewMatrix*vec4(VertexPosition,1.0)).xyz;
    gl_Position = MVP*vec4(VertexPosition,1.0);
}
