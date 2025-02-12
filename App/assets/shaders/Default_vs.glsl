#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoords;
layout (location = 2) in vec3 normal;

out vec3 fragPosition;
out vec2 fragCoords;
out vec3 fragNormal;

uniform mat4 transformMatrix;
uniform mat4 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    fragPosition = vec3(transformMatrix * vec4(position, 1.f));
    fragCoords = uvCoords;
    fragNormal = mat3(normalMatrix) * normal;
    gl_Position = projectionMatrix * viewMatrix * transformMatrix * vec4(position, 1.f);
}
