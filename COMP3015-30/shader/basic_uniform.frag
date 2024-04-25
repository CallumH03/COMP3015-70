#version 460

//Input Variables
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

//Texture sampler
layout (binding=0) uniform sampler2D Tex1;
layout (binding=1) uniform sampler2D Tex2;

//Output variable
layout (location = 0) out vec4 FragColor;

//Light information
uniform struct LightInfo{
    vec4 Position;
    vec3 La;
    vec3 L;
}lights[3];

//Material information
uniform struct MaterialInfo{
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
}Material;

//Fog information
uniform struct FogInfo{
    float MaxDist;
    float MinDist;
    vec3 Color;
}Fog;

//Bool, used to indicate if using sand texture
uniform bool Sand;

//Function to calculate phong lighting
vec3 phongModel( int light, vec3 position, vec3 n){
    vec3 texColor = vec3(0.7f, 0.4f, 0.2f);
    if (Sand) {
        texColor = texture(Tex1, TexCoord).rgb;
    }
    else {
        texColor = texture(Tex2, TexCoord).rgb;
    }
    vec3 ambient=lights[light].La*texColor;;
    vec3 s=normalize(vec3(lights[light].Position.xyz)-position);
    float sDotN=max(dot(s,n),0.0);
    vec3 diffuse=texColor*sDotN;
    vec3 spec=vec3(0.0);
    if (sDotN>0.0){
        vec3 v=normalize(-position.xyz);
        vec3 r=reflect(-s,n);
        spec=Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
    }
    return ambient+(diffuse+spec)*lights[light].L;
}

void main() {
    vec3 NormalizedNormal = normalize(Normal);
    vec3 Colour = vec3(0.0);

    //Calculate lighting contribution from each light source
    for (int i = 0; i < 3; i++) {
        Colour += phongModel(i, Position, NormalizedNormal);
    }

    //Calulate fog based on distance from camera
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    //Mix final color with fog color based on fog factor
    vec3 finalColor = mix(Fog.Color, Colour, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}