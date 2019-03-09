#pragma once
#include "NodeStack.h"
#include "Scene.h"
#include <stack>

class SceneMachine
{
protected:
	stack<Scene*> sceneStack;

public:
	SceneMachine();
	~SceneMachine();

	void addScene(Scene* s);
	void delScene();
	void update();
};

