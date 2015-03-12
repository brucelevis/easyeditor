#include "ShaderMgrBase.h"
#include "IShader.h"

namespace e2d
{
	
ShaderMgrBase::ShaderMgrBase()
	: m_curr_shader(NULL)
{
}

ShaderMgrBase::~ShaderMgrBase()
{
}

void ShaderMgrBase::Release()
{
	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		m_shaders[i]->Unload();
	}
}

void ShaderMgrBase::reload()
{
	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		m_shaders[i]->Load();
	}
}

void ShaderMgrBase::Commit()
{
	if (m_curr_shader) {
		m_curr_shader->Commit();
	}
}

void ShaderMgrBase::SetProjection(int width, int height)
{
	for (int i = 0, n = m_shaders.size(); i < n; ++i) {
		m_shaders[i]->SetProjection(width, height);
	}
}

void ShaderMgrBase::Flush()
{
	// 	if (m_sprite_count != 0) {
	// 		wxLogDebug(_T("Shader Commit Flush"));
	// 	}

	Commit();

	//	DynamicTexture::Instance()->DebugDraw();
}

void ShaderMgrBase::Switch(IShader* shader)
{
	if (m_curr_shader != shader)
	{
		if (m_curr_shader) {
			m_curr_shader->Commit();
			m_curr_shader->Unbind();
		}
		m_curr_shader = shader;
		if (m_curr_shader) {
			m_curr_shader->Bind();
		}
	}
}

}