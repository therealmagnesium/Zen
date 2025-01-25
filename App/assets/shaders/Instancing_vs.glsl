#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoords;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 instanceMatrix;

out vec3 fragPosition;
out vec2 fragCoords;
out vec3 fragNormal;

uniform mat4 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    fragPosition = vec3(instanceMatrix * vec4(position, 1.f));
    fragCoords = uvCoords;
    fragNormal = vec3(normalMatrix * vec4(normal, 1.f));
    gl_Position = projectionMatrix * viewMatrix * instanceMatrix * vec4(position, 1.f);
}
