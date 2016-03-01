#ifndef _EASYEDITOR_ATOMIC_TYPE_H_
#define _EASYEDITOR_ATOMIC_TYPE_H_

namespace ee
{

enum AtomicType
{
	AT_MOVE = 0,
	AT_ROTATE,
	AT_INSERT,
	AT_DELETE,
	AT_SCALE,
	AT_SHEAR,
	AT_MIRROR
};

}

#endif // _EASYEDITOR_ATOMIC_TYPE_H_