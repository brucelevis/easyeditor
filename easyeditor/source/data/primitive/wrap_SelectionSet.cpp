#include "wrap_SelectionSet.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"
#include "script_type.h"

namespace ee
{

SpriteSelection* luax_checkSpriteSelection(lua_State* L, int idx)
{
	return luax_checktype<SpriteSelection>(L, idx, "SpriteSelection", SPRITE_SELECTION_DATA_T);
}

int w_SpriteSelection_move(lua_State* L)
{
	SpriteSelection* t = luax_checkSpriteSelection(L, 1);
	float x = (float)luaL_checknumber(L, 2);
	float y = (float)luaL_checknumber(L, 3);
	if (!t) return 0;
	std::vector<Sprite*> sprites;
	t->Traverse(FetchAllVisitor<Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		Sprite* s = sprites[i];
		sm::vec2 pos = s->GetPosition() + sm::vec2(x, y);
		s->SetTransform(pos, s->GetAngle());
	}
	return 0;		
}

int w_SpriteSelection_get(lua_State* L)
{
	SpriteSelection* t = luax_checkSpriteSelection(L, 1);
	if (!t) return 0;
	std::vector<Sprite*> sprites;
	t->Traverse(FetchAllVisitor<Sprite>(sprites));
	if (sprites.empty())
		return 0;
	luax_newtype(L, "Sprite", SPRITE_DATA_T, (void*)sprites[0]);
	return 1;
}

static const luaL_Reg functions[] = {
	{ "move", w_SpriteSelection_move },
	{ "get", w_SpriteSelection_get },
	{ 0, 0 }
};

extern "C" int luaopen_SpriteSelection(lua_State* L)
{
	return luax_register_type(L, "SpriteSelection", functions);
}

}