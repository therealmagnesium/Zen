#version 450 core
in vec3 fragPosition;
in vec2 fragCoords;
in vec3 fragNormal;

out vec4 finalColor;

struct Material
{
    vec3 diffuse;
    sampler2D diffuseMap;
};

struct DirectionalLight
{
    float intensity;
    vec3 color;
    vec3 direction;
};

const float near = 0.1f;
const float far = 100.f;

uniform vec3 cameraPosition;
uniform Material material;
uniform DirectionalLight directionalLight;

vec3 GetObjectColor()
{
    vec3 objectColor = vec3(0.f);
    vec3 texel = texture(material.diffuseMap, fragCoords).xyz;

    if (length(texel) > 0.f)
        objectColor = texel * material.diffuse;
    else
        objectColor = material.diffuse;

    return objectColor;
}

float CalculateDiffuse(vec3 normal, vec3 lightDirection)
{ 
    float diffuse = max(dot(normal, lightDirection), 0.4f);
    return diffuse; 
}

float CalculateSpecular(vec3 normal, vec3 lightDirection)
{
    vec3 viewDirection = normalize(cameraPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specular = pow(max(dot(viewDirection, reflectDirection), 0.f), 128.f);

    return specular;
}

vec3 CalculateDirectionalLighting(vec3 normal)
{ 
    vec3 lightDirection = normalize(-directionalLight.direction);
    vec3 lightOutput = directionalLight.color * directionalLight.intensity;
    float diffuse = CalculateDiffuse(normal, lightDirection);
    float specular = CalculateSpecular(normal, lightDirection);

    return lightOutput * (diffuse + specular);
}

vec3 LinearizeDepth(float depth)
{
    float z = depth * 2.f - 1.f;
    float linearDepth = (2.f * near * far) / (far + near - z * (far - near));

    return vec3(linearDepth) / far;
}

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 objectColor = GetObjectColor(); 
    vec3 result = CalculateDirectionalLighting(normal) * objectColor;

    finalColor = vec4(result, 1.f);
}
