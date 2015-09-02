#ifndef _LIBCOCO_EPD_COCO_PACKER_H_
#define _LIBCOCO_EPD_COCO_PACKER_H_

#include <drag2d.h>

#include "TPParser.h"

namespace ebuilder { class CodeGenerator; }
namespace ecomplex { class Symbol; }
namespace libanim { class Symbol; }
namespace escale9 { class Symbol; }
namespace emesh { class Symbol; class Sprite; }
namespace eterrain2d { class Symbol; class Sprite; }
namespace etexture { class Symbol; class Sprite; }
namespace eicon { class Symbol; class Sprite; }
namespace epbin { class ImageIDer; }

namespace libcoco
{
namespace epd
{

class CocoPacker
{
public:
	CocoPacker(const std::vector<const d2d::ISymbol*>& symbols, 
		const TextureMgr& tex_mgr);
	CocoPacker(const std::vector<const d2d::ISymbol*>& symbols, 
		const TextureMgr& tex_mgr, const std::string& img_id_file, 
		float scale = 1.0f);
	~CocoPacker();

	void Parser();

	void Output(const char* filepath) const;

private:
	enum PicFixType
	{
		e_null = 0,
		e_xfix,
		e_yfix,
		e_bothfix,
	};

private:
	void ResolveSymbols();

	void ParserPicture(const d2d::ImageSprite* sprite, PicFixType tsrc = e_null, PicFixType tscreen = e_null);
	void ParserPicture(const d2d::ImageSymbol* symbol, PicFixType tsrc = e_null);
	int ParserIcon(const eicon::Sprite* sprite);
	void ParserIcon(const eicon::Symbol* symbol, float process, int id);
	void ParserComplex(const ecomplex::Symbol* symbol);
	void ParserAnimation(const libanim::Symbol* symbol);
	void ParserScale9(const escale9::Symbol* symbol);

	void CalSrcFromUV(d2d::Vector src[4], TPParser::Picture* picture);
	void CalSrcFromUVFixed(d2d::Vector src[4], TPParser::Picture* picture);
	int ParserMesh(const emesh::Sprite* sprite);
	int ParserTerrain2D(const eterrain2d::Sprite* sprite);
	int ParserTexture(const etexture::Sprite* sprite);

	void ParserSymbolBase(const d2d::ISymbol* symbol);

	void ParserSpriteForComponent(const d2d::ISprite* sprite, 
		std::vector<int>& ids, std::map<int, std::vector<std::string> >& unique, 
		std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const d2d::ISprite* sprite, int index,
		const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order);
	void ParserSpriteForFrame(const d2d::ISprite* sprite, const std::vector<std::pair<int, std::string> >& order,
		const std::map<int, int>& map_id2idx);
	void ParserSpriteForFrame(const d2d::ISprite* sprite, int id, bool forceMat);
	void ParserImageForFrame(const d2d::ISprite* sprite, int id);
	void ParserFontForFrame(const d2d::FontSprite* font, int id);

	void TransToMat(const d2d::ISprite* sprite, float mat[6], bool force = false) const;

	void GetColorAssignParams(const d2d::ISprite* sprite, std::vector<std::string>& params) const;

	int QueryIconID(const eicon::Sprite* icon) const;

private:
	ebuilder::CodeGenerator* m_gen;

	float m_scale;

	TPParser m_parser;

	int m_id;
	std::map<const d2d::ISprite*, int> m_mapSpriteID;
	std::map<const d2d::ISymbol*, int> m_mapSymbolID;

	// patch for multi mesh symbol
	struct SpriteID
	{
		d2d::ISprite* sprite;
		int id;
	};
	std::map<const d2d::ISymbol*, std::vector<SpriteID> > m_map_symbol2ids;

	std::map<const eicon::Symbol*, std::map<float, int> > m_map_icon2ids;

	epbin::ImageIDer* m_img_ider;

	std::set<int> m_terrain2d_ids;

}; // COCCode

}
}

#endif // _LIBCOCO_EPD_COCO_PACKER_H_