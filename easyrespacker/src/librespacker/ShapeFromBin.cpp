#include "ShapeFromBin.h"
#include "pack_unpack.h"
#include "typedef.h"

#include <ee/trans_color.h>

#include <spritepack.h>

namespace erespacker
{

int ShapeFromBin::Size(const PackShape* shape)
{
	return SIZEOF_SHAPE + shape->vertices.size() * sizeof(int32_t) * 2;
}

void ShapeFromBin::Unpack(uint8_t** ptr, PackShape* shape)
{
	uint8_t shape_type;
	unpack(shape_type, ptr);
	
	uint32_t color;
	unpack(color, ptr);
	shape->color = ee::TransColor(color, ee::PT_RGBA);

	uint16_t num;
	unpack(num, ptr);
	
	shape->vertices.reserve(num);
	for (int i = 0; i < num; ++i) {
		int32_t x, y;
		unpack(x, ptr);
		unpack(y, ptr);

		sm::vec2 pos;
		pos.x = (float)x / SCALE;
		pos.y =-(float)y / SCALE;

		shape->vertices.push_back(pos);
	}
}

}