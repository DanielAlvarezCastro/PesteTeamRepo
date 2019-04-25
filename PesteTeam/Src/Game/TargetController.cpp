#include "TargetController.h"

TargetController::TargetController(GameObject* target, Ogre::Camera* camera_, string sightPath, int w_, int h_): BehaviourComponent(target), camera(camera_), w(w_), h(h_)
{
	newPos = Ogre::Vector2(0, 0);
	sight = GUIManager::instance()->createImage(sightPath, newPos.x, newPos.y, w, h, "ImageBox", "Sight");
	gameObject->getScene()->addGUIObject(sight);
}

TargetController::~TargetController()
{
}

void TargetController::Update(float t)
{
	if (getScreenspaceCoords(gameObject->getSceneNode(), newPos)) {
		sight->setVisible(true);
		sight->setPosition(newPos.x, newPos.y);
	}
	else sight->setVisible(false);
}

bool TargetController::getScreenspaceCoords(Ogre::SceneNode* object, Ogre::Vector2& result)
{


	// Get the center point of the object's bounding box
	Ogre::Vector3 point = object->_getDerivedPosition();

	// Is the camera facing that point? If not, return false
	Ogre::Plane cameraPlane = Plane(Ogre::Vector3(camera->getDerivedOrientation().zAxis()), camera->getDerivedPosition());
	if (cameraPlane.getSide(point) != Plane::NEGATIVE_SIDE)
		return false;

	// Transform the 3D point into screen space
	point = camera->getProjectionMatrix() * (camera->getViewMatrix() * point);

	// Transform from coordinate space [-1, 1] to [0, 1] and update in-value
	result.x = 800*((point.x / 2) + 0.5f)-(w/2);
	result.y = 600*(-(point.y / 2) + 0.5f)-(h/2);

	return true;
}
