#ifndef _SPRITE2_RF_OUTER_GLOW_H_
#define _SPRITE2_RF_OUTER_GLOW_H_

#include "RenderFilter.h"

namespace s2
{

class RFOuterGlow : public RenderFilter
{
public:
	RFOuterGlow() 
		: RenderFilter(FM_OUTER_GLOW) 
		, m_iterations(9)
	{}

	int GetIterations() const { return m_iterations; }
	void SetIterations(int iterations) { m_iterations = iterations; }

private:
	int m_iterations;

}; // RFOuterGlow

}

#endif // _SPRITE2_RF_OUTER_GLOW_H_