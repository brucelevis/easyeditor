#include "SelectSpritesOP.h"
#include "EditDialog.h"
#include "StagePanel.h"

// #include <easyanim.h>
#include <easy9patch.h>

namespace libcomplex
{

SelectSpritesOP::SelectSpritesOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(editPanel, spritesImpl, propertyPanel, callback)
{
}

bool SelectSpritesOP::onMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::onMouseLeftDClick(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	d2d::ISprite* selected = m_spritesImpl->querySpriteByPos(pos);
	if (d2d::ComplexSprite* complex = dynamic_cast<d2d::ComplexSprite*>(selected))
	{
// 		d2d::ComplexSymbol& symbol = const_cast<d2d::ComplexSymbol&>(complex->getSymbol());
// 		EditDialog dlg(m_editPanel, &symbol);
// 		dlg.ShowModal();
// 
//  		m_editPanel->resetCanvas();

		//////////////////////////////////////////////////////////////////////////

		std::string cmd = "easycomplex.exe " + complex->getSymbol().getFilepath();
		WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
	}
	//else if (d2d::AnimSprite* anim = dynamic_cast<d2d::AnimSprite*>(selected))
	//{
 //		libanim::PreviewDialog dlg(Context::Instance()->stage, Context::Instance()->library, &anim->getSymbol());
 //		dlg.ShowModal();
 //		Context::Instance()->stage->resetCanvas();
	//}
 	else if (d2d::Patch9Sprite* patch9 = dynamic_cast<d2d::Patch9Sprite*>(selected))
 	{
  		d2d::Patch9Symbol& symbol = const_cast<d2d::Patch9Symbol&>(patch9->getSymbol());
  		lib9patch::EditDialog dlg(m_editPanel, &symbol);
  		dlg.ShowModal();
  
 		m_editPanel->resetCanvas();
 	}

	return false;
}

d2d::IPropertySetting* 
SelectSpritesOP::createPropertySetting(d2d::ISprite* sprite) const
{
	if (sprite)
		return d2d::SelectSpritesOP::createPropertySetting(sprite);
	else
		return new d2d::ComplexPropertySetting(m_editPanel, static_cast<StagePanel*>(m_editPanel)->getSymbol());
}

} // libcomplex