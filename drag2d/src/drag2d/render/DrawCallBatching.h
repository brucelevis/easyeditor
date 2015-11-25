#ifndef _DRAG2D_DRAW_CALL_BATCHING_H_
#define _DRAG2D_DRAW_CALL_BATCHING_H_

#include <string>
#include <map>

struct dtex_cg;

namespace d2d
{

class Image;

class DrawCallBatching
{
public:
	void LoadBegin();
	int Load(const Image* img);
	void LoadEnd();

	void ReloadBegin();
	void Reload(const Image* img);
	void ReloadEnd();

	float* Query(const Image* img, int* id);

	dtex_cg* GetDtexCG();

	static DrawCallBatching* Instance();

private:
	DrawCallBatching();
	~DrawCallBatching();

private:
	std::map<std::string, int> m_path2id;

private:
	static DrawCallBatching* m_instance;

}; // DrawCallBatching

}

#endif // _DRAG2D_DRAW_CALL_BATCHING_H_