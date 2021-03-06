#include "RenderShader.h"
#include "RenderFilter.h"

namespace s2
{

RenderShader::RenderShader()
	: filter(NULL)
	, blend(BM_NULL)
	, fast_blend(FBM_NULL)
{}

RenderShader::~RenderShader()
{
	// todo 
//	delete filter;
}

RenderShader RenderShader::operator * (const RenderShader& rs) const
{
	RenderShader ret;

	if (rs.blend != s2::BM_NULL) {
		ret.blend = rs.blend;
	} else {
		ret.blend = blend;
	}

	if (rs.fast_blend != s2::FBM_NULL) {
		ret.fast_blend = rs.fast_blend;
	} else {
		ret.fast_blend = fast_blend;
	}

	if (rs.filter && rs.filter->GetMode()!= s2::FM_NULL) {
		ret.filter = rs.filter;
	} else {
		ret.filter = filter;
	}

	return ret;
}

}