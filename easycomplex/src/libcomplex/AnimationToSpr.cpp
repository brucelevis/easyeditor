#include "AnimationToSpr.h"
#include "NodeToSprite.h"
#include "Symbol.h"
#include "Sprite.h"

#include <easyanim.h>

namespace ecomplex
{

d2d::ISprite* AnimationToSpr::Trans(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);
	if (anim->actions[0].size == 1) {
		return TransComplex(anim);
	} else {
		return TransAnim(anim);
	}
}

d2d::ISprite* AnimationToSpr::TransComplex(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size == 1);

	ecomplex::Symbol* complex = new ecomplex::Symbol;
	const libcoco::PackAnimation::Frame& src = anim->frames[0];
	ecomplex::Symbol* dst = new ecomplex::Symbol;
	for (int i = 0; i < src.parts.size(); ++i) {
		const libcoco::PackAnimation::Part& part = src.parts[i];
		d2d::ISprite* spr = NodeToSprite::Trans(anim->components[part.comp_idx].node);
		TransSprite(spr, part.t);
		dst->m_sprites.push_back(spr);
	}
	dst->InitBounding();
	complex->m_sprites.push_back(new Sprite(dst));
	complex->InitBounding();
	return new Sprite(complex);
}

d2d::ISprite* AnimationToSpr::TransAnim(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);

	libanim::Symbol* anim_symbol = new libanim::Symbol;
	libanim::Symbol::Layer* layer = new libanim::Symbol::Layer;
	for (int i = 0; i < anim->actions[0].size; ++i) {
		const libcoco::PackAnimation::Frame& src = anim->frames[i];
		libanim::Symbol::Frame* frame = new libanim::Symbol::Frame;
		frame->index = i;
		frame->bClassicTween = false;
		for (int j = 0; j < src.parts.size(); ++j) {
			const libcoco::PackAnimation::Part& part = src.parts[j];
			d2d::ISprite* spr = NodeToSprite::Trans(anim->components[part.comp_idx].node);
			TransSprite(spr, part.t);
			frame->sprites.push_back(spr);
		}
		layer->frames.push_back(frame);
	}
	anim_symbol->setFPS(30);
	anim_symbol->m_layers.push_back(layer);
	anim_symbol->InitBounding();
	return new libanim::Sprite(anim_symbol);
}

void AnimationToSpr::TransSprite(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	if (!libcoco::PackAnimation::IsMatrixIdentity(t.mat)) {
		TransSpriteMat(spr, t);
	}
	TransSpriteCol(spr, t);
}

void AnimationToSpr::TransSpriteMat(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	float dx = t.mat[4] / 16.0f,
		dy = -t.mat[5] / 16.0f;

	// no shear
	// 	mat[0] = sx*c;
	// 	mat[1] = sx*s;
	// 	mat[2] = -sy*s;
	// 	mat[3] = sy*c;	

	float angle = atan2((float)t.mat[1], (float)t.mat[0]);
	// #ifdef _DEBUG
	// 	if (t.mat[2] != 0) {
	// 		assert(atan2(-(float)t.mat[2], (float)t.mat[3]) == angle);
	// 	}
	// #endif
	float c = cos(angle), s = sin(angle);
	float sx, sy;
	if (c != 0) {
		sx = t.mat[0] / c / 1024.0f;
		sy = t.mat[3] / c / 1024.0f;
	} else {
		sx = t.mat[1] / s / 1024.0f;
		sy = -t.mat[2] / s / 1024.0f;
	}

	if (fabs(t.mat[0] - sx * c * 1024) >= 1 ||
		fabs(t.mat[1] - sx * s * 1024) >= 1 ||
		fabs(t.mat[2] - -sy * s * 1024) >= 1 ||
		fabs(t.mat[3] - sy * c * 1024) >= 1) {
			// todo use mat for sprite

			sx = sy = 1;
			angle = 0;
	}

	// no scale
	// 	mat[0] = c - ky*s;
	// 	mat[1] = s + ky*c;
	// 	mat[2] = kx*c - s;
	// 	mat[3] = kx*s + c;

	bool xmirror = false, ymirror = false;
	if (sx < 0) {
		xmirror = true;
		sx = -sx;
	}
	if (sy < 0) {
		ymirror = true;
		sy = -sy;
	}
	spr->SetMirror(xmirror, ymirror);

	spr->SetScale(sx, sy);
	spr->SetTransform(d2d::Vector(dx, dy), angle);
}

void AnimationToSpr::TransSpriteCol(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	spr->multiCol = d2d::transColor(t.color, d2d::PT_ARGB);
	spr->addCol = d2d::transColor(t.additive, d2d::PT_ARGB);

	spr->r_trans = d2d::transColor(t.rmap, d2d::PT_ARGB);
	spr->g_trans = d2d::transColor(t.gmap, d2d::PT_ARGB);
	spr->b_trans = d2d::transColor(t.bmap, d2d::PT_ARGB);
}


}