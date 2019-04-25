#include <BehaviourComponent.h>
#include <MainApp.h>
#include <GUIManager.h>

class TargetController : public BehaviourComponent 
{
public:
	TargetController(GameObject* target, Ogre::Camera* camera_, string sightPath, int w_, int h_);
	virtual ~TargetController();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
private:
	MyGUI::ImageBox* sight;
	Ogre::Camera* camera;
	Ogre::Vector2 newPos;
	int w;
	int h;
	bool getScreenspaceCoords(Ogre::SceneNode* object, Ogre::Vector2& result);
};
