#ifndef _LIBPACKER_NORMAL_PACK_H_
#define _LIBPACKER_NORMAL_PACK_H_

#include <vector>
#include <wx/string.h>

#include "Rect.h"

namespace libpacker
{

class NormalPack
{
public:
	NormalPack(const std::vector<wxString>& files);

	void Pack();

	void OutputInfo(const wxString& src_folder, const wxString& dst_file) const;
	void OutputImage(const wxString& filepath) const;

private:
	const std::vector<wxString>& m_files;

	std::vector<RectSize> m_src_sizes;

	std::vector<Rect> m_output;

	RectSize m_size;

}; // NormalPack

}

#endif // _LIBPACKER_NORMAL_PACK_H_