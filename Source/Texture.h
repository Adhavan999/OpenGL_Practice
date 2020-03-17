#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID, m_ActiveSlot;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	template<typename T>
	void SetParameter(unsigned int target, unsigned int parametername, T parameter);

	void Bind(unsigned int slot = 0);
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};
