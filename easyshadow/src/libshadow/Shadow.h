#ifndef _EASYSHADOW_SHADOW_H_
#define _EASYSHADOW_SHADOW_H_

#include <drag2d.h>

namespace eshadow
{

class Shadow : public d2d::Object
{
public:
	Shadow(float radius);
	~Shadow();

	void Draw(const d2d::Matrix& mt) const;

	void BuildFace();
	void BuildOutterLine();
	void BuildInnerLine(const std::vector<d2d::Vector>& loop);

	const d2d::Rect& GetRegion() const { return m_region; }

	const d2d::Colorf& GetInnerColor() const { return m_inner_color; }
	const d2d::Colorf& GetOuterColor() const { return m_outer_color; }
	void SetInnerColer(const d2d::Colorf& col) { m_inner_color = col; }
	void SetOuterColer(const d2d::Colorf& col) { m_outer_color = col; }	

	void SetRadius(float r) { m_radius = r; }

private:
	void BuildInnerLoop(const std::vector<d2d::Vector>& loop);
	void BuildOuterLoop();

private:
	float m_radius;

	std::vector<d2d::Vector> m_inner_loop;
	std::vector<d2d::Vector> m_outer_loop;

	d2d::Colorf m_inner_color, m_outer_color;

	std::vector<d2d::Vector> m_tris;	
	std::vector<d2d::Colorf> m_colors;	

	d2d::Rect m_region;

}; // Shadow

}

#endif // _EASYSHADOW_SHADOW_H_