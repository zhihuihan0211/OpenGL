#include "Shader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filePath) 
    : m_filePath(filePath)//,m_RendererID(0)
{ 
    GLCall(ShaderPorgramSource source = parseShader(filePath));
    m_RendererID = CreateSharder(source.VertexSoruce, source.FragmentSource);

}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID))
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0))
}

void Shader::SetUniform1i(const std::string& Name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(Name), value));
}

void Shader::SetUniform4f(const std::string& Name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(Name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& Name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(Name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& Name)
{
    if (m_LocationCache.contains(Name))
        return m_LocationCache[Name];

    GLCall(int location = glGetUniformLocation(m_RendererID, Name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: Uniform < " << Name << " > doesn't exist! " << std::endl;
    }
    m_LocationCache[Name] = location;
    

	return location;
}

ShaderPorgramSource Shader::parseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class EShaderType
    {
        None = -1, vertex = 0, fragment = 1
    };

    std::string line;
    EShaderType type = EShaderType::None;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = EShaderType::vertex;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = EShaderType::fragment;
            }
        }
        else
        {
            ss[int(type)] << line << "\n";
        }
    }

    return  { ss[0].str(), ss[1].str() };
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& Soruce)
{
    unsigned int id = glCreateShader(type);
    const char* src = Soruce.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::string log("Failed to compile"); //= std::string("Failed to compile");
        if (type == GL_VERTEX_SHADER)
            log.append(" vertex ");
        else if (type == GL_FRAGMENT_SHADER)
            log.append(" fragment ");
        std::cout << log << std::endl;

        std::cout << message << std::endl;
        std::cout << "==============================================================" << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


unsigned int Shader::CreateSharder(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
