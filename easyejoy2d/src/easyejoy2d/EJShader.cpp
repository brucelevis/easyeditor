#include "EJShader.h"

#include <dtex.h>

namespace eejoy2d
{

void EJShader::Reset()
{
	dtex_shader_set_texture(0);
	dtex_shader_program(DTEX_PROGRAM_NULL);
}

}