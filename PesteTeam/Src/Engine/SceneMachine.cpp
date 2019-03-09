#include "SceneMachine.h"

SceneMachine::SceneMachine()
{
}

SceneMachine::~SceneMachine()
{
}

void SceneMachine::addScene(Scene* s) {
	sceneStack.push(s);
}

void SceneMachine::delScene() {
	sceneStack.pop();
}

void SceneMachine::update() {
	sceneStack.top()->updateScene();;
}