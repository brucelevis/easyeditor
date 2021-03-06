#ifndef _GLUE_IMAGE_SYMBOL_H_
#define _GLUE_IMAGE_SYMBOL_H_

#include "Symbol.h"

#include <string>

namespace glue
{

class Texture;

class ImageSymbol : public Symbol
{
public:
	ImageSymbol(const std::string& filepath);

	virtual void Draw(const sm_mat4& mt) const;

	void Draw25(const sm_mat4& mt) const;

private:
	void Load();

private:
	std::string m_filepath;

	Texture* m_tex;

}; // ImageSymbol

}

#endif // _GLUE_IMAGE_SYMBOL_H_