#include "SelectSpritesOP.h"

#include <easycomplex.h>

#include "Context.h"
#include "StagePanel.h"

namespace edb
{
	SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
		: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
	{
	}

	bool SelectSpritesOP::onKeyDown(int keyCode)
	{
		if (d2d::SelectSpritesOP::onKeyDown(keyCode)) return true;

		if (keyCode == WXK_SPACE)
		{
			d2d::SpriteSelection* selection = Context::Instance()->stage->getSpriteSelection();
			std::vector<d2d::ISprite*> sprites;
			selection->traverse(d2d::FetchAllVisitor<d2d::ISprite>(sprites));

			for (size_t i = 0, n = sprites.size(); i < n; ++i)
			{
				complex::Sprite* complex = dynamic_cast<complex::Sprite*>(sprites[i]);
				if (!complex) continue;

				complex::Symbol& symbol = const_cast<complex::Symbol&>(complex->getSymbol());
				complex::EditDialog dlg(m_editPanel, &symbol);
				dlg.ShowModal();

				m_editPanel->resetCanvas();
			}
		}

		return false;
	}

	bool SelectSpritesOP::onMouseLeftDClick(int x, int y)
	{
		if (d2d::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
		d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
		if (complex::Sprite* complex = dynamic_cast<complex::Sprite*>(selected))
		{
			complex::Symbol& symbol = const_cast<complex::Symbol&>(complex->getSymbol());
			complex::EditDialog dlg(m_editPanel, &symbol);
			dlg.ShowModal();

			m_editPanel->resetCanvas();
		}
		//else if (d2d::AnimSprite* anim = dynamic_cast<d2d::AnimSprite*>(selected))
		//{
		//		libanim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
		//		dlg.ShowModal();
		//		Context::Instance()->stage->resetCanvas();
		//}
		// 	else if (d2d::Patch9Sprite* patch9 = dynamic_cast<d2d::Patch9Sprite*>(selected))
		// 	{
		//  		d2d::Patch9Symbol& symbol = const_cast<d2d::Patch9Symbol&>(patch9->getSymbol());
		//  		lib9patch::EditDialog dlg(m_editPanel, NULL, &symbol);
		//  		dlg.ShowModal();
		//  
		////   		std::vector<d2d::Patch9Sprite*> sprites;
		////   		m_editPanel->traverseSprites(d2d::FetchAllVisitor<d2d::Patch9Sprite>(sprites));
		////   		for (size_t i = 0, n = sprites.size(); i < n; ++i)
		////   		{
		////   			d2d::ISprite* sprite = sprites[i];
		////   			if (&sprite->getSymbol() == &symbol)
		////   				sprite->buildBounding();
		////   		}
		//  
		// 		m_editPanel->resetCanvas();
		// 	}

		return false;
	}
}