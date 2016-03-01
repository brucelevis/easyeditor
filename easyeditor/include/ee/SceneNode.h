#ifndef _EASYEDITOR_SCENE_NODE_H_
#define _EASYEDITOR_SCENE_NODE_H_

#include <set>

namespace ee
{

class SceneNode
{
public:
	virtual ~SceneNode() {}
	virtual bool Update(float dt) = 0;
	virtual void Draw() const = 0;
}; // SceneNode

class SceneNodeMgr
{
public:
	void Add(SceneNode* node);
	void Remove(SceneNode* node);

	bool Update(float dt);
	void Draw() const;

	static SceneNodeMgr* Instance();

private:
	SceneNodeMgr() {}

private:
	std::set<SceneNode*> m_nodes;

private:
	static SceneNodeMgr* m_instance;

}; // SceneNodeMgr

}

#endif // _EASYEDITOR_SCENE_NODE_H_