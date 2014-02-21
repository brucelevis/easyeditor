#pragma once

#include "interfaces.h"

#include "algorithms/DelaunayTriangulation.h"

#include "dataset/Bitmap.h"
#include "dataset/Image.h"
#include "dataset/RawPixels.h"
#include "dataset/ISymbol.h"
#include "dataset/NullSymbol.h"
#include "dataset/SymbolFactory.h"
#include "dataset/SymbolMgr.h"
#include "dataset/ImageSymbol.h"
#include "dataset/ShapeSymbol.h"
#include "dataset/EShapeSymbol.h"
#include "dataset/MeshSymbol.h"
#include "dataset/CombinationSymbol.h"
#include "dataset/Patch9Symbol.h"
#include "dataset/FontBlankSymbol.h"
#include "dataset/ScriptsSymbol.h"
#include "dataset/ISprite.h"
#include "dataset/NullSprite.h"
#include "dataset/SpriteFactory.h"
#include "dataset/ImageSprite.h"
#include "dataset/ShapeSprite.h"
#include "dataset/EShapeSprite.h"
#include "dataset/MeshSprite.h"
#include "dataset/CombinationSprite.h"
#include "dataset/Patch9Sprite.h"
#include "dataset/TextSprite.h"
#include "dataset/FontSprite.h"
#include "dataset/ChainShape.h"
#include "dataset/CosineShape.h"
#include "dataset/PolygonShape.h"
#include "dataset/RectShape.h"
#include "dataset/CircleShape.h"
#include "dataset/BezierShape.h"
#include "dataset/PolygonBody.h"
#include "dataset/CircleBody.h"
#include "dataset/ChainBody.h"
#include "dataset/BodyData.h"
#include "dataset/BodyFactory.h"
#include "dataset/JointDef.h"
#include "dataset/Joint.h"
#include "dataset/Layer.h"
#include "dataset/BVFactory.h"
#include "dataset/ImageLoader.h"
#include "dataset/ImageSaver.h"

#include "view/OrthoCanvas.h"
#include "view/PerspectCanvas.h"
#include "view/Camera.h"
#include "view/LibraryPanel.h"
#include "view/ILibraryPage.h"
#include "view/LibraryList.h"
#include "view/LibraryImagePage.h"
#include "view/LibraryShapePage.h"
#include "view/LibraryEShapePage.h"
#include "view/LibraryMeshPage.h"
#include "view/LibraryCombinationPage.h"
#include "view/Library9PatchPage.h"
#include "view/LibraryFontBlankPage.h"
#include "view/LibraryScriptsPage.h"
#include "view/PropertySettingPanel.h"
#include "view/IPropertySetting.h"
#include "view/SpritePropertySetting.h"
#include "view/MultiSpritesPropertySetting.h"
#include "view/TextPropertySetting.h"
#include "view/SymbolPropertySetting.h"
#include "view/DialogStageCanvas.h"
#include "view/SpriteStageCanvas.h"
#include "view/ShapeStageCanvas.h"
#include "view/ShapesPanelImpl.h"
#include "view/SpritesPanelImpl.h"
#include "view/MultiShapesImpl.h"
#include "view/MultiSpritesImpl.h"
#include "view/PhysicsPanelImpl.h"
#include "view/ToolbarPanel.h"
#include "view/ViewlistPanel.h"
#include "view/ExceptionDlg.h"
#include "view/ExitDlg.h"
#include "view/Frame.h"
#include "view/VectorUtils.h"

#include "operator/AbstractEditOP.h"
#include "operator/NullEditOP.h"
#include "operator/ZoomViewOP.h"
#include "operator/ArrangeSpriteOP.h"
#include "operator/ArrangeSpriteFixOP.h"
#include "operator/DragPhysicsOP.h"
#include "operator/UniversalArrangeOP.h"
#include "operator/SelectSpritesOP.h"
#include "operator/SelectPixelsOP.h"
#include "operator/SelectShapesOP.h"
#include "operator/DrawRectangleOP.h"
#include "operator/DrawPolylineOP.h"
#include "operator/DrawPolygonEdgeOP.h"
#include "operator/DrawCurveOP.h"
#include "operator/PasteSymbolOP.h"
#include "operator/PasteSymbolRandomOP.h"
#include "operator/PasteSymbolCaptureOP.h"
#include "operator/PasteSymbolTileOP.h"
#include "operator/PasteSymbolPhysicsOP.h"
#include "operator/PasteSymbolPhysicsRandomOP.h"
#include "operator/PasteSpriteOP.h"
#include "operator/DrawPencilLineOP.h"
#include "operator/DrawPenLineOP.h"
#include "operator/DrawCosineCurveOP.h"
#include "operator/EditPolylineOP.h"
#include "operator/EditPolylinesOP.h"
#include "operator/EditRectOP.h"
#include "operator/EditCircleOP.h"
#include "operator/EditBezierOP.h"

#include "history/AbstractAtomicOP.h"

#include "component/AbstractEditCMPT.h"
#include "component/UniversalCMPT.h"
#include "component/PasteSymbolScaleCMPT.h"
#include "component/PasteSymbolRandomCMPT.h"
#include "component/PasteSymbolOffsetCMPT.h"
#include "component/PasteSymbolPhysicsCMPT.h"
#include "component/PasteSpriteCMPT.h"
#include "component/DrawPencilLineCMPT.h"
#include "component/DrawLineCMPT.h"
#include "component/DrawPolygonCMPT.h"
#include "component/NodeCaptureCMPT.h"
#include "component/EditNodesCMPT.h"
#include "component/EditPolylinesCMPT.h"
#include "component/EditPolygonCMPT.h"
#include "component/LayersMgrCMPT.h"
#include "component/MousePositionCMPT.h"
#include "component/UniversalArrangeCMPT.h"

#include "widgets/SetValueDialog.h"
#include "widgets/ListItem.h"
#include "widgets/VerticalImageList.h"
#include "widgets/DragTargetNull.h"
#include "widgets/SelectDrawTypeWidget.h"
#include "widgets/LayersMgrWidget.h"
#include "widgets/PasteSymbolRandomWidget.h"

#include "common/LibJpeg.h"
#include "common/log.h"
#include "common/FileNameParser.h"
#include "common/FilenameTools.h"
#include "common/PolylineFileAdapter.h"
#include "common/CircleFileAdapter.h"
#include "common/CombinationFileAdapter.h"
#include "common/MeshFileAdapter.h"
#include "common/ShapeFileAdapter.h"
#include "common/TexPackerAdapter.h"
#include "common/TexturePackerFileAdapter.h"
#include "common/ParticleFileAdapter.h"
#include "common/AnimFileAdapter.h"
#include "common/Settings.h"
#include "common/Math.h"
#include "common/visitors.h"
#include "common/tools.h"
#include "common/Random.h"
#include "common/Context.h"
#include "common/Exception.h"
#include "common/color_trans.h"
#include "common/runtime.h"
#include "common/scripts.h"

#include "render/b2Render.h"
#include "render/PrimitiveDraw.h"
#include "render/DrawSelectedShapeVisitor.h"
#include "render/DrawShapesVisitor.h"
#include "render/DrawSelectedSpriteVisitor.h"
#include "render/DrawSpritesVisitor.h"
#include "render/SpriteDraw.h"
#include "render/SpriteBatch.h"