#ifndef _LIBRESPACKER_LABEL_BUILDER_H_
#define _LIBRESPACKER_LABEL_BUILDER_H_

#include "INodeBuilder.h"

#include <drag2d.h>

namespace librespacker
{

class IPackNode;
class PackLabel;
class PackAnchor;

class LabelBuilder : public INodeBuilder
{
public:
	LabelBuilder();
	virtual ~LabelBuilder();

	virtual void Traverse(d2d::IVisitor& visitor) const;

	const IPackNode* Create(const d2d::FontSprite* spr);

private:
	std::vector<const PackLabel*> m_labels;
	std::vector<const PackAnchor*> m_anchors;

}; // LabelBuilder

}

#endif // _LIBRESPACKER_LABEL_BUILDER_H_