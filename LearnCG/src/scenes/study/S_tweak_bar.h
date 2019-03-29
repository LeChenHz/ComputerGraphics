#pragma once

#include "../Scene.h"

class S_tweak_bar : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
public:
	S_tweak_bar();
	~S_tweak_bar();
public:
	static void TW_CALL RunCB() { cout << "RunCB" << endl; };
};

