#ifndef _DRAG2D_SPRITE_RENDERER_H_
#define _DRAG2D_SPRITE_RENDERER_H_

#include "common/Matrix.h"
#include "common/Color.h"

namespace d2d
{

class ISprite;
class ISymbol;
class SpriteBatch;
class Vector;
class Matrix;
class Camera;
class FBO;

class SpriteRenderer
{
public:
	void Draw(const ISprite* sprite, 
			  const Matrix& mt = Matrix(),
			  const Colorf& mul = Colorf(1,1,1,1),
			  const Colorf& add = Colorf(0,0,0,0),
			  const Colorf& r_trans = Colorf(1, 0, 0, 0),
			  const Colorf& g_trans = Colorf(0, 1, 0, 0),
			  const Colorf& b_trans = Colorf(0, 0, 1, 0),
			  bool multi_draw = true) const;
	void DrawWithoutBlend(const ISprite* sprite, const d2d::Matrix& mt) const;

	void InvalidRect(const ISprite* sprite, const Matrix& mt = Matrix());

	void Draw(const ISymbol* symbol, 
			  const d2d::Matrix& mt = Matrix(), 
			  const Vector& pos = Vector(0, 0),
			  float angle = 0.0f, 
			  float xScale = 1.0f, 
			  float yScale = 1.0f,
			  float xShear = 0.0f, 
			  float yShear = 0.0f, 
			  const Colorf& mul = Colorf(1,1,1,1),
			  const Colorf& add = Colorf(0,0,0,0),
			  const Colorf& r_trans = Colorf(1, 0, 0, 0),
			  const Colorf& g_trans = Colorf(0, 1, 0, 0),
			  const Colorf& b_trans = Colorf(0, 0, 1, 0)) const;

	void SetCamera(const Camera* cam) { m_cam = cam; }
	const Camera* GetCamera() const { return m_cam; }

	static SpriteRenderer* Instance();

private:
	SpriteRenderer();

	void DrawImpl(const ISprite* sprite, 
  				  const Matrix& mt = Matrix(),
  				  const Colorf& mul = Colorf(1,1,1,1),
  				  const Colorf& add = Colorf(0,0,0,0),
  				  const Colorf& r_trans = Colorf(1, 0, 0, 0),
  				  const Colorf& g_trans = Colorf(0, 1, 0, 0),
  				  const Colorf& b_trans = Colorf(0, 0, 1, 0)) const;

private:
	const Camera* m_cam;

private:
	static SpriteRenderer* m_instance;
	
}; // SpriteRenderer

}

#endif // _DRAG2D_SPRITE_RENDERER_H_