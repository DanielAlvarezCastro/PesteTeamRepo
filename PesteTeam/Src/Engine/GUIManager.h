#pragma once

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>
#include "Scene.h"
class GUIManager
{
private:
	static GUIManager* instance_;
	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;
public:
	GUIManager();
	~GUIManager();
	void initScene(Scene* escena);
	void createTextbox(string text, int x, int y, int w, int h, string skinName, string name, int size);
	MyGUI::TextBox* getTextBox(string name);
	static GUIManager* instance() {
		if (instance_ == nullptr)
			instance_ = new GUIManager();
		return instance_;
	}
};

