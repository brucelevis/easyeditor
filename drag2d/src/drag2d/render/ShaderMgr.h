#ifndef _D2D_SHADER_MGR_H_
#define _D2D_SHADER_MGR_H_

#include "ShaderMgrBase.h"

#include "common/Vector.h"

namespace d2d
{

struct Colorf;
class Vector;
class ShaderImpl;
class IShader;
class ShapeShader;
class SpriteShader;
class FontShader;

class ShaderMgr : public ShaderMgrBase
{
public:
	static ShaderMgr* Instance();

	void SetSpriteColor(const Colorf& multi, const Colorf& add);
	void SetSpriteColorTrans(const Colorf& r_trans, const Colorf& g_trans, const Colorf& b_trans);
	void SetShapeColor(const Colorf& col);

	void sprite();
	void shape();
	void null();

	int GetTexID() const;
	int GetFboID() const;

	void SetTexture(int tex);
	void SetFBO(int fbo);

	void Draw(const float vb[16], int texid);
	void Draw(const Vector vertices[4], const Vector texcoords[4], int texid);

	int GetVersion() const;

	void SetBufferData(bool open);
	bool IsOpenBufferData() const;

	void SetModelView(const Vector& offset, float scale);
	void GetModelView(Vector& offset, float& scale);

	void SetSpriteShader(d2d::SpriteShader* shader, bool delete_old = true);

	ShapeShader* GetShapeShader() { return m_shape_shader; }

private:
	ShaderMgr();
	~ShaderMgr();

private:
	static ShaderMgr* m_instance;

private:
	ShapeShader* m_shape_shader;
	SpriteShader* m_sprite_shader;
	FontShader* m_font_shader;

	int m_version;

	// cache model view
	Vector m_offset;
	float m_scale;

}; // ShaderMgr

}

#endif // _D2D_SHADER_MGR_H_