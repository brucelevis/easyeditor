#include <gl/glew.h>

#include "SpriteBlend.h"
#include "SpriteRenderer.h"
#include "ScreenCache.h"
#include "RenderContextStack.h"
#include "Sprite.h"
#include "ImageSymbol.h"
#include "GL.h"
#include "Math2D.h"
#include "EE_ShaderLab.h"

#include <dtex_facade.h>
#include <shaderlab.h>

#include <assert.h>

namespace ee
{

void SpriteBlend::Draw(const Sprite* spr, const sm::mat4& mt)
{
	assert(spr->rp->shader.blend != BM_NULL);

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	RenderContextStack* rc = RenderContextStack::Instance();

	sm::vec2 ori_offset;
	float ori_scale;
	rc->GetModelView(ori_offset, ori_scale);

	int ori_width, ori_height;
	rc->GetProjection(ori_width, ori_height);

	rc->SetModelView(sm::vec2(0, 0), 1);
	int edge = dtexf_c1_get_texture_size();
	rc->SetProjection(edge, edge);
	GL::Viewport(0, 0, edge, edge);

	DrawSprToTmp(spr, mt);

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	GL::Viewport(0, 0, ori_width, ori_height);

	DrawTmpToScreen(spr, mt);
}

void SpriteBlend::DrawSprToTmp(const Sprite* spr, const sm::mat4& mt)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	sl::BlendShader* shader = static_cast<sl::BlendShader*>(mgr->GetShader(sl::BLEND));

	dtexf_c1_bind();
	dtexf_c1_clear(0, -2, 2, 0);

	mgr->SetShader(sl::BLEND);
	BlendMode mode = spr->rp->shader.blend;
	shader->SetMode(mode);

	const_cast<Sprite*>(spr)->rp->shader.blend = BM_NULL;
	RenderParams params(mt);
	params.set_shader = false;
	SpriteRenderer::Draw(spr, spr, params);
	const_cast<Sprite*>(spr)->rp->shader.blend = mode;

	shader->Commit();

	dtexf_c1_unbind();
}

void SpriteBlend::DrawTmpToScreen(const Sprite* sprite, const sm::mat4& mt)
{
	sm::mat4 t;
	sprite->GetTransMatrix(t);
	t = t * mt;

	Rect r = sprite->GetSymbol().GetSize();

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(r.xmin, r.ymin);
	vertices[1] = sm::vec2(r.xmin, r.ymax);
	vertices[2] = sm::vec2(r.xmax, r.ymax);
	vertices[3] = sm::vec2(r.xmax, r.ymin);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math2D::TransVector(vertices[i], t);
	}

	sm::vec2 texcoords[4];
	int edge = dtexf_c1_get_texture_size();
	for (int i = 0; i < 4; ++i) {
		texcoords[i] = vertices[i] - sprite->GetPosition();
		texcoords[i].x = texcoords[i].x / edge + 0.5f;
		texcoords[i].y = texcoords[i].y / edge + 0.5f;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader(sl::SPRITE2));
	shader->SetColor(0xffffffff, 0);
	shader->SetColorMap(0x000000ff, 0x0000ff00, 0x00ff0000);
	shader->Draw(&vertices[0].x, &texcoords[0].x, dtexf_c1_get_texture_id());
}

}