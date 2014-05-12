#ifndef _LIBSHAPE_EDIT_NODES_ATOMIC_OP_H_
#define _LIBSHAPE_EDIT_NODES_ATOMIC_OP_H_

#include <drag2d.h>

namespace libshape
{

class ChainShape;

namespace edit_nodes
{

class ModifyNodesAOP : public d2d::AbstractAtomicOP
{
public:
	ModifyNodesAOP(const std::vector<ChainShape*>& chains, 
		const std::vector<std::vector<d2d::Vector> >& dstChains);

	virtual void undo();
	virtual void redo();

	virtual Json::Value store(const std::vector<d2d::ISprite*>& sprites) { return NULL; }

private:
	std::vector<ChainShape*> m_chains;
	std::vector<std::vector<d2d::Vector> > m_src, m_dst;

}; // ModifyNodesAOP

}

}

#endif // _LIBSHAPE_EDIT_NODES_ATOMIC_OP_H_