#include "SelectSpritesOP.h"

#include <easyscale9.h>
#include <easymesh.h>
#include <easyanim.h>
#include <easycomplex.h>
#include <easytexture.h>
#include <easyshape.h>

namespace emap
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::ViewPanelMgr* view_panel_mgr, d2d::AbstractEditCMPT* callback)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, view_panel_mgr, callback)
{
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::ISprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(selected))
	{
 		ecomplex::Symbol& symbol = const_cast<ecomplex::Symbol&>(complex->GetSymbol());
 		ecomplex::EditDialog dlg(m_stage, &symbol);
 		dlg.ShowModal();
 
  		//m_stage->resetCanvas();
		m_stage->ResetViewport();

		//////////////////////////////////////////////////////////////////////////

// 		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
// 		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	else if (libanim::Sprite* anim = dynamic_cast<libanim::Sprite*>(selected))
	{
 		libanim::PreviewDialog dlg(m_stage, &anim->GetSymbol());
 		dlg.ShowModal();

// 		m_stage->resetCanvas();
		m_stage->ResetViewport();
	}
	else if (escale9::Sprite* patch9 = dynamic_cast<escale9::Sprite*>(selected))
 	{
		escale9::Symbol& symbol = const_cast<escale9::Symbol&>(patch9->GetSymbol());
  		escale9::EditDialog dlg(m_stage, &symbol);
  		dlg.ShowModal();
  
// 		m_stage->resetCanvas();
		m_stage->ResetViewport();
 	}
	else if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(selected))
	{
		emesh::EditDialog dlg(m_stage, sprite);
		dlg.ShowModal();

//		m_stage->resetCanvas();
		m_stage->ResetViewport();
	}
	else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(selected))
	{
		d2d::TextDialog dlg(m_stage, font);
		dlg.ShowModal();
	}
	else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(selected))
	{
		std::vector<d2d::ISprite*> sprites;
		m_spritesImpl->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

		etexture::EditDialog dlg(m_stage, tex, sprites);
		dlg.ShowModal();

		m_stage->ResetViewport();
	}
	else if (libshape::Sprite* shape = dynamic_cast<libshape::Sprite*>(selected))
	{
		libshape::Symbol& symbol = const_cast<libshape::Symbol&>(shape->GetSymbol());
		libshape::EditDialog dlg(m_stage, &symbol);
		dlg.ShowModal();

		m_stage->ResetViewport();		
	}

	return false;
}

}