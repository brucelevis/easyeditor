#include <gl/glew.h>

#include "SpriteRenderer.h"
#include "ShaderMgr.h"
#include "ScreenFBO.h"
#include "BlendShader.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "dataset/SpriteTools.h"
#include "view/Camera.h"

namespace d2d
{

SpriteRenderer* SpriteRenderer::m_instance = NULL;

SpriteRenderer::SpriteRenderer()
	: m_fbo(600, 600)
	, m_cam(NULL)
{
}

void SpriteRenderer::Draw(const ISprite* sprite, 
						  const d2d::Matrix& mt,
						  const Colorf& mul, 
						  const Colorf& add,
						  const Colorf& r_trans,
						  const Colorf& g_trans,
						  const Colorf& b_trans,
						  bool multi_draw) const
{
	if (!sprite->visiable)
		return;

	if (!multi_draw || sprite->GetBlendMode() == BM_NORMAL) {
		DrawImpl(sprite, mt, mul, add, r_trans, g_trans, b_trans);
	} else {
		DrawImplBlend(sprite);
	}
}

void SpriteRenderer::Draw(const ISymbol* symbol, 
						  const d2d::Matrix& mt,
						  const Vector& pos, 
						  float angle/* = 0.0f*/, 
						  float xScale/* = 1.0f*/, 
						  float yScale/* = 1.0f*/, 
						  float xShear/* = 0.0f*/, 
						  float yShear/* = 0.0f*/, 
						  const Colorf& mul /*= Colorf(1,1,1,1)*/,
						  const Colorf& add /*= Colorf(0,0,0,0)*/,
						  const Colorf& r_trans,
						  const Colorf& g_trans,
						  const Colorf& b_trans) const
{
	Matrix t;
	t.setTransformation(pos.x, pos.y, angle, xScale, yScale, 0, 0, xShear, yShear);
	t = mt * t;
	symbol->draw(t, mul, add, r_trans, g_trans, b_trans);
}

void SpriteRenderer::DrawImpl(const ISprite* sprite, 
							  const d2d::Matrix& mt,
							  const Colorf& mul, 
							  const Colorf& add,
							  const Colorf& r_trans,
							  const Colorf& g_trans,
							  const Colorf& b_trans) const
{
	Matrix t;
	sprite->GetTransMatrix(t);
	t = mt * t;

	Colorf _mul = cMul(sprite->multiCol, mul),
		_add = cAdd(sprite->addCol, add);

	Colorf _r_trans, _g_trans, _b_trans;

	_r_trans.r = sprite->r_trans.r * r_trans.r + sprite->r_trans.g * g_trans.r + sprite->r_trans.b * b_trans.r;
	_r_trans.g = sprite->r_trans.r * r_trans.g + sprite->r_trans.g * g_trans.g + sprite->r_trans.b * b_trans.g;
	_r_trans.b = sprite->r_trans.r * r_trans.b + sprite->r_trans.g * g_trans.b + sprite->r_trans.b * b_trans.b;

	_g_trans.r = sprite->g_trans.r * r_trans.r + sprite->g_trans.g * g_trans.r + sprite->g_trans.b * b_trans.r;
	_g_trans.g = sprite->g_trans.r * r_trans.g + sprite->g_trans.g * g_trans.g + sprite->g_trans.b * b_trans.g;
	_g_trans.b = sprite->g_trans.r * r_trans.b + sprite->g_trans.g * g_trans.b + sprite->g_trans.b * b_trans.b;

	_b_trans.r = sprite->b_trans.r * r_trans.r + sprite->b_trans.g * g_trans.r + sprite->b_trans.b * b_trans.r;
	_b_trans.g = sprite->b_trans.r * r_trans.g + sprite->b_trans.g * g_trans.g + sprite->b_trans.b * b_trans.g;
	_b_trans.b = sprite->b_trans.r * r_trans.b + sprite->b_trans.g * g_trans.b + sprite->b_trans.b * b_trans.b;

	sprite->getSymbol().draw(t, _mul, _add, _r_trans, _g_trans, _b_trans, sprite);

	SpriteTools::DrawName(sprite, t);
}

void SpriteRenderer::DrawImplBlend(const ISprite* sprite) const
{
//	DrawUnderToTmp(sprite);
 	DrawSprToTmp(sprite);
 	DrawTmpToScreen(sprite);
}

void SpriteRenderer::DrawUnderToTmp(const ISprite* sprite) const
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	FBO& scr_fbo = ScreenFBO::Instance()->GetFBO();

	mgr->sprite();
	mgr->SetFBO(m_fbo.GetFboID());

	mgr->SetBlendMode(BM_NORMAL);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// src 
	Vector src[4];
	Rect src_rect = sprite->GetRect();
	int scr_w = scr_fbo.GetWidth(),
		scr_h = scr_fbo.GetHeight();
	src[0] = m_cam->transPosProjectToScreen(Vector(src_rect.xMin, src_rect.yMin), scr_w, scr_h);
	src[1] = m_cam->transPosProjectToScreen(Vector(src_rect.xMax, src_rect.yMin), scr_w, scr_h);
	src[2] = m_cam->transPosProjectToScreen(Vector(src_rect.xMax, src_rect.yMax), scr_w, scr_h);
	src[3] = m_cam->transPosProjectToScreen(Vector(src_rect.xMin, src_rect.yMax), scr_w, scr_h);
	for (int i = 0; i < 4; ++i) {
		src[i].y = scr_h - 1 - src[i].y;
		src[i].x /= scr_w;
		src[i].y /= scr_h;
		src[i].x = std::min(std::max(0.0f, src[i].x), 1.0f);
		src[i].y = std::min(std::max(0.0f, src[i].y), 1.0f);
	}

	// dst
	Vector dst[4];
	Rect dst_rect = sprite->getSymbol().getSize();
	dst[0] = Vector(dst_rect.xMin, dst_rect.yMin);
	dst[1] = Vector(dst_rect.xMax, dst_rect.yMin);
	dst[2] = Vector(dst_rect.xMax, dst_rect.yMax);
	dst[3] = Vector(dst_rect.xMin, dst_rect.yMax);

	Vector offset;
	float scale;
	mgr->GetModelView(offset, scale);

	mgr->SetModelView(Vector(0, 0), 1);
	Rect r = sprite->getSymbol().getSize();
	mgr->SetProjection(r.xLength(), r.yLength());
	glViewport(0, 0, r.xLength(), r.yLength());

	BlendShader* blend_shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
	blend_shader->SetBaseTexID(scr_fbo.GetTexID());
 	blend_shader->DrawBlend(dst, src, src, scr_fbo.GetTexID());

	mgr->Commit();

	mgr->SetModelView(offset, scale);
	mgr->SetProjection(scr_fbo.GetWidth(), scr_fbo.GetHeight());
	glViewport(0, 0, scr_fbo.GetWidth(), scr_fbo.GetHeight());
}

void SpriteRenderer::DrawSprToTmp(const ISprite* sprite) const
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	FBO& scr_fbo = ScreenFBO::Instance()->GetFBO();

	mgr->sprite();
	mgr->SetFBO(m_fbo.GetFboID());

	mgr->SetBlendMode(sprite->GetBlendMode());

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	Vector offset;
	float scale;
	mgr->GetModelView(offset, scale);

	mgr->SetModelView(Vector(0, 0), 1);
	Rect r = sprite->getSymbol().getSize();
	mgr->SetProjection(r.xLength(), r.yLength());
	glViewport(0, 0, r.xLength(), r.yLength());

	BlendShader* blend_shader = static_cast<BlendShader*>(mgr->GetSpriteShader());
	blend_shader->SetBaseTexID(scr_fbo.GetTexID());
//	blend_shader->SetBaseTexID(m_fbo.GetTexID());

	sprite->getSymbol().draw(Matrix(), Colorf(1, 1, 1, 1), Colorf(0, 0, 0, 0), 
		Colorf(1, 0, 0, 0), Colorf(0, 1, 0, 0), Colorf(0, 0, 1, 0), sprite);

	mgr->Commit();

	mgr->SetModelView(offset, scale);
	mgr->SetProjection(scr_fbo.GetWidth(), scr_fbo.GetHeight());
	glViewport(0, 0, scr_fbo.GetWidth(), scr_fbo.GetHeight());
}

void SpriteRenderer::DrawTmpToScreen(const ISprite* sprite) const
{
	ShaderMgr* mgr = ShaderMgr::Instance();
	FBO& scr_fbo = ScreenFBO::Instance()->GetFBO();

	mgr->sprite();
	mgr->SetFBO(0);
	mgr->SetFBO(scr_fbo.GetFboID());

	mgr->SetBlendMode(BM_NORMAL);

	const d2d::Rect& r_dst = sprite->GetRect();
	float xmin = r_dst.xMin, xmax = r_dst.xMax;
	float ymin = r_dst.yMin, ymax = r_dst.yMax;

	d2d::Rect r_src = sprite->getSymbol().getSize();
	//  		float txmin = r_src.xMin / m_fbo.GetWidth() + 0.5f,
	//  			txmax = r_src.xMax / m_fbo.GetWidth() + 0.5f;
	//  		float tymin = r_src.yMin / m_fbo.GetHeight() + 0.5f,
	//  			tymax = r_src.yMax / m_fbo.GetHeight() + 0.5f;
	float txmin = 0, txmax = r_src.xLength() / m_fbo.GetWidth();
	float tymin = 0, tymax = r_src.yLength() / m_fbo.GetHeight();
	if (BlendShader* blend_shader = dynamic_cast<BlendShader*>(mgr->GetSpriteShader()))
	{
		const float vertices[] = { 
			xmin, ymin, txmin, tymin, txmin, tymin,
			xmin, ymax, txmin, tymax, txmin, tymax,
			xmax, ymax, txmax, tymax, txmax, tymax,
			xmax, ymin, txmax, tymin, txmax, tymin };
		blend_shader->DrawBlend(vertices, m_fbo.GetTexID());
	}
	else
	{
		const float vertices[] = { 
			xmin, ymin, txmin, tymin,
			xmin, ymax, txmin, tymax,
			xmax, ymax, txmax, tymax,
			xmax, ymin, txmax, tymin};
		mgr->Draw(vertices, m_fbo.GetTexID());
	}

	mgr->Commit();
}

SpriteRenderer* SpriteRenderer::Instance()
{
	if (!m_instance) {
		m_instance = new SpriteRenderer();
	}
	return m_instance;
}

} // d2d