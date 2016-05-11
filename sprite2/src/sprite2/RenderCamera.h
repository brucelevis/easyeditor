#ifndef _SPRITE2_RENDER_CAMERA_H_
#define _SPRITE2_RENDER_CAMERA_H_

#include "CameraMode.h"

#include <SM_Vector.h>

namespace s2
{

class RenderCamera
{
public:
	CameraMode	mode;
	float		base_y;	

public:
	RenderCamera();

	void CalculateZ(float cam_angle, sm::vec2 vertices[4], float z[4]) const;

}; // RenderCamera

}

#include "RenderCamera.inl"

#endif // _SPRITE2_RENDER_CAMERA_H_