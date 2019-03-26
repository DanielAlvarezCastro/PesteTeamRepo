#include "SoundManager.h"

// Singleton, we will access this variable with de instance function
SoundManager* SoundManager::instance_ = nullptr;

// Principal class constructor
SoundManager::SoundManager()
{
}

// Principal class deconstructor
SoundManager::~SoundManager()
{
}

int SoundManager::Init()
{
	// start the sound engine with default parameters
	engine_ = createIrrKlangDevice();

	// error starting up the engine
	if (!engine_)
		return -1;
}

int SoundManager::PlaySound2D(std::string file, bool loop, bool startPaused)
{
	// get the final path
	std::string finalPath = (path_ + file);

	// play some sound stream
	ISound* is_aux = engine_->play2D(finalPath.c_str(), loop, startPaused);

	if (loop)
	{
		soundsLooping_[file] = is_aux;
	}

	return 0;
}

int SoundManager::Drop()
{
	// delete engine
	engine_->drop(); 

	return 0;
}