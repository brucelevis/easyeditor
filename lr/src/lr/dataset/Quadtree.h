#ifndef _LR_QUADTREE_H_
#define _LR_QUADTREE_H_

#include <drag2d.h>

namespace lr
{

class Quadtree
{
public:
	Quadtree(const d2d::Rect& rect);
	~Quadtree();

	void Insert(const d2d::ISprite* spr);
	
	std::vector<const d2d::ISprite*> Query(const d2d::Rect& rect) const;

private:
	class Node
	{
	public:
		Node(const d2d::Rect& rect);
		~Node();

		void Insert(const d2d::ISprite* spr);

		bool IsLeaf() const;

	private:
		bool IsIntersect(const d2d::Rect& rect) const;

		void Split();

		static d2d::Rect GetSprRect(const d2d::ISprite* spr);

	private:
		static const int MAX_COUNT;

	private:
		d2d::Rect m_rect;

		// 0 1
		// 2 3
		Node* m_children[4];

		std::vector<const d2d::ISprite*> m_sprites;

		friend class Quadtree;

	}; // Node

private:
	Node* m_root;

}; // Quadtree

}

#endif // _LR_QUADTREE_H_