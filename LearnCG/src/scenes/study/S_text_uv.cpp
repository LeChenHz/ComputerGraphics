#include "S_text_uv.h"

void S_text_uv::initGL()
{
	text2D = new Text2D();
}

void S_text_uv::paintGL(float deltaTime)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const char *text = "FPS";
	text2D->printText2D(text, 10, 500, 60);
}

void S_text_uv::freeGL()
{
}

S_text_uv::S_text_uv()
{
}

S_text_uv::~S_text_uv()
{
	delete text2D;
}
