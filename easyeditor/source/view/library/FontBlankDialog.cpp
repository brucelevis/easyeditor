#include "FontBlankDialog.h"
#include "FontBlankSymbol.h"
#include "FileType.h"
#include "FileHelper.h"
#include "StringHelper.h"

#include <json/json.h>

#include <fstream>

namespace ee
{

FontBlankDialog::FontBlankDialog(wxWindow* parent, FontBlankSymbol* symbol)
 		: wxDialog(parent, wxID_ANY, "Edit Font", wxDefaultPosition, 
		wxSize(400, 300), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
		, m_symbol(symbol)
{
	InitLayout();
}

FontBlankDialog::~FontBlankDialog()
{
	m_symbol->name = m_name->GetValue();
	m_symbol->font = m_font->GetValue();
	m_symbol->color = m_color->GetValue();

	double tmp;
	m_align->GetValue().ToDouble(&tmp);
	m_symbol->align_hori = tmp;

	m_size->GetValue().ToDouble(&tmp);
	m_symbol->size = tmp;

	m_width->GetValue().ToDouble(&tmp);
	m_symbol->width = tmp;

	m_height->GetValue().ToDouble(&tmp);
	m_symbol->height = tmp;

	StoreSymbol();
}

void FontBlankDialog::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "name"), 0, wxLEFT | wxRIGHT, 5);

		m_name = new wxTextCtrl(this, wxID_ANY);
		m_name->SetValue(m_symbol->name);
		csizer->Add(m_name, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "font"), 0, wxLEFT | wxRIGHT, 5);

		m_font = new wxTextCtrl(this, wxID_ANY);
		m_font->SetValue(m_symbol->font);
		csizer->Add(m_font, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "color"), 0, wxLEFT | wxRIGHT, 5);

		m_color = new wxTextCtrl(this, wxID_ANY);
		m_color->SetValue(m_symbol->color);
		csizer->Add(m_color, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "align"), 0, wxLEFT | wxRIGHT, 5);

		m_align = new wxTextCtrl(this, wxID_ANY);
		m_align->SetValue(StringHelper::ToString(m_symbol->align_hori));
		csizer->Add(m_align, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "size"), 0, wxLEFT | wxRIGHT, 5);

		m_size = new wxTextCtrl(this, wxID_ANY);
		m_size->SetValue(StringHelper::ToString(m_symbol->size));
		csizer->Add(m_size, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "width"), 0, wxLEFT | wxRIGHT, 5);

		m_width = new wxTextCtrl(this, wxID_ANY);
		m_width->SetValue(StringHelper::ToString(m_symbol->width));
		csizer->Add(m_width, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	sizer->AddSpacer(5);
	{
		wxSizer* csizer = new wxBoxSizer(wxHORIZONTAL);
		csizer->Add(new wxStaticText(this, wxID_ANY, "height"), 0, wxLEFT | wxRIGHT, 5);

		m_height = new wxTextCtrl(this, wxID_ANY);
		m_height->SetValue(StringHelper::ToString(m_symbol->height));
		csizer->Add(m_height, 0, wxLEFT | wxRIGHT, 5);

		sizer->Add(csizer);
	}
	SetSizer(sizer);

	sizer->Layout();
	Refresh(true);
}

void FontBlankDialog::StoreSymbol()
{
	if (!m_symbol) return;

	std::string filepath = m_symbol->GetFilepath();
	if (filepath.empty())
	{
		std::string ext = FileType::GetTag(FileType::e_fontblank);
		ext = "_" + ext + ".json";

		std::string name = m_symbol->name;
		if (name.empty()) 
		{
			name = m_symbol->GetName();
		}

		std::string dir = FileHelper::GetFileDir(m_symbol->GetFilepath());
		filepath = dir + "\\" + m_symbol->name + ext;
	}

	Json::Value value;

	value["name"] = m_symbol->name;
	value["font"] = m_symbol->font;
	value["color"] = m_symbol->color;
	value["align hori"] = m_symbol->align_hori;
	value["align vert"] = m_symbol->align_vert;
	value["size"] = m_symbol->size;
	value["width"] = m_symbol->width;
	value["height"] = m_symbol->height;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();

	m_symbol->LoadFromFile(filepath);
}
}