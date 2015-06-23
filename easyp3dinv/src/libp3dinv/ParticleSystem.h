#ifndef _EASYP3DINV_PARTICLE_SYSTEM_H_
#define _EASYP3DINV_PARTICLE_SYSTEM_H_

#include <drag2d.h>

#include "Particle.h"

namespace ep3dinv
{

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Update(float dt);

	void Draw(const d2d::Matrix& mt) const;

	void LoadFromFile(const std::string& filepath);

private:
	void Clear();

private:
	std::vector<Particle*> m_particles;

}; // ParticleSystem

}

#endif // _EASYP3DINV_PARTICLE_SYSTEM_H_