#ifndef _EASYANIM_LOVE2D_CODE_H_
#define _EASYANIM_LOVE2D_CODE_H_

#include <easybuilder.h>

namespace eanim
{

class Love2dCode
{
public:
	Love2dCode(ebuilder::CodeGenerator& gen);

	void Resolve();

private:
	int Query(const std::string& filepath) const;

private:
	ebuilder::CodeGenerator& m_gen;

	d2d::TexPackerAdapter m_tp_adapter;

}; // Love2dCode

}

#endif // _EASYANIM_LOVE2D_CODE_H_