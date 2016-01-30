#include "ImageBuilder.h"
#include "IPackNode.h"

namespace librespacker
{

ImageBuilder::ImageBuilder()
{
}

ImageBuilder::~ImageBuilder()
{
	for_each(m_nodes.begin(), m_nodes.end(), DeletePointerFunctor<IPackNode>());	
}

void ImageBuilder::Traverse(d2d::Visitor& visitor) const
{
	for (int i = 0, n = m_nodes.size(); i < n; ++i) {
		bool has_next;
		visitor.Visit(m_nodes[i], has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* ImageBuilder::Create(const d2d::ImageSprite* spr)
{
	PackPicture* node = new PackPicture;
	PackPicture::Quad quad;
	LoadPictureQuad(spr, quad);
	node->quads.push_back(quad);
	m_nodes.push_back(node);
	return node;
}

void ImageBuilder::LoadPictureQuad(const d2d::ImageSprite* img, PackPicture::Quad& quad)
{
	quad.img = img->GetSymbol().GetImage();

	quad.texture_coord[0].Set(0, 0);
	quad.texture_coord[1].Set(0, 1);
	quad.texture_coord[2].Set(1, 1);
	quad.texture_coord[3].Set(1, 0);

	d2d::Rect r = img->GetSymbol().GetSize();	
	quad.screen_coord[0].Set(r.xmin, r.ymin);
	quad.screen_coord[1].Set(r.xmin, r.ymax);
	quad.screen_coord[2].Set(r.xmax, r.ymax);
	quad.screen_coord[3].Set(r.xmax, r.ymin);
	TransScreen(quad, img);
}

void ImageBuilder::TransScreen(PackPicture::Quad& quad, const d2d::Sprite* spr)
{
	// 1. shear
	float hw = quad.img->GetClippedWidth() * 0.5f,
		hh = quad.img->GetClippedHeight() * 0.5f;
	float sx = spr->GetShear().x,
		sy = spr->GetShear().y;
	quad.screen_coord[1].x += sx * hh;
	quad.screen_coord[2].x += sx * hh;
	quad.screen_coord[0].x -= sx * hh;
	quad.screen_coord[3].x -= sx * hh;
	quad.screen_coord[2].y += sy * hw;
	quad.screen_coord[3].y += sy * hw;
	quad.screen_coord[1].y -= sy * hw;
	quad.screen_coord[0].y -= sy * hw;

	// 2. mirror
	bool xMirror, yMirror;
	spr->GetMirror(xMirror, yMirror);
	if (xMirror) {
		for (size_t i = 0; i < 4; ++i)
			quad.screen_coord[i].x = -quad.screen_coord[i].x;
	}
	if (yMirror) {
		for (size_t i = 0; i < 4; ++i)
			quad.screen_coord[i].y = -quad.screen_coord[i].y;
	}

	// 3. scale
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i].x *= spr->GetScale().x;
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i].y *= spr->GetScale().y;

	// 4. rotate
	for (size_t i = 0; i < 4; ++i) {
		d2d::Vector rot = d2d::Math2D::RotateVector(quad.screen_coord[i], spr->GetAngle());
		quad.screen_coord[i] = rot;
	}

	// 5. translate
	d2d::Vector center = spr->GetCenter();
	for (size_t i = 0; i < 4; ++i)
		quad.screen_coord[i] += center;

// 	// flip y
// 	for (size_t i = 0; i < 4; ++i)
// 		quad.screen_coord[i].y = -quad.screen_coord[i].y;
// 
// 	// final scale
// 	const float SCALE = 16;
// 	for (size_t i = 0; i < 4; ++i)
// 		quad.screen_coord[i] *= SCALE;
}

}