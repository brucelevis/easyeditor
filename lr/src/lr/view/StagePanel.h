#ifndef _LR_STAGE_PANEL_H_
#define _LR_STAGE_PANEL_H_

#include "dataset/CharacterAllDirections.h"
#include "preview/IPathfinding.h"

#include <ee/EditPanel.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/MultiShapesImpl.h>

#include <sprite2/Color.h>

namespace ee { class PropertySettingPanel; class LibraryPanel; }

namespace lr
{

class Grids;
class Quadtree;
class Layer;

class StagePanel : public ee::EditPanel, public ee::MultiSpritesImpl, public ee::MultiShapesImpl
{
public:
	StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
		ee::PropertySettingPanel* property, ee::LibraryPanel* library);
	virtual ~StagePanel();

	//
	// ee::EditPanel interface
	//
	virtual bool Update(float dt);

	//
	// ee::MultiSpritesImpl interface
	//
	virtual void TraverseSprites(ee::Visitor& visitor, 
		ee::DataTraverseType type = ee::DT_ALL, bool order = true) const;

	//
	// ee::MultiShapesImpl interface
	//
	virtual void TraverseShapes(ee::Visitor& visitor, 
		ee::DataTraverseType type = ee::DT_ALL) const;

	void EnableObserve(bool enable);

	void DebugDraw() const;

	void Pathfinding(const sm::vec2& start, const sm::vec2& end);
	void PointQuery(const sm::vec2& pos);

	void SetLayers(const std::vector<Layer*>& layers);
	const std::vector<Layer*>& GetLayers() const { return m_layers; }

	void BuildGrids(int w, int h);
	const Grids* GetGrids() const { return m_grids; }

	const CharacterAllDirections* GetCharaDirs() const { return &m_chara_dirs; }

	ee::EditOP* GetBaseOP() { return m_arrange_op; }

	void SetUpdateState(bool enable) {
		m_enable_update = enable;
	}

	ee::LibraryPanel* GetLibrary() { return m_library; }

	s2::Color& GetScreenMultiColor();
	s2::Color& GetScreenAddColor();

	void SetResDir(const std::string& res_dir) { m_res_dir = res_dir; }
	const std::string& GetResDir() const { return m_res_dir; }

protected:
	virtual void OnMouseHook(wxMouseEvent& event);
	virtual void OnKeyHook(int key_code);

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

private:
	void ChangeEditOP();

	void ReorderSprite(ee::Sprite* spr, bool up);
	void ReorderSpriteMost(ee::Sprite* spr, bool up);
	void SortSprites(std::vector<ee::Sprite*>& sprites);
	void InsertSprite(ee::Sprite* spr, int idx);
	void RemoveSprite(ee::Sprite* spr);
	void ClearSprite();

	void RemoveShape(ee::Shape* shape);
	void InsertShape(ee::Shape* shape);
	void ClearShape();

private:
	ee::LibraryPanel* m_library;

	ee::EditOP* m_arrange_op;

	Grids* m_grids;

	Quadtree* m_sindex;

	preview::IPathfinding* m_pathfinding;

	std::vector<Layer*> m_layers;

	CharacterAllDirections m_chara_dirs;

	bool m_enable_update;

	std::string m_res_dir;

}; // StagePanel

}

#endif // _LR_STAGE_PANEL_H_
