#pragma once
#include "Graphics/Lights.h"
#include "Graphics/Material.h"
#include "Core/Base.h"

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Graphics
{
    enum ShaderLocation
    {
        SHADER_LOC_MAP_DIFFUSE = 0,
        SHADER_LOC_MAP_SPECULAR,
        SHADER_LOC_COLOR_AMBIENT,
        SHADER_LOC_COLOR_DIFFUSE,
        SHADER_LOC_COLOR_SPECULAR,
        SHADER_LOC_VALUE_ROUGHNESS,
        SHADER_LOC_VALUE_METALIC,
        SHADER_LOC_VECTOR_VIEW,
        SHADER_LOC_MATRIX_MODEL,
        SHADER_LOC_MATRIX_VIEW,
        SHADER_LOC_MATRIX_PROJECTION,
        SHADER_LOC_MATRIX_NORMAL,
        SHADER_LOC_COUNT
    };

    struct Shader
    {
        u32 id = 0;
        std::unordered_map<std::string, s32> uniforms;

        void CreateUniform(const std::string& name);
        void CreateMaterialUniform(const std::string& name);
        void CreateDirectionalLightUniform(const std::string& name);
        void SetInt(const std::string& name, s32 value);
        void SetFloat(const std::string& name, float value);
        void SetVec3(const std::string& name, glm::vec3& value);
        void SetMat4(const std::string& name, glm::mat4& value);
        void SetMaterial(const std::string& name, Material& material);
        void SetLight(const std::string& name, DirectionalLight& light);
    };

    Shader LoadShader(const char* vertexPath, const char* fragmentPath);
    void UnloadShader(Shader& shader);
    void BindShader(Shader& shader);
    void UnbindShader();
}
