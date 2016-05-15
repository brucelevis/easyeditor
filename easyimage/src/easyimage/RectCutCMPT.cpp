#include "RectCutCMPT.h"
#include "RectCutOP.h"
#include "StagePanel.h"

#include <ee/FileHelper.h>
#include <ee/ImageSprite.h>
#include <ee/Image.h>
#include <ee/Rect.h>
#include <ee/StringHelper.h>
#include <ee/panel_msg.h>
#include <ee/ImageSaver.h>
#include <ee/DummySprite.h>
#include <ee/DummySymbol.h>
#include <ee/FinishDialog.h>
#include <ee/ImageClip.h>

#include <easycomplex.h>
#include <easyimage.h>

#include <fstream>

namespace eimage
{

static const std::string FILTER = "rectcut";

RectCutCMPT::RectCutCMPT(wxWindow* parent, const std::string& name,
						 StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_editop = new RectCutOP(this, stage);
}

void RectCutCMPT::OnSaveEditOP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Save"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILTER + wxT(".json"), wxFD_SAVE);
	if (dlg.ShowModal() == wxID_OK)
	{
		RectCutOP* op = static_cast<RectCutOP*>(m_editop);

		Json::Value value;

		std::string filepath = op->GetImageFilepath();
		std::string dir = ee::FileHelper::GetFileDir(dlg.GetPath().ToStdString());
		value["image filepath"] = ee::FileHelper::GetRelativePath(dir, filepath);
		op->GetRectMgr().Store(value);

		std::string filename = ee::FileHelper::GetFilenameAddTag(dlg.GetPath().ToStdString(), FILTER, "json");
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filename.c_str());
		std::locale::global(std::locale("C"));
		writer.write(fout, value);
		fout.close();
	}
}

void RectCutCMPT::OnLoadEditOP(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open"), wxEmptyString, wxEmptyString, 
		wxT("*_") + FILTER + wxT(".json"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		std::string filename = ee::FileHelper::GetFilenameAddTag(dlg.GetPath().ToStdString(), FILTER, "json");
		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(filename.c_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		RectCutOP* op = static_cast<RectCutOP*>(m_editop);
		std::string dlgpath = ee::FileHelper::GetFileDir(filename);
		std::string path = value["image filepath"].asString();
		std::string absolutePath = ee::FileHelper::GetAbsolutePath(dlgpath, path);
		op->LoadImageFromFile(absolutePath);
		op->GetRectMgr().Load(value);
	}
}

wxSizer* RectCutCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
// 	sizer->Add(initEditIOLayout());
// 	sizer->AddSpacer(10);
	sizer->Add(InitDataOutputLayout());
	sizer->AddSpacer(10);
	sizer->Add(InitAddRectLayout());
	return sizer;
}

wxSizer* RectCutCMPT::InitEditIOLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Edit I/O"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
	// save
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Save"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnSaveEditOP));
		sizer->Add(btn);
	}
	sizer->AddSpacer(5);
	// load
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Load"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnLoadEditOP));
		sizer->Add(btn);
	}
	return sizer;
}

wxSizer* RectCutCMPT::InitDataOutputLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Data Output"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	// images path
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("images path"));
		wxSizer* sz = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		m_imagePath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY);
		sz->Add(m_imagePath);

		sz->AddSpacer(5);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnSetImagesPath));
		sz->Add(btn);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	// json path
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("json path"));
		wxSizer* sz = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		m_jsonPath = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY);
		sz->Add(m_jsonPath);

		sz->AddSpacer(5);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnSetJsonPath));
		sz->Add(btn);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	// output
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Output"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnOutputData));
		sizer->Add(btn);
	}
	return sizer;
}

wxSizer* RectCutCMPT::InitAddRectLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Add Rect"));
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);

		m_widthCtrl = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxSize(60, -1));
		inputSizer->Add(m_widthCtrl);

		inputSizer->AddSpacer(5);

		m_heightCtrl = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxSize(60, -1));
		inputSizer->Add(m_heightCtrl);

		sizer->Add(inputSizer);
	}
	sizer->AddSpacer(5);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Add"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(RectCutCMPT::OnAddRect));
		sizer->Add(btn);
	}
	return sizer;
}

void RectCutCMPT::OnSetImagesPath(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editop);
	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Images Path", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_imagePath->SetValue(dlg.GetPath());
	}

	op->SetMouseMoveFocus(true);
}

void RectCutCMPT::OnSetJsonPath(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editop);
	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Json Path", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_jsonPath->SetValue(dlg.GetPath());
	}

	op->SetMouseMoveFocus(true);
}

void RectCutCMPT::OnOutputData(wxCommandEvent& event)
{
	RectCutOP* op = static_cast<RectCutOP*>(m_editop);
	const std::vector<ee::Rect*>& rects = op->GetRectMgr().GetAllRect();
	if (rects.empty()) {
		return;
	}

	const ee::Sprite* sprite = m_stage->GetImage();
	if (!sprite) {
		return;
	}

	const ee::ImageSprite* imgSprite = dynamic_cast<const ee::ImageSprite*>(sprite);
	if (!imgSprite) {
		return;
	}

	ee::Image* image = imgSprite->GetSymbol().GetImage();

	std::string imageDir = m_imagePath->GetValue();
	std::string jsonDir = m_jsonPath->GetValue();

	std::string imageName = ee::FileHelper::GetFilename(image->GetFilepath());
	ecomplex::Symbol* complex = new ecomplex::Symbol;
	for (int i = 0, n = rects.size(); i < n; ++i)
	{
		const ee::Rect& r = *rects[i];

		ee::ImageClip clip(*image->GetImageData());
		const uint8_t* pixels = clip.Clip(r.xmin, r.xmax, r.ymin, r.ymax);
		float width = r.Width();
		float height = r.Height();

		std::string img_filename = imageDir + "\\" + imageName + "_" + ee::StringHelper::ToString(i);
		ee::ImageSaver::StoreToFile(pixels, width, height, 4, img_filename, ee::ImageSaver::e_png);

		std::string img_fullname = img_filename + ".png";
		ee::Sprite* sprite = new ee::DummySprite(new ee::DummySymbol(img_fullname, width, height));
		sm::vec2 off;
		off.x = r.CenterX() - image->GetClippedWidth() * 0.5f;
		off.y = r.CenterY() - image->GetClippedHeight() * 0.5f;
		sprite->Translate(off);
		complex->Add(sprite);
	}

	std::string tag = ee::FileType::GetTag(ee::FileType::e_complex);
	std::string json_filename = jsonDir + "\\" + imageName + "_" + tag + ".json";
	ecomplex::FileStorer::Store(json_filename.c_str(), complex);
	delete complex;

	ee::FinishDialog dlg(this);
	dlg.ShowModal();
}

void RectCutCMPT::OnAddRect(wxCommandEvent& event)
{
	double width, height;
	m_widthCtrl->GetValue().ToDouble(&width);
	m_heightCtrl->GetValue().ToDouble(&height);
	
	RectCutOP* op = static_cast<RectCutOP*>(m_editop);
	op->GetRectMgr().Insert(ee::Rect(sm::vec2(0, 0), sm::vec2((float)width, (float)height)));

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}