#ifndef _EASYPARTICLE2D_LOAD_ADAPTER_H_
#define _EASYPARTICLE2D_LOAD_ADAPTER_H_

#include <drag2d.h>

namespace eparticle2d
{

class LoadAdapter
{
public:
	void Load(const std::string& filepath);

private:
	void LoadComponent(const std::string& dir, const Json::Value& comp_val);

public:
	struct Component
	{
		float angle_start, angle_end;

		float scale_start, scale_end;

		d2d::Colorf col_mul_start, col_mul_end;
		d2d::Colorf col_add_start, col_add_end;

		std::string filepath;
	}; // Component

public:
	std::string name;

	int mode_type;

	//////////////////////////////////////////////////////////////////////////
	// A: gravity + tangential + radial
	//////////////////////////////////////////////////////////////////////////
	struct A
	{
		d2d::Vector gravity;

		float speed, speed_var;

		float tangential_accel, tangential_accel_var;
		float radial_accel, radial_accel_var;

		bool rotation_is_dir;
	};

	//////////////////////////////////////////////////////////////////////////
	// B: radius + rotate
	//////////////////////////////////////////////////////////////////////////
	struct B
	{
		float start_radius, start_radius_var;
		float end_radius, end_radius_var;

		float direction_delta, direction_delta_var;
	};

	//////////////////////////////////////////////////////////////////////////
	// C: tangential spd cos
	//////////////////////////////////////////////////////////////////////////
	struct C
	{
		float speed, speed_var;

		float cos_amplitude, cos_amplitude_var;
		float cos_frequency, cos_frequency_var;
	};

	A A;
	B B;
	C C;

	float emission_time;
	int count;

	float life, life_var;

	d2d::Vector position, position_var;

	float direction, direction_var;

	std::vector<Component> components;

}; // LoadAdapter

}

#endif // _EASYPARTICLE2D_LOAD_ADAPTER_H_