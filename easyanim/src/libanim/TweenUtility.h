#ifndef _EASYANIM_TWEEN_UTILITY_H_
#define _EASYANIM_TWEEN_UTILITY_H_

#include <vector>

namespace ee { class Sprite; }
namespace s2 { class Sprite; }

namespace eanim
{

class TweenUtility
{
public:
	static void GetTweenSprites(const std::vector<s2::Sprite*>& start, 
		const std::vector<s2::Sprite*>& end, 
		std::vector<ee::Sprite*>& tween, float process);
	static void GetTweenSprites(const std::vector<ee::Sprite*>& start, 
		const std::vector<ee::Sprite*>& end, 
		std::vector<ee::Sprite*>& tween, float process);

	static bool IsTweenMatched(const ee::Sprite* s0, const ee::Sprite* s1);

	static void GetTweenSprite(ee::Sprite* start, ee::Sprite* end, 
		ee::Sprite* tween, float process);	

}; // TweenUtility

}

#endif // _EASYANIM_TWEEN_UTILITY_H_