#ifndef _EASY2D_SPRITE_SHADER_H_
#define _EASY2D_SPRITE_SHADER_H_

#include "IShader.h"

#include "common/Matrix.h"

typedef unsigned __int32 uint32_t;

struct Colorf;

namespace e2d
{

class SpriteShader : public IShader
{
public:
	SpriteShader();
	virtual ~SpriteShader();

	virtual void Load();
	virtual void Unload();	

	virtual void Bind();
	virtual void Unbind();

	virtual void SetProjection(int width, int height);

	virtual void Commit();

	void SetModelView(const vec2& offset, float scale);

	int GetTexID() const;
	void SetTexID(int tex);

	int GetFboID() const;
	void SetFboID(int fbo);

	void SetBufferData(bool open);
	bool IsOpenBufferData() const;

	void SetColor(const Colorf& multi, const Colorf& add);
	void SetColorTrans(const Colorf& r_trans, const Colorf& g_trans, const Colorf& b_trans);

	void Draw(const float vb[16], int texid);
	void Draw(const vec2 vertices[4], const vec2 texcoords[4], int texid);

protected:
	virtual void BindAttrib(GLuint prog);

	virtual void LoadShader();

private:
	void InitBuffers();

	void CopyVertex(const float vb[16]);

private:
	GLuint m_model_view, m_projection;

	GLuint m_vertex_buffer;
	GLuint m_index_buffer;

	int m_count;
	float* m_vb;

	uint32_t m_color, m_additive;
	uint32_t m_r_trans, m_g_trans, m_b_trans;

	GLuint m_tex;
	GLuint m_fbo;

	bool m_open_buffer_data;

	Matrix m_mat_modelview, m_mat_projection;
	bool m_is_mat_dirty;

}; // SpriteShader

}

#endif // _EASY2D_SPRITE_SHADER_H_