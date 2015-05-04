#include "MultiSpritesPropertyImpl.h"

#include "dataset/AbstractBV.h"
#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "common/color_config.h"

#include <algorithm>

namespace d2d
{

const wxChar* MultiSpritesPropertyImpl::BOOL_3TYPE_LABELS[] 
	= { wxT("[?]"), wxT("True"), wxT("False"), NULL };		

const wxChar* MultiSpritesPropertyImpl::ALIGN_LABELS[] 
	= { wxT("none"), wxT("left"), wxT("right"), wxT("up"), wxT("down"), wxT("center x"), wxT("center y"), NULL };

const wxChar* MultiSpritesPropertyImpl::CENTER_LABELS[] 
	= { wxT("none"), wxT("horizontal"), wxT("vertical"), wxT("relative"), NULL };

MultiSpritesPropertyImpl::MultiSpritesPropertyImpl(const std::vector<ISprite*>& sprites)
{
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		sprites[i]->Retain();
	}
	m_sprites = sprites;
}

MultiSpritesPropertyImpl::~MultiSpritesPropertyImpl()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->Release();
	}
}

std::string MultiSpritesPropertyImpl::GetTag() const
{
	if (m_sprites.empty()) {
		return "";
	}

	std::string tag = m_sprites[0]->tag;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (tag != m_sprites[i]->tag) {
			return "[different]";
		}
	}
	return tag;
}

const wxChar* MultiSpritesPropertyImpl::GetClip() const
{
	if (m_sprites.empty()) {
		return BOOL_3TYPE_LABELS[e_bool_different];
	}

	bool clip = m_sprites[0]->clip;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (clip != m_sprites[i]->clip) {
			return BOOL_3TYPE_LABELS[e_bool_different];
		}
	}
	return clip ? BOOL_3TYPE_LABELS[e_bool_true] : BOOL_3TYPE_LABELS[e_bool_false];
}

Vector MultiSpritesPropertyImpl::GetPosition() const
{
	if (m_sprites.empty()) {
		Vector p;
		p.setInvalid();
		return p;
	}

	Vector p = m_sprites[0]->getPosition();
	for (size_t i = 1, n = m_sprites.size(); i < n; ++i)
	{
		if (p != m_sprites[i]->getPosition())
		{
			p.setInvalid();
			break;
		}
	}
	return p;
}

d2d::Colorf MultiSpritesPropertyImpl::GetMultiColor() const
{
	if (m_sprites.empty()) {
		return WHITE;
	}

	Colorf col = m_sprites[0]->multiCol;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->multiCol) {
			return WHITE;
		}
	}
	return col;
}

d2d::Colorf MultiSpritesPropertyImpl::GetAddColor() const
{
	if (m_sprites.empty()) {
		return BLACK;
	}
	
	Colorf col = m_sprites[0]->addCol;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->addCol) {
			return BLACK;
		}
	}
	return col;
}

Colorf MultiSpritesPropertyImpl::GetTransColorR() const
{
	if (m_sprites.empty()) {
		return RED;
	}

	Colorf col = m_sprites[0]->r_trans;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->r_trans) {
			return RED;
		}
	}
	return col;
}

Colorf MultiSpritesPropertyImpl::GetTransColorG() const
{
	if (m_sprites.empty()) {
		return GREEN;
	}

	Colorf col = m_sprites[0]->g_trans;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->g_trans) {
			return GREEN;
		}
	}
	return col;
}

Colorf MultiSpritesPropertyImpl::GetTransColorB() const
{
	if (m_sprites.empty()) {
		return BLUE;
	}

	Colorf col = m_sprites[0]->b_trans;
	for (int i = 1, n = m_sprites.size(); i < n; ++i) {
		if (col != m_sprites[i]->b_trans) {
			return BLUE;
		}
	}
	return col;
}

void MultiSpritesPropertyImpl::SetTag(const wxString& tag)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->tag = tag;
	}
}

void MultiSpritesPropertyImpl::SetClip(int clip)
{
	Bool3Type type = (Bool3Type)clip;
	if (type == e_bool_true) {
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_sprites[i]->clip = true;
		}
	} else if (type == e_bool_false) {
		for (int i = 0, n = m_sprites.size(); i < n; ++i) {
			m_sprites[i]->clip = false;
		}
	}
}

void MultiSpritesPropertyImpl::SetPosX(float x)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* spr = m_sprites[i];
		spr->setTransform(Vector(x, spr->getPosition().y), spr->getAngle());
	}
}

void MultiSpritesPropertyImpl::SetPosY(float y)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		ISprite* spr = m_sprites[i];
		spr->setTransform(Vector(spr->getPosition().x, y), spr->getAngle());
	}
}

void MultiSpritesPropertyImpl::SetColorMul(const Colorf& col)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->multiCol = col;
	}
}

