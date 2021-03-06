#include "NodeFromBin.h"
#include "pack_unpack.h"

#include <ee/Math2D.h>

namespace erespacker
{

float NodeFromBin::TransTime(int time)
{
	return time * 0.001f;
}

float NodeFromBin::TransDegree(int deg)
{
	return deg * SM_DEG_TO_RAD;
}

float NodeFromBin::TransFloatX100(int f)
{
	return f * 0.01f;
}

float NodeFromBin::TransFloatX1024(int f)
{
	return f / 1024.0f;
}

bool NodeFromBin::TransBool(int b)
{
	return b == 0 ? false : true;
}

int NodeFromBin::SizeVertices(const std::vector<sm::vec2>& vertices)
{
	return vertices.size() * sizeof(int32_t) * 2;
}

void NodeFromBin::UnpackVertices(std::vector<sm::vec2>& vertices, uint8_t** ptr, 
								 bool reverse_y, int scale)
{
	uint16_t num;
	unpack(num, ptr);

	vertices.clear();
	vertices.reserve(num);
	for (int i = 0; i < num; ++i) 
	{
		int32_t x, y;
		unpack(x, ptr);
		unpack(y, ptr);

		sm::vec2 pos;
		pos.x = (float)x / scale;
		pos.y = (float)y / scale;

		if (reverse_y) {
			pos.y = -pos.y;
		}

		vertices.push_back(pos);
	}
}

}