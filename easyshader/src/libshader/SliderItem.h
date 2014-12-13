#ifndef _ESHADER_SLIDER_ITEM_H_
#define _ESHADER_SLIDER_ITEM_H_

namespace eshader 
{

template<typename T>
struct SliderItem
{		
	std::string title;

	T default;
	T min, max;

	SliderItem(const char* title, T default, T min, T max)
		: title(title), default(default), min(min), max(max) {}

}; // SliderItem

typedef SliderItem<float> SliderItemFloat;

}

#endif // _ESHADER_SLIDER_ITEM_H_