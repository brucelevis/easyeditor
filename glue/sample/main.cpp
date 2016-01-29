#ifndef _TEST_MAIN_
#define _TEST_MAIN_

#include <gl/glew.h>
#include <gl/freeglut.h>

#include <glue.h>

#include "TestShape.h"
#include "TestSprite.h"
#include "TestMulti.h"

//test::TestTask* task = new test::TestShape;
//test::TestTask* task = new test::TestSprite;
test::TestTask* task = new test::TestMulti;

void 
display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	task->Draw();

	glue::ShaderMgr::Instance()->Flush();

	glFlush();
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
	glutReshapeFunc(resize);

	glutMainLoop();

	return 0;
}

#endif // _TEST_MAIN_