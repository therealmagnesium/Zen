#include "Graphics/Shader.h"

#include "Core/IO.h"
#include "Core/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Graphics
{
    static u32 CompileShader(u32 target, const char* path)
    {
        s32 success;
        char log[512];

        Core::File file = Core::ReadFile(path);
        if (!file.isValid)
        {
            WARN("Failed to compile shader %s!", path);
            return 0;
        }

        u32 targetId = glCreateShader(target);
        glShaderSource(targetId, 1, (const char* const*)&file.data, NULL);
        glCompileShader(targetId);
        glGetShaderiv(targetId, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(targetId, 512, NULL, log);
            ERROR("Failed to compile shader!\n%s", log);
        }

        free(file.data);
        return targetId;
    }

    Shader LoadShader(const char* vertexPath, const char* fragmentPath)
    {
        Shader shader;
        shader.id = glCreateProgram();

        u32 vertex = CompileShader(GL_VERTEX_SHADER, vertexPath);
        u32 fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentPath);

        int success;
        char log[512];

        glAttachShader(shader.id, vertex);
        glAttachShader(shader.id, fragment);
        glLinkProgram(shader.id);
        glGetProgramiv(shader.id, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(shader.id, 512, NULL, log);
            ERROR("Failed to link shader!\n%s", log);
            return (Shader){};
        }

        return shader;
    }

    void UnloadShader(Shader& shader)
    {
        glUseProgram(0);
        glDeleteProgram(shader.id);
    }

    void BindShader(Shader& shader)
    {
        glUseProgram(shader.id);
    }

    void UnbindShader()
    {
        glUseProgram(0);
    }

    void Shader::CreateUniform(const std::string& name)
    {
        if (uniforms.find(name) != uniforms.end())
        {
            WARN("Could not create uniform %s because it already exists...", name.c_str());
            return;
        }

        s32 uniformLocation = glGetUniformLocation(id, name.c_str());
        if (uniformLocation < 0)
        {
            WARN("Could not create uniform %s because it wasn't found in the shader...", name.c_str());
            return;
        }

        uniforms[name] = uniformLocation;
    }

    void Shader::CreateMaterialUniform(const std::string& name)
    {
        CreateUniform(std::string(name) + ".diffuse");
        CreateUniform(std::string(name) + ".diffuseMap");
    }

    void Shader::CreateDirectionalLightUniform(const std::string& name)
    {
        CreateUniform(std::string(name) + ".intensity");
        CreateUniform(std::string(name) + ".color");
        CreateUniform(std::string(name) + ".direction");
    }

    void Shader::SetInt(const std::string& name, s32 value)
    {
        if (uniforms.find(name) != uniforms.end())
            glUniform1i(uniforms[name], value);
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        if (uniforms.find(name) != uniforms.end())
            glUniform1f(uniforms[name], value);
    }

    void Shader::SetVec3(const std::string& name, glm::vec3& value)
    {
        if (uniforms.find(name) != uniforms.end())
            glUniform3fv(uniforms[name], 1, glm::value_ptr(value));
    }

    void Shader::SetMat4(const std::string& name, glm::mat4& value)
    {
        if (uniforms.find(name) != uniforms.end())
            glUniformMatrix4fv(uniforms[name], 1, false, glm::value_ptr(value));
    }

    void Shader::SetMaterial(const std::string& name, Material& material)
    {
        SetVec3(name + ".diffuse", material.diffuse);
        SetInt(name + ".diffuseMap", material.diffuseMap.id);
    }

    void Shader::SetLight(const std::string& name, DirectionalLight& light)
    {
        SetFloat(name + ".intensity", light.intensity);
        SetVec3(name + ".color", light.color);
        SetVec3(name + ".direction", light.direction);
    }
}
