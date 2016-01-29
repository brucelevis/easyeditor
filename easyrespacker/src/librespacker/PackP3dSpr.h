#ifndef _LIBRESPACKER_PACK_P3D_SPR_H_
#define _LIBRESPACKER_PACK_P3D_SPR_H_

#include "IPackNode.h"

#include <drag2d.h>

namespace librespacker
{

class PackP3dSpr : public IPackNode
{
public:
	const IPackNode* p3d;

	bool loop;
	bool local;
	bool alone;
	bool reuse;

public:
	PackP3dSpr() {}
	PackP3dSpr(int id);

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const d2d::TexturePacker& tp, float scale) const;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<d2d::Image*>& images);

	virtual int SizeOfPackToBin() const;
	virtual void PackToBin(uint8_t** ptr,
		const d2d::TexturePacker& tp, float scale) const;
	virtual int SizeOfUnpackFromBin() const;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<d2d::Image*>& images);

}; // PackP3dSpr

}

#endif // _LIBRESPACKER_PACK_P3D_SPR_H_