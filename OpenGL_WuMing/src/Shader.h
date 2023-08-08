#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderPorgramSource
{
	std::string VertexSoruce;
	std::string FragmentSource;
};

class Shader
{
private:
	
	std::string m_filePath;
	
	unsigned int m_RendererID;

	/*
	* mutable unordered_map<std::string, int> m_LocationCache
	* cache for uniforms
	*/
	mutable std::unordered_map<std::string, int> m_LocationCache;

public:

	Shader(const std::string& filePath);

	~Shader();

	void Bind() const;
	void UnBind() const;

	//set uniforms
	void SetUniform1i(const std::string& Name, int value);

	void SetUniform4f(const std::string& Name, float v0, float v1, float v2, float v3);

	void SetUniformMat4f(const std::string& Name, const glm::mat4& matrix);
	//have some Uniform * f func //

private:

	int GetUniformLocation(const std::string& Name) const;

	ShaderPorgramSource parseShader(const std::string& filePath);

	unsigned int CompileShader(unsigned int type, const std::string& Soruce);

	unsigned int CreateSharder(const std::string& vertexShader, const std::string& fragmentShader);


};

