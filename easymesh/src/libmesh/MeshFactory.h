#ifndef _EASYMESH_MESH_FACTORY_H_
#define _EASYMESH_MESH_FACTORY_H_

#include "config.h"

namespace ee { class Image; }

namespace emesh
{

class Mesh;

class MeshFactory
{
public:
	Mesh* CreateMesh(const ee::Image& image) const;

	void SetShapeType(MeshType type);

public:
	static MeshFactory* Instance();

private:
	MeshFactory() {}

private:
	MeshType m_type;

private:
	static MeshFactory* m_instance;

}; // MeshFactory

}

#endif // _EASYMESH_MESH_FACTORY_H_