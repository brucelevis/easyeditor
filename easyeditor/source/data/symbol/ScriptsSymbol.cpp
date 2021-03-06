#include "ScriptsSymbol.h"

#include <fstream>

namespace ee
{

void ScriptsSymbol::LoadResources()
{
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	fin.seekg(0, std::ios::end);   
	m_data.reserve(fin.tellg());
	fin.seekg(0, std::ios::beg);
	m_data.assign((std::istreambuf_iterator<char>(fin)),
		std::istreambuf_iterator<char>());

	std::string str = m_data;
}

sm::rect ScriptsSymbol::GetSize(const Sprite* sprite) const 
{ 
//	return sm::rect(sm::vec2(FLT_MAX, FLT_MAX), 0, 0);
	return sm::rect();
}

}