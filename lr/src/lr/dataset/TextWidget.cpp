#include "TextWidget.h"
#include "DynamicInfo.h"

namespace lr
{

std::string TextWidget::GetValue() const
{
	std::string str = m_ctrl->GetValue().ToStdString();
	if (str == m_default) {
		return DEFAULT_VAL;
	} else {
		return str;
	}
}

void TextWidget::InitLayout(wxWindow* parent, wxSizer* top_sizer,
								const DynamicInfo& info)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new wxStaticText(parent, wxID_ANY, GetTitle()), 0, wxLEFT | wxRIGHT, 5);

	std::string default_str = m_default;
	std::string value = info.QueryValue(GetKey());
	if (!value.empty()) {
		default_str = value;
	}
	m_ctrl = new wxTextCtrl(parent, wxID_ANY, default_str, wxDefaultPosition, wxSize(200, -1));
	sizer->Add(m_ctrl, 0, wxLEFT | wxRIGHT, 5);

	top_sizer->Add(sizer);
	top_sizer->AddSpacer(10);
}

void TextWidget::Load(const Json::Value& value)
{
	if (!value["default"].isNull()) {
		m_default = value["default"].asString();
	}
}

}