#ifndef _EASYEDITOR_RENDER_CONTEXT_STACK_H_
#define _EASYEDITOR_RENDER_CONTEXT_STACK_H_

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class StageCanvas;
class RenderContext;

class RenderContextStack
{
public:
	void Push(StageCanvas* canvas, RenderContext* render);
	void Pop();

	void SetModelView(const sm::vec2& offset, float scale);
	void SetProjection(int width, int height);

	bool GetModelView(sm::vec2& offset, float& scale) const;
	bool GetProjection(int& width, int& height) const;

	void SetCurrCtx(RenderContext* ctx);

	static RenderContextStack* Instance();

private:
	RenderContextStack();
	
private:
	struct Context
	{
		StageCanvas* canvas;
		RenderContext* render;
	};

private:
	std::vector<Context> m_stack;

	RenderContext* m_curr_ctx;

private:
	static RenderContextStack* m_instance;

}; // RenderContextStack

}

#endif // _EASYEDITOR_RENDER_CONTEXT_STACK_H_