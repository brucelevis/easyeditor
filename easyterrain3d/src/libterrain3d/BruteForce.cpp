#include "BruteForce.h"

namespace eterrain3d
{

void BruteForce::Draw() const
{
	if (!m_data.pixels) {
		return;
	}

	const float scale = 0.1f;

	e3d::ShaderMgr* mgr = e3d::ShaderMgr::Instance();
	mgr->Shape();

	float col, height;
	for (int y = 0; y < m_data.size - 1; ++y)
	{
		int size = (m_data.size - 1) * 2;
		std::vector<ee::Colorf> colors;
		colors.reserve(size);
		std::vector<sm::vec3> vertices;
		vertices.reserve(size);
		for (int x = 0; x < m_data.size - 1; ++x)
		{
			col = GetTrueHeightAtPoint(x, y) / 255.0f;
			colors.push_back(ee::Colorf(col, col, col));
			height = GetScaledHeightAtPoint(x, y);
			vertices.push_back(sm::vec3(scale*x, scale*y, height));

			col = GetTrueHeightAtPoint(x, y+1) / 255.0f;
			colors.push_back(ee::Colorf(col, col, col));
			height = GetScaledHeightAtPoint(x, y+1);
			vertices.push_back(sm::vec3(scale*x, scale*(y+1), height));
		}

		mgr->DrawShape(GL_TRIANGLE_STRIP, &vertices[0].x, size, &colors[0], true);	
	}
}

}