#version 450 core
out vec4 finalColor;

in vec3 fragUVCoords;

uniform samplerCube skybox;

void main()
{
    finalColor = texture(skybox, fragUVCoords);
}
