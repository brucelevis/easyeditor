#include "BlendModes.h"

#include <string>

namespace ee
{

BlendModes* BlendModes::m_instance = NULL;

BlendModes::BlendModes()
{
	// normal
	m_modes.push_back(Item(s2::BM_NULL, "null", "无"));
//	m_modes.push_back(Item(s2::BM_DISSOLVE, "dissolve", "溶解"));

	// darken modes
	m_modes.push_back(Item(s2::BM_DARKEN, "darken", "变暗"));
	m_modes.push_back(Item(s2::BM_MULTIPLY, "multiply", "正片叠底"));
	m_modes.push_back(Item(s2::BM_COLOR_BURN, "color burn", "颜色加深"));
	m_modes.push_back(Item(s2::BM_LINEAR_BURN, "linear burn", "线性颜色加深"));
//	m_modes.push_back(Item(s2::BM_DARKER_COLOR, "darker color", "?"));

	// lighten modes
	m_modes.push_back(Item(s2::BM_LIGHTEN, "lighten", "变亮"));
	m_modes.push_back(Item(s2::BM_SCREEN, "screen", "屏幕"));
	m_modes.push_back(Item(s2::BM_COLOR_DODGE, "color dodge", "颜色减淡"));
	m_modes.push_back(Item(s2::BM_LINEAR_DODGE, "linear dodge", "线性颜色减淡"));
//	m_modes.push_back(Item(s2::BM_LIGHTER_COLOR, "lighter color", "?"));

	// saturation modes
	m_modes.push_back(Item(s2::BM_OVERLAY, "overlay", "叠加"));
	m_modes.push_back(Item(s2::BM_SOFT_LIGHT, "soft light", "柔光"));
	m_modes.push_back(Item(s2::BM_HARD_LIGHT, "hard light", "强光"));
	m_modes.push_back(Item(s2::BM_VIVID_LIGHT, "vivid light", "亮光"));
	m_modes.push_back(Item(s2::BM_LINEAR_LIGHT, "linear light", "线性光"));
	m_modes.push_back(Item(s2::BM_PIN_LIGHT, "pin light", "固定光"));
	m_modes.push_back(Item(s2::BM_HARD_MIX, "hard mix", "实色混合"));

	// substraction modes
	m_modes.push_back(Item(s2::BM_DIFFERENCE, "difference", "差异"));
	m_modes.push_back(Item(s2::BM_EXCLUSION, "exclusion", "排除"));

	// color modes 
// 	m_modes.push_back(Item(s2::BM_HUE, "hue", "色调"));
// 	m_modes.push_back(Item(s2::BM_SATURATION, "saturation", "饱和度"));
// 	m_modes.push_back(Item(s2::BM_COLOR, "color", "着色"));
// 	m_modes.push_back(Item(s2::BM_LUMINOSITY, "luminosity", "明度"));
}

s2::BlendMode BlendModes::GetModeFromNameEN(const std::string& name) const
{
	if (name.empty()) {
		return s2::BM_NULL;
	}

	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.name_en == name) {
			return item.mode;
		}
	}
	return s2::BM_NULL;
}

std::string BlendModes::GetNameENFromMode(s2::BlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		const Item& item = m_modes[i];
		if (item.mode == mode) {
			return item.name_en;
		}
	}
	return "";
}

s2::BlendMode BlendModes::GetIDFromIdx(int idx) const
{
	if (idx < 0 || idx >= static_cast<int>(m_modes.size())) {
		return s2::BM_NULL;
	} else {
		return m_modes[idx].mode;
	}
}

int BlendModes::GetIdxFromID(s2::BlendMode mode) const
{
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		if (m_modes[i].mode == mode) {
			return i;
		}
	}
	return 0;
}

void BlendModes::GetAllNameCN(std::vector<std::string>& names) const
{
	names.clear();
	for (int i = 0, n = m_modes.size(); i < n; ++i) {
		names.push_back(m_modes[i].name_cn);
	}
}

BlendModes* BlendModes::Instance()
{
	if (!m_instance) {
		m_instance = new BlendModes();
	}
	return m_instance;
}

}