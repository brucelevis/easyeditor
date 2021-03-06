#ifndef _EASYANIM_KEY_FRAME_H_
#define _EASYANIM_KEY_FRAME_H_

#include "SkeletonData.h"

#include <ee/Object.h>

namespace eanim
{

class Layer;

class KeyFrame : public ee::Object
{
public:
	KeyFrame(int time);
	~KeyFrame();

	void SetLayer(Layer* layer);

	void CopyFromOther(const KeyFrame* src);

	bool IsEmpty() const {
		return m_sprites.empty();
	}

	void Insert(ee::Sprite* sprite, int idx);
	bool Remove(ee::Sprite* sprite);
	bool Reorder(const ee::Sprite* sprite, bool up);
	bool ReorderMost(const ee::Sprite* sprite, bool up);
	bool Sort(std::vector<ee::Sprite*>& sprites);

	int Size() const { return m_sprites.size(); }
	const ee::Sprite* GetSprite(int index) {
		return index >= 0 && index < m_sprites.size() ? m_sprites[index] : NULL;
	}

	const std::vector<ee::Sprite*>& GetAllSprites() const { return m_sprites; }

	bool HasClassicTween() const { return m_classic_tween; }
	void SetClassicTween(bool tween = true) { m_classic_tween = tween; }

	int GetTime() const { return m_time; }
	void SetTime(int time) { m_time = time; }

	void Clear();

	SkeletonData& GetSkeletonData() { return m_skeleton; }

	int GetID() const { return m_id; }
	void SetID(int id) { m_id = id; }

	void OnActive();

	static void GetTweenSprite(const KeyFrame* start, const KeyFrame* end, std::vector<ee::Sprite*>& tween, float process);

// private:
// 	void GetTweenSprite(ee::Sprite* start, ee::Sprite* end, ee::Sprite* tween, float process) const;
// 	bool IsTweenMatched(const ee::Sprite* s0, const ee::Sprite* s1) const;

private:
	int m_time;

	std::vector<ee::Sprite*> m_sprites;

	bool m_classic_tween;

	Layer* m_layer;

	SkeletonData m_skeleton;

	int m_id;

}; // KeyFrame

}

#endif // _EASYANIM_KEY_FRAME_H_
