#ifndef _LR_PREVIEW_PATH_GRID_H_
#define _LR_PREVIEW_PATH_GRID_H_

#include "IPathfinding.h"
#include "PathUtil.h"

#include <ee/Rect.h>

#include <vector>

namespace lr
{
namespace preview
{

class PathGrid : public IPathfinding
{
public:
	PathGrid(const ee::Rect& region, int row, int col);

	virtual void DisableRegion(const ee::Sprite* spr, bool disable);

	virtual void QueryRoute(const sm::vec2& start, const sm::vec2& end);

	virtual void DebugDraw() const;

private:
	struct Node
	{
		Node() : m_used(false) {}

		int id;

		bool m_used;

	}; // Node

	struct Connection
	{
		Connection(float len, Node* n) 
			: len(len), n(n) {}

		float len;
		Node* n;
	}; // Connection

	class Network : public INetwork
	{
	public:
		Network(const ee::Rect& region, int row, int col);
		~Network();

		virtual sm::vec2 TransIDToPos(int id) const;

		void SetStatus(const ee::Rect& region, bool used);

		VisitedNode* QueryRoute(const sm::vec2& start, const sm::vec2& end);

		void DebugDraw() const;

	private:
		Node* QueryNode(const sm::vec2& pos) const;

		void Expand(VisitedNode* node, const sm::vec2& end);

		void GetConnections(VisitedNode* node, std::vector<Connection>& connections) const;

	private:
		ee::Rect m_region;

		int m_row, m_col;
		float m_width, m_height;

		Node* m_nodes;

		VisitedList m_visited;
		CandidateList m_candidate;

		// todo
		friend class PathGrid;

	}; // Network

private:
	Network m_nw;

	std::vector<sm::vec2> m_routes;

}; // PathGrid

}
}

#endif // _LR_PREVIEW_PATH_GRID_H_