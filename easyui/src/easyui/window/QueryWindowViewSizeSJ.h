#ifndef _EASYUI_QUERY_WINDOW_VIEW_SIZE_SJ_H_
#define _EASYUI_QUERY_WINDOW_VIEW_SIZE_SJ_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class QueryWindowViewSizeSJ : public d2d::Subject
{
public:
	struct Params
	{
		int width;
		int height;
	};

public:
	void Query(int& width, int& height);

	static QueryWindowViewSizeSJ* Instance();

private:
	QueryWindowViewSizeSJ(int id);

private:
	static QueryWindowViewSizeSJ* m_instance;	

}; // QueryWindowViewSizeSJ

}
}

#endif // _EASYUI_QUERY_WINDOW_VIEW_SIZE_SJ_H_