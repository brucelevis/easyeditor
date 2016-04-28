#include "MeshRenderer.h"
#include "Mesh.h"
#include "Triangle.h"
#include "color_config.h"

#include <ee/Vector.h>
#include <ee/EE_RVG.h>
#include <ee/SpriteRenderer.h>
#include <ee/RenderContextStack.h>
#include <ee/GL.h>
#include <ee/trans_color.h>
#include <ee/Math2D.h>

#include <shaderlab.h>
#include <dtex_facade.h>

#include <set>
#include <vector>

namespace emesh
{

void MeshRenderer::DrawInfoUV(const Mesh* mesh)
{
	std::set<ee::Vector, ee::VectorCmp> unique;
	std::vector<ee::Vector> tmp(3);
	const std::vector<Triangle*>& tris = mesh->GetTriangles();
	float w = mesh->GetWidth(),
		  h = mesh->GetHeight();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* tri = tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i].x = (tri->nodes[i]->uv.x - 0.5f) * w;
			tmp[i].y = (tri->nodes[i]->uv.y - 0.5f) * h;
			unique.insert(tmp[i]);
		}
		ee::RVG::Color(RED);
		ee::RVG::Polyline(tmp, true);
	}
	std::vector<ee::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	ee::RVG::Color(BLUE);
	ee::RVG::Circles(nodes, mesh->GetNodeRegion(), true);
}

void MeshRenderer::DrawInfoXY(const Mesh* mesh)
{
	std::set<ee::Vector, ee::VectorCmp> unique;
	std::vector<ee::Vector> tmp(3);
	const std::vector<Triangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* tri = tris[i];
		for (int i = 0; i < 3; ++i)
		{
			tmp[i] = tri->nodes[i]->xy;
			unique.insert(tmp[i]);
		}
		ee::RVG::Color(RED);
		ee::RVG::Polyline(tmp, true);
	}
	std::vector<ee::Vector> nodes;
	copy(unique.begin(), unique.end(), back_inserter(nodes));
	ee::RVG::Color(BLUE);
	ee::RVG::Circles(nodes, mesh->GetNodeRegion(), true);
}

void MeshRenderer::DrawTexture(const Mesh* mesh, const ee::RenderParams& params)
{
	sl::ShaderMgr::Instance()->GetShader()->Commit();

	ee::RenderContextStack* rc = ee::RenderContextStack::Instance();

	ee::Vector ori_offset;
	float ori_scale;
	rc->GetModelView(ori_offset, ori_scale);

	int ori_width, ori_height;
	rc->GetProjection(ori_width, ori_height);

	rc->SetModelView(ee::Vector(0, 0), 1);
	int edge = dtexf_c1_get_texture_size();
	rc->SetProjection(edge, edge);
	ee::GL::Viewport(0, 0, edge, edge);

	DrawMeshToTmp(mesh, params);

	rc->SetModelView(ori_offset, ori_scale);
	rc->SetProjection(ori_width, ori_height);
	ee::GL::Viewport(0, 0, ori_width, ori_height);

	DrawTmpToScreen(mesh, params);
}

void MeshRenderer::DrawMesh(const Mesh* mesh, const ee::RenderParams& params, int texid)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(ee::color2int(params.color.multi, ee::PT_ABGR),
		ee::color2int(params.color.add, ee::PT_ABGR));
	shader->SetColorMap(ee::color2int(params.color.r, ee::PT_ABGR),
		ee::color2int(params.color.g, ee::PT_ABGR),
		ee::color2int(params.color.b, ee::PT_ABGR));

	int dst_edge = dtexf_c1_get_texture_size();
	float ori_w = mesh->GetWidth(),
		  ori_h = mesh->GetHeight();

	const std::vector<Triangle*>& tris = mesh->GetTriangles();
	for (int i = 0, n = tris.size(); i < n; ++i)
	{
		Triangle* tri = tris[i];
		ee::Vector vertices[4], texcoords[4];
		for (int i = 0; i < 3; ++i)
		{
			vertices[i] = ee::Math2D::TransVector(tri->nodes[i]->xy, params.mt);
			texcoords[i].x = (tri->nodes[i]->uv.x * ori_w - ori_w * 0.5f + dst_edge * 0.5f) / dst_edge;
			texcoords[i].y = (tri->nodes[i]->uv.y * ori_h - ori_h * 0.5f + dst_edge * 0.5f) / dst_edge;
		}
		vertices[3] = vertices[2];
		texcoords[3] = texcoords[2];

		shader->Draw(&vertices[0].x, &texcoords[0].x, texid);
	}
}

void MeshRenderer::DrawMeshToTmp(const Mesh* mesh, const ee::RenderParams& params)
{
	dtexf_c1_bind();
	dtexf_c1_clear(0, -2, 2, 0);

	ee::RenderParams _params = params;
	_params.mt.SetIdentity();
	ee::SpriteRenderer::Draw(mesh->GetBaseSymbol(), _params);

	sl::ShaderMgr::Instance()->GetShader()->Commit();

	dtexf_c1_unbind();
}

void MeshRenderer::DrawTmpToScreen(const Mesh* mesh, const ee::RenderParams& params)
{
	DrawMesh(mesh, params, dtexf_c1_get_texture_id());
}

}