void MultiSpritesPropertyImpl::SetColorAdd(const Colorf& col)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->addCol = col;
	}
}

void MultiSpritesPropertyImpl::SetColorAlpha(float alpha)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->multiCol.a = alpha;
	}
}

void MultiSpritesPropertyImpl::OnAlign(int align)
{
	AlignType type = (AlignType)align;

	if (type == e_align_left)
	{
		float left = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float l = sprite->getPosition().x - sprite->getSymbol().getSize().xLength()*0.5f;
			if (l < left)
				left = l;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float x = left + sprite->getSymbol().getSize().xLength()*0.5f;
			m_sprites[i]->setTransform(Vector(x, sprite->getPosition().y), sprite->getAngle());
		}
	}
	else if (type == e_align_right)
	{
		float right = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float r = sprite->getPosition().x + sprite->getSymbol().getSize().xLength()*0.5f;
			if (r > right)
				right = r;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float x = right - sprite->getSymbol().getSize().xLength()*0.5f;
			m_sprites[i]->setTransform(Vector(x, sprite->getPosition().y), sprite->getAngle());
		}
	}
	else if (type == e_align_up)
	{
		float up = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float u = sprite->getPosition().y + sprite->getSymbol().getSize().yLength()*0.5f;
			if (u > up)
				up = u;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float y = up - sprite->getSymbol().getSize().yLength()*0.5f;
			m_sprites[i]->setTransform(Vector(sprite->getPosition().x, y), sprite->getAngle());
		}
	}
	else if (type == e_align_down)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float d = sprite->getPosition().y - sprite->getSymbol().getSize().yLength()*0.5f;
			if (d < down)
				down = d;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float y = down + sprite->getSymbol().getSize().yLength()*0.5f;
			m_sprites[i]->setTransform(Vector(sprite->getPosition().x, y), sprite->getAngle());
		}
	}
	else if (type == e_align_centerx)
	{
		float down = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float y = m_sprites[i]->getPosition().y;
			if (y < down)
				down = y;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			m_sprites[i]->setTransform(Vector(sprite->getPosition().x, down), sprite->getAngle());
		}
	}
	else if (type == e_align_centery)
	{
		float left = FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float x = m_sprites[i]->getPosition().x;
			if (x < left)
				left = x;
		}
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			m_sprites[i]->setTransform(Vector(left, sprite->getPosition().y), sprite->getAngle());
		}
	}
}

void MultiSpritesPropertyImpl::OnCenter(int center)
{
	CenterType type = (CenterType)center;

	if (type == e_center_horizontal)
	{
		float left = FLT_MAX, right = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float x = sprite->getPosition().x;
			if (x < left) left = x;
			if (x > right) right = x;
		}

		std::vector<ISprite*> sorted(m_sprites);
		std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_x));
		const float space = (right - left) / (sorted.size() - 1);
		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			ISprite* sprite = sorted[i];
			sprite->setTransform(Vector(left + space * i, sprite->getPosition().y), sprite->getAngle());
		}
	}
	else if (type == e_center_vertical)
	{
		float down = FLT_MAX, up = -FLT_MAX;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			float y = sprite->getPosition().y;
			if (y < down) down = y;
			if (y > up) up = y;
		}

		std::vector<ISprite*> sorted(m_sprites);
		std::sort(sorted.begin(), sorted.end(), SpriteCmp(SpriteCmp::e_y));
		const float space = (up - down) / (sorted.size() - 1);
		for (size_t i = 0, n = sorted.size(); i < n; ++i)
		{
			ISprite* sprite = sorted[i];
			sprite->setTransform(Vector(sprite->getPosition().x, down + space * i), sprite->getAngle());
		}
	}
	else if (type == e_center_relative)
	{
		ISprite* base = NULL;
		float maxArea = 0;
		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			float area = m_sprites[i]->getBounding()->area();
			if (area > maxArea)
			{
				maxArea = area;
				base = m_sprites[i];
			}
		}

		for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		{
			ISprite* sprite = m_sprites[i];
			if (sprite != base)
				sprite->setTransform(base->getPosition(), sprite->getAngle());
		}
	}
}

void MultiSpritesPropertyImpl::OnPosChange(float dx, float dy)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* spr = m_sprites[i];
		d2d::Vector pos = spr->getPosition();
		pos.x += dx;
		pos.y += dy;
		spr->setTransform(pos, spr->getAngle());
	}
}

void MultiSpritesPropertyImpl::OnAngleChange(float angle)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* spr = m_sprites[i];
		spr->setTransform(spr->getPosition(), spr->getAngle() + angle);
	}
}

void MultiSpritesPropertyImpl::OnScaleChange(float dx, float dy)
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i)
	{
		ISprite* s = m_sprites[i];
		d2d::Vector scale = s->getScale();
		scale.x *= dx;
		scale.y *= dy;
		s->setScale(scale.x, scale.y);
	}
}

}