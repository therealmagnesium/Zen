#version 450 core
layout (location = 0) in vec3 position;

out vec3 fragUVCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    fragUVCoords = position; 
    vec4 screenSpacePosition = projectionMatrix * viewMatrix * vec4(position, 1.f);
    gl_Position = screenSpacePosition;
}
