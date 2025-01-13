#version 450 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uvCoord;

out vec2 fragCoord;

uniform mat4 projectionMatrix;

void main()
{
    fragCoord = uvCoord;
    gl_Position = projectionMatrix * vec4(position.xy, 0.f, 1.f);
}
