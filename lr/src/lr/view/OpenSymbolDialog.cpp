#include "OpenSymbolDialog.h"
#include "StagePanel.h"
#include "LibraryPanel.h"
#include "UnitEditDlg.h"
#include "LevelEditDlg.h"

#include "dataset/ShapesUD.h"
#include "dataset/Layer.h"
#include "dataset/data_utility.h"

#include <easyscale9.h>
#include <easymesh.h>
#include <easyanim.h>
#include <easycomplex.h>
#include <easytexture.h>
#include <easyshape.h>
#include <easyterrain2d.h>
#include <easyshadow.h>

namespace lr
{

OpenSymbolDialog::OpenSymbolDialog(wxWindow* wnd, d2d::EditPanelImpl* stage, 
								   d2d::MultiSpritesImpl* sprites_impl)
	: m_wnd(wnd)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
{
}

void OpenSymbolDialog::Open(d2d::ISprite* spr)
{
	if (spr->GetSymbol().GetFilepath().find("[gen].json") != std::string::npos) {
		wxMessageBox("禁止编辑自动生成的文件", "warning", wxOK | wxICON_INFORMATION, m_wnd);
		return;
	}

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->EnableObserve(false);
	stage->GetCanvas()->EnableObserve(false);
	stage->SetUpdateState(false);
	if (static_cast<LibraryPanel*>(stage->GetLibrary())->IsCurrUnitLayer()) 
	{
		std::vector<std::string> path_names;
		static_cast<LibraryPanel*>(stage->GetLibrary())->GetAllPathName(path_names);
		UnitEditDlg dlg(m_wnd, spr, path_names);
		if (dlg.ShowModal() == wxID_OK) {
			spr->tag = dlg.ToString();
		}
	} 
	else if (static_cast<LibraryPanel*>(stage->GetLibrary())->IsCurrLevelLayer())  {
		LevelEditDlg dlg(m_wnd, spr);
		if (dlg.ShowModal() == wxID_OK) {
			bool use_symbol = spr->tag.find("[symbol]") != std::string::npos;
			if (use_symbol) {
				spr->tag = "[symbol];";
			}
			spr->tag += dlg.ToString();
		}
	}
	else if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(spr))
	{
		ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->GetSymbol());
		ecomplex::EditDialog dlg(m_wnd, &symbol, m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();

		//////////////////////////////////////////////////////////////////////////

		// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
		// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(spr))
	{
		libanim::PreviewDialog dlg(m_wnd, &anim->GetSymbol(), m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(spr))
	{
		escale9::Symbol& symbol = const_cast<escale9::Symbol&>(patch9->GetSymbol());
		escale9::EditDialog dlg(m_wnd, &symbol, m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}
	else if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(spr))
	{
		emesh::EditDialog dlg(m_wnd, sprite, m_stage->GetCanvas()->GetGLContext());
		dlg.ShowModal();
	}
	else if (d2d::FontBlankSprite* font = dynamic_cast<d2d::FontBlankSprite*>(spr))
	{
		d2d::TextDialog dlg(m_wnd, font);
		dlg.ShowModal();
	}
	else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(spr))
	{
		etexture::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), tex, m_sprites_impl);
		dlg.ShowModal();
		UpdateShapeFromETexture(tex);
	}
	else if (libshape::Sprite* shape = dynamic_cast<libshape::Sprite*>(spr))
	{
		libshape::EditDialogSimple dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), shape, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (eterrain2d::Sprite* terr = dynamic_cast<eterrain2d::Sprite*>(spr))
	{
		eterrain2d::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), terr, m_sprites_impl);
		dlg.ShowModal();
	} 
	else if (eshadow::Sprite* shadow = dynamic_cast<eshadow::Sprite*>(spr))
	{
		eshadow::EditDialog dlg(m_wnd, m_stage->GetCanvas()->GetGLContext(), shadow, m_sprites_impl);
		dlg.ShowModal();
	}
	else if (spr)
	{
		d2d::SpriteDialog dlg(m_wnd, spr);
		if (dlg.ShowModal() == wxID_OK) {
			spr->name = dlg.GetNameStr();
			spr->tag = dlg.GetTagStr();
		}
	}

	stage->SetUpdateState(true);
	stage->EnableObserve(true);
}

void OpenSymbolDialog::UpdateShapeFromETexture(etexture::Sprite* spr)
{
	if (!spr->GetUserData()) {
		return;
	}

	UserData* ud = static_cast<UserData*>(spr->GetUserData());
	if (ud->type == UT_BASE_FILE) {
		return;
	}

	ShapesUD* sud = static_cast<ShapesUD*>(ud);
	LibraryPanel* library = static_cast<LibraryPanel*>(static_cast<StagePanel*>(m_wnd)->GetLibrary());
	Layer* layer = library->GetLayer(sud->layer_id);
	for (int i = 0, n = sud->shape_names.size(); i < n; ++i) {
		d2d::IShape* shape = layer->QueryShape(sud->shape_names[i]);
		layer->RemoveShape(shape);
	}

	sud->shape_names.clear();
	std::vector<d2d::IShape*> shapes;
	create_shapes_from_etxture(spr, shapes);
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		layer->InsertShape(shapes[i]);
		sud->shape_names.push_back(shapes[i]->name);
	}
}

}