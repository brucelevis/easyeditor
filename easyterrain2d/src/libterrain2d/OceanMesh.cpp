#include "OceanMesh.h"
#include "MeshShape.h"
#include "config.h"

namespace eterrain2d
{

OceanMesh::OceanMesh(const libshape::PolygonShape* shape, 
					 const d2d::ImageSymbol* image)
	 : m_shape(shape)
	 , m_image0(image)
	 , m_image1(NULL)
{
	m_row = MESH_ROW;
	m_col = MESH_COL;

	m_wave_open = true;
	m_wave_speed = WAVE_SPEED * 0.1f;
	m_wave_height = WAVE_HEIGHT;

	m_uv_move_open = true;
	m_texcoords_spd.set(UV_SPEED_X * 0.01f, UV_SPEED_Y * 0.01f);
	m_texcoords_base.set(0, 0);

	m_blend_open = true;
	m_blend_base = 0;
	m_blend_speed = BLEND_SPEED * 0.01f;

	Build();
}

OceanMesh::~OceanMesh()
{
	Clear();
}

void OceanMesh::Build()
{
	Clear();

	std::vector<d2d::Vector> bound;
	d2d::Math::removeDuplicatePoints(m_shape->GetVertices(), bound);

	d2d::Rect r = CalBoundRegion(bound);

	std::vector<d2d::Vector> segs;
	CalSegments(r, segs);

	std::vector<d2d::Vector> tris_vertices;
	d2d::Triangulation::lines(bound, segs, tris_vertices);
	if (tris_vertices.empty()) {
		return;
	}

	std::vector<d2d::Vector> tris_texcoords;
	CalTrisTexcords(r, tris_vertices, tris_texcoords);

	BuildGrids(r, tris_vertices, tris_texcoords);
}

void OceanMesh::SetSmallGridSize(int row, int col)
{
	if (m_row != row || m_col != col) {
		m_row = row;
		m_col = col;
		Build();
	}
}

void OceanMesh::SetWaveInfo(float speed, float height)
{
	m_wave_speed = speed;
	m_wave_height = height;
}

void OceanMesh::SetTexcoordsSpeed(const d2d::Vector& speed)
{
	m_texcoords_spd = speed;
}

void OceanMesh::SetBlendSpeed(float spd)
{
	m_blend_speed = spd;
}

void OceanMesh::Update(float dt)
{
	static float during = 0;
	if (m_uv_move_open) {
		UpdateUVMove(dt);
		Build();
	}
	if (m_wave_open) {
		UpdateWave(during);
	}
	if (m_blend_open) {
		UpdateBlend(dt);
	}
	during += dt;
}

void OceanMesh::Draw(bool draw_tris) const
{
	d2d::Matrix mt;
	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	if (m_blend_open && m_image1) {
		shader->SetSpriteColor(d2d::Colorf(1, 1, 1, m_blend_base), d2d::Colorf(0, 0, 0, 0));
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(mt, m_image0->getTextureID());
		}
		shader->SetSpriteColor(d2d::Colorf(1, 1, 1, 1 - m_blend_base), d2d::Colorf(0, 0, 0, 0));
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(mt, m_image1->getTextureID());
		}
	} else {
		shader->SetSpriteColor(d2d::Colorf(1, 1, 1, 1), d2d::Colorf(0, 0, 0, 0));
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawTexture(mt);
		}
	}

	if (draw_tris) {
		for (int i = 0, n = m_grids.size(); i < n; ++i) {
			m_grids[i]->DrawInfoXY();
		}
	}
}

void OceanMesh::OpenWave(bool open) 
{ 
	m_wave_open = open; 
}

void OceanMesh::OpenUVMove(bool open) 
{ 
	if (!open) {
		m_texcoords_base.set(0, 0);
	}
	m_uv_move_open = open; 
}

void OceanMesh::OpenBlend(bool open) 
{ 
	if (!open) {
		m_blend_base = 0;
	}
	m_blend_open = open; 
}

void OceanMesh::SetImage1(const d2d::ImageSymbol* image)
{
	if (m_image0->getSize().xLength() != image->getSize().xLength() ||
		m_image0->getSize().yLength() != image->getSize().yLength()) {
		return;
	}

	m_image1 = image;
}

void OceanMesh::Clear()
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		m_grids[i]->Release();
	}
	m_grids.clear();
}

d2d::Rect OceanMesh::CalBoundRegion(const std::vector<d2d::Vector>& bound)
{
	d2d::Rect r;
	for (int i = 0, n = bound.size(); i < n; ++i) {
		r.combine(bound[i]);
	}
	return r;
}

void OceanMesh::CalSegments(const d2d::Rect& r, std::vector<d2d::Vector>& segs)
{
	int img_w = m_image0->getSize().xLength(),
		img_h = m_image0->getSize().yLength();
	float dw = img_w / m_col,
		  dh = img_h / m_row;
	for (float x = r.xMin; x < r.xMax; x += dw) {
		segs.push_back(d2d::Vector(x, r.yMin - 1));
		segs.push_back(d2d::Vector(x, r.yMax + 1));
	}
	for (float y = r.yMin; y < r.yMax; y += dh) {
		segs.push_back(d2d::Vector(r.xMin - 1, y));
		segs.push_back(d2d::Vector(r.xMax + 1, y));
	}

	if (m_texcoords_base.x != 0) {
		float offset = m_texcoords_base.x * img_w;
		for (float x = r.xMin + offset; x < r.xMax; x += img_w) {
			segs.push_back(d2d::Vector(x, r.yMin - 1));
			segs.push_back(d2d::Vector(x, r.yMax + 1));
		}
	}
	if (m_texcoords_base.y != 0) {
		float offset = m_texcoords_base.y * img_h;
		for (float y = r.yMin + offset; y < r.yMax; y += img_h) {
			segs.push_back(d2d::Vector(r.xMin - 1, y));
			segs.push_back(d2d::Vector(r.xMax + 1, y));
		}
	}
}

void OceanMesh::CalTrisTexcords(const d2d::Rect& r, 
								const std::vector<d2d::Vector>& tris_vertices,
								std::vector<d2d::Vector>& texcoords)
{
	float img_w = m_image0->getSize().xLength(),
		  img_h = m_image0->getSize().yLength();

	d2d::Vector left_low;
	left_low.x = r.xMin - (1 - m_texcoords_base.x) * img_w;
	left_low.y = r.yMin - (1 - m_texcoords_base.y) * img_h;
	assert(tris_vertices.size() % 3 == 0);
	for (int i = 0, n = tris_vertices.size(); i < n; i += 3)
	{
		d2d::Vector center = tris_vertices[i] + tris_vertices[i+1] + tris_vertices[i+2];
		center /= 3;

		d2d::Vector base;
		int ix = (center.x - left_low.x) / img_w,
			iy = (center.y - left_low.y) / img_h;
		base.x = left_low.x + img_w * ix;
		base.y = left_low.y + img_h * iy;

		for (int j = 0; j < 3; ++j) {
			float tx = (tris_vertices[i+j].x - base.x) / img_w,
				  ty = (tris_vertices[i+j].y - base.y) / img_h;
			texcoords.push_back(d2d::Vector(tx, ty));
		}
	}
}

void OceanMesh::BuildGrids(const d2d::Rect& region, 
						   const std::vector<d2d::Vector>& vertices, 
						   const std::vector<d2d::Vector>& texcoords)
{
	float img_w = m_image0->getSize().xLength(),
		  img_h = m_image0->getSize().yLength();
	int cx = std::ceil(region.xLength() / img_w),
		cy = std::ceil(region.yLength() / img_h);
	std::vector<MeshShape*> grids;
	grids.resize(cx * cy, NULL);
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		d2d::Vector center = vertices[i] + vertices[i+1] + vertices[i+2];
		center /= 3;

		int ix = (center.x - region.xMin) / img_w,
			iy = (center.y - region.yMin) / img_h;
		MeshShape** grid = &grids[iy * cx + ix];
		if (!*grid) {
			*grid = new MeshShape(*m_image0->getImage());
		}
		(*grid)->InsertTriangle(&vertices[i], &texcoords[i]);
	}

	for (int i = 0, n = grids.size(); i < n; ++i) {
		if (grids[i]) {
			m_grids.push_back(grids[i]);
		}
	}
}

void OceanMesh::UpdateWave(float during)
{
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		MeshShape* grid = m_grids[i];
		const std::vector<emesh::Triangle*>& tris = grid->GetTriangles();
		for (int j = 0, m = tris.size(); j < m; ++j) {
			emesh::Triangle* tri = tris[j];
			for (int k = 0; k < 3; ++k) {
				emesh::Node* n = tri->nodes[k];
				float dis = n->ori_xy.y - m_wave_speed * during;
				n->xy.y = n->ori_xy.y + m_wave_height * cos(dis);
			}
		}
	}
}

void OceanMesh::UpdateUVMove(float dt)
{
	m_texcoords_base += m_texcoords_spd * dt;
	if (m_texcoords_base.x >= 1) m_texcoords_base.x -= 1;
	if (m_texcoords_base.x < 0) m_texcoords_base.x += 1;
	if (m_texcoords_base.y >= 1) m_texcoords_base.y -= 1;
	if (m_texcoords_base.y < 0) m_texcoords_base.y += 1;
}

void OceanMesh::UpdateBlend(float dt)
{
	m_blend_base += m_blend_speed * dt;
	if (m_blend_base > 1) {
		m_blend_speed = -m_blend_speed;
		m_blend_base = 1;
	}
	if (m_blend_base < 0) {
		m_blend_speed = -m_blend_speed;
		m_blend_base = 0;
	}
}

}