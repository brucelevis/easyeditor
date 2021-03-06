#ifndef _EASYSCALE9_RESIZE_ATOMIC_OP_H_
#define _EASYSCALE9_RESIZE_ATOMIC_OP_H_

#include <ee/AtomicOP.h>

#include <SM_Vector.h>

namespace escale9
{

class Symbol;

class ResizeAtomicOP : public ee::AtomicOP
{
public:
	ResizeAtomicOP(Symbol* symbol, const sm::vec2& src, 
		const sm::vec2& dst);

	virtual void Undo();
	virtual void Redo();

	virtual Json::Value Store(const std::vector<ee::Sprite*>& sprites) const {
		Json::Value ret;
		return ret;
	}

private:
	Symbol* m_symbol;

	sm::vec2 m_src, m_dst;

}; // ResizeAtomicOP

}

#endif // _EASYSCALE9_RESIZE_ATOMIC_OP_H_