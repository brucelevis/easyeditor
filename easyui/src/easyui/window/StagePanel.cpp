#include "StagePanel.h"
#include "StageCanvas.h"
#include "ArrangeSpriteImpl.h"
#include "TopPannels.h"
#include "TopLibraryPanel.h"
#include "ToolbarPanel.h"
#include "TopToolbarPanel.h"
#include "message_id.h"
#include "QueryWindowViewSizeSJ.h"
#include "ChangeWindowViewSizeSJ.h"
#include "PreviewDialog.h"
#include "Code.h"
#include "widget_id.h"
#include "SymbolCfg.h"

#include <easycomplex.h>
#include <easybuilder.h>

namespace eui
{
namespace window
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, TopPannels* top_pannels)
	: UIStagePage(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), top_pannels->library->GetUILibrary())
	, m_top_pannels(top_pannels)
	, m_symbols_cfg(this, top_pannels->library->GetUILibrary())
{
	SetEditOP(new d2d::ArrangeSpriteOP<ecomplex::SelectSpritesOP>(this, GetStageImpl(), this, top_pannels->property, 
		NULL, d2d::ArrangeSpriteConfig(), new ArrangeSpriteImpl(this, top_pannels->property)));
	SetCanvas(new StageCanvas(this));

	int width, height;
	window::QueryWindowViewSizeSJ::Instance()->Query(width, height);
	m_view_width = width;
	m_view_height = height;

	m_toolbar = new ToolbarPanel(top_pannels->toolbar, this);
	m_toolbar_idx = top_pannels->toolbar->AddToolbar(m_toolbar);

	RegistSubject(QueryWindowViewSizeSJ::Instance());
}

void StagePanel::LoadFromFile(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filename);
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	std::string type = value["type"].asString();
	if (!type.empty() && !is_widget(type, ID_WINDOW)) {
		return;
	}

	std::string name = value["name"].asString();
	m_toolbar->SetWindowName(name);

	if (!value["view"].isNull()) {
		m_view_width = value["view"]["width"].asInt();
		m_view_height = value["view"]["height"].asInt();
		m_anchor_mgr.OnViewChanged(m_view_width, m_view_height);
		ChangeWindowViewSizeSJ::Instance()->Change(m_view_width, m_view_height);
	}

	std::string dir = d2d::FileHelper::GetFileDir(filename) + "\\";
//	std::vector<d2d::Sprite*> sprites;
	int idx = 0;
	Json::Value spr_val = value["sprite"][idx++];
	while (!spr_val.isNull()) {
		std::string filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
		d2d::Symbol* symbol = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);

		d2d::Sprite* sprite = d2d::SpriteFactory::Instance()->Create(symbol);
		sprite->Load(spr_val);

		d2d::InsertSpriteSJ::Instance()->Insert(sprite);
//		sprites.push_back(sprite);

		sprite->Release();
		symbol->Release();

		spr_val = value["sprite"][idx++];
	}
//	GetAnchorMgr()->LoadFromFile(value, sprites);
}

void StagePanel::StoreToFile(const char* filename) const
{
	std::string dir = d2d::FileHelper::GetFileDir(filename) + "\\";

	Json::Value value;

	value["type"] = get_widget_desc(ID_WINDOW);

	value["name"] = m_toolbar->GetWindowName();

	std::vector<d2d::Sprite*> sprites;
	TraverseSprites(d2d::FetchAllVisitor<d2d::Sprite>(sprites), d2d::DT_ALL);
	for (int i = 0, n = sprites.size(); i < n; ++i) 
	{
		d2d::Sprite* spr = sprites[i];

		Json::Value spr_val;
		spr_val["filepath"] = d2d::FileHelper::GetRelativePath(dir,
			spr->GetSymbol().GetFilepath()).ToStdString();
		spr->Store(spr_val);

		value["sprite"][i] = spr_val;
	}

	m_anchor_mgr.StoreToFile(value);

	// wrapper complex
	ecomplex::Symbol wrapper_complex;
	for_each(sprites.begin(), sprites.end(), d2d::RetainObjectFunctor<d2d::Sprite>());
	wrapper_complex.m_sprites = sprites;
	std::string name = filename;
	name = name.substr(0, name.find_last_of('_'));
	std::string wrapper_path = name + "_wrapper_complex[gen].json";
	wrapper_complex.SetFilepath(wrapper_path);
	wrapper_complex.name = m_toolbar->GetWindowName();
	ecomplex::FileStorer::Store(wrapper_path.c_str(), &wrapper_complex);

	value["view"]["width"] = m_view_width;
	value["view"]["height"] = m_view_height;

	value["wrapper filepath"] = d2d::FileHelper::GetRelativePath(dir, wrapper_path).ToStdString();;

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filename);
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void StagePanel::EnablePage(bool enable)
{
	if (enable) {
		m_top_pannels->toolbar->EnableToolbar(m_toolbar_idx);
		d2d::SetCanvasDirtySJ::Instance()->SetDirty();
		m_top_pannels->library->EnableUILibrary(true);
	} else {
		GetSpriteSelection()->Clear();
	}

	m_toolbar->EnableObserve(enable);
	m_anchor_mgr.EnableObserve(enable);
	EnableObserve(enable);
	GetStageImpl()->EnableObserve(enable);
}

void StagePanel::OnPreview()
{
	int width, height;
	QueryWindowViewSizeSJ::Instance()->Query(width, height);

	std::vector<const d2d::Sprite*> sprites;
	TraverseSprites(d2d::FetchAllVisitor<const d2d::Sprite>(sprites));

	EnableObserve(false);
	GetCanvas()->EnableObserve(false);
	GetStageImpl()->EnableObserve(false);

	PreviewDialog dlg(const_cast<StagePanel*>(this), GetCanvas()->GetGLContext(), width, height, sprites);
	dlg.ShowModal();

	EnableObserve(true);
	GetCanvas()->EnableObserve(true);
	GetStageImpl()->EnableObserve(true);
}

void StagePanel::OnCode() const
{
	ebuilder::CodeDialog dlg(const_cast<StagePanel*>(this));
	std::vector<d2d::Sprite*> sprites;
	TraverseSprites(d2d::FetchAllVisitor<d2d::Sprite>(sprites));

	// ui
	{
		ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, wxT("ui.lua"));

		ebuilder::CodeGenerator gen;
		Code code(gen);
		code.ResolveUI(sprites);
		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		dlg.notebook->AddPage(page, page->getName());
	}
	// tid
	{
		ebuilder::love2d::Page* page = new ebuilder::love2d::Page(dlg.notebook, wxT("texts.lua"));

		ebuilder::CodeGenerator gen;
		Code code(gen);
		code.ResolveText(sprites);
		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		dlg.notebook->AddPage(page, page->getName());
	}
	dlg.ShowModal();
}

void StagePanel::InitConfig()
{
	m_symbols_cfg.LoadConfig();
}

void StagePanel::SetViewSize(int width, int height)
{
	if (m_view_width == width && 
		m_view_height == height) {
		return;
	}

	m_view_width = width;
	m_view_height = height;
	m_anchor_mgr.OnViewChanged(m_view_width, m_view_height);

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	d2d::SpritesPanelImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case MSG_QUERY_WINDOW_VIEW_SIZE:
		{
			QueryWindowViewSizeSJ::Params* p = (QueryWindowViewSizeSJ::Params*)ud;
			p->width = m_view_width;
			p->height = m_view_height;
		}
		break;
	case d2d::MSG_INSERT_SPRITE:
		{
			d2d::InsertSpriteSJ::Params* p = (d2d::InsertSpriteSJ::Params*)ud;
			d2d::Sprite* spr = p->spr;
			std::string type = "";
			SymbolCfg::Instance()->QueryType(&spr->GetSymbol(), type);
			if (type.empty() && spr->tag.find("type=unknown") == std::string::npos) {
				spr->tag = "type=unknown;" + spr->tag;
			}
		}
		break;
	}
}

}
}