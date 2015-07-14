#include "StageCanvas.h"

#include "Symbol.h"
#include "config.h"

namespace libsketch
{

StageCanvas::StageCanvas(d2d::EditPanel* edit_panel, d2d::MultiSpritesImpl* sprites_impl, 
						 d2d::LibraryPanel* library)
	: e3d::StageCanvas(edit_panel)
	, m_sprites_impl(sprites_impl)
	, m_library(library)
{
}

ivec2 StageCanvas::TransPos3ProjectToScreen(const vec3& proj) const
{
	vec3 pos = GetCamera3().GetModelViewMat() * proj;
	return e3d::ViewFrustum::TransPos3ProjectToScreen(pos, m_screen_width, m_screen_height);
}

vec3 StageCanvas::TransPos3ScreenToProject(const ivec2& scr, float proj_z) const
{
	vec3 pos = GetCamera3().GetModelViewMat() * vec3(0, 0, proj_z);
	return e3d::ViewFrustum::TransPos3ScreenToProject(scr, pos.z, m_screen_width, m_screen_height);
}

vec3 StageCanvas::TransPos3ScreenToDir(const ivec2& screen) const
{
	return e3d::ViewFrustum::TransPos3ScreenToDir(screen, m_screen_width, m_screen_height);
}

void StageCanvas::InitGL()
{
	e3d::StageCanvas::InitGL();
	InitDefaultSymbol();
}

void StageCanvas::OnSize(int w, int h)
{
	e3d::StageCanvas::OnSize(w, h);
	m_screen_width = w;
	m_screen_height = h;
}

void StageCanvas::OnDrawSprites() const
{
	e3d::ShaderMgr::Instance()->SetModelView(GetCamera3().GetModelViewMat());
	DrawBackground();
 	DrawSprites();
 
  	m_stage->DrawEditOP();
}

void StageCanvas::InitDefaultSymbol()
{
	e3d::ISurface* surface;

	surface = new e3d::Cone(2, 1);
	LoadSymbolToLib(surface, "Cone");

	surface = new e3d::Sphere(1);
	LoadSymbolToLib(surface, "Sphere");

	surface = new e3d::Torus(0.5f, 0.2f);
	LoadSymbolToLib(surface, "Torus");

	surface = new e3d::TrefoilKnot(1);
	LoadSymbolToLib(surface, "TrefoilKnot");

	surface = new e3d::MobiusStrip(0.2f);
	LoadSymbolToLib(surface, "MobiusStrip");

	surface = new e3d::KleinBottle(0.1f);
	LoadSymbolToLib(surface, "KleinBottle");
}

void StageCanvas::LoadSymbolToLib(e3d::ISurface* surface, const char* name)
{
	e3d::AABB aabb;
	e3d::ModelParametric* model = new e3d::ModelParametric(surface, aabb);

	libsketch::Symbol* symbol = new libsketch::Symbol();
	symbol->SetAABB(aabb);
	symbol->SetName(name);
	symbol->SetModel(model);

	std::string filepath = FILE_TAG;
	filepath += ".json";
	symbol->SetFilepath(filepath);

	m_library->AddSymbol(symbol);
}

void StageCanvas::DrawBackground() const
{
	e3d::DrawCross(vec3(0, 0, 0), vec3(10, 10, 10), d2d::LIGHT_RED);
//  	e3d::DrawGrids(vec3(-10, -10, 0), vec3(10, 10, 0), vec3(0.5f, 0.5f, FLT_MAX), 
//  		d2d::LIGHT_RED);
}

void StageCanvas::DrawSprites() const
{
	std::vector<d2d::ISprite*> sprites;
	m_sprites_impl->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];
		if (!sprite->visiable)
			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprite);
	}
}

}