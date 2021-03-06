#ifndef _TEST_MAIN_
#define _TEST_MAIN_

#include <shaderlab.h>

#include <gl/glew.h>
#include <gl/freeglut.h>

#include <glue.h>

#include <iostream>

#include "TestShape.h"
#include "TestSprite.h"
#include "TestMulti.h"
#include "TestSprite25.h"
#include "TestModel3.h"
#include "TestFilter.h"

//test::TestTask* task = new test::TestShape;
//test::TestTask* task = new test::TestSprite;
//test::TestTask* task = new test::TestMulti;
//test::TestTask* task = new test::TestSprite25;
//test::TestTask* task = new test::TestModel3;
test::TestTask* task = new test::TestFilter;

void 
display(void) {
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->GetContext()->Clear(0);
	task->Draw();
	sl_mgr->GetShader()->Commit();

	glFlush();
}

void
update(void) {
	task->Update();
	glutPostRedisplay();
}

void 
init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glue::RenderContext::Instance()->Init();

	task->Init();
}

void 
resize(int width, int height) {
	glViewport(0, 0, width, height);

	task->Resize(width, height);
}

int 
main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(800, 480);

	glutInitWindowPosition(100, 100);

	glutCreateWindow("hello");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutReshapeFunc(resize);

	glutMainLoop();

	return 0;
}

#endif // _TEST_MAIN_