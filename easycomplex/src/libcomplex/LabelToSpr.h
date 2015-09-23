#ifndef _LIBCOMPLEX_LABEL_TO_SPR_H_
#define _LIBCOMPLEX_LABEL_TO_SPR_H_

#include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class LabelToSpr
{
public:
	static d2d::ISprite* Trans(const libcoco::PackLabel* label);

}; // LabelToSpr

}

#endif // _LIBCOMPLEX_LABEL_TO_SPR_H_