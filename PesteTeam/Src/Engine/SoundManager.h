#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <irrKlang.h>
#include <string>
#include <vector>
#include <map>

using namespace irrklang;

typedef std::map<std::string, ISound*> SoundMap;

class SoundManager {
private:
	// Basic variables of our manager, the instance ptr, the irrKlang engine variable & the root path 
	static SoundManager* instance_;
	ISoundEngine* engine_;
	std::string path_ = "./Assets/Audio/";

	// We store our looping sounds so we can access them one by one
	SoundMap soundsLooping_;
public:	
	SoundManager();
	~SoundManager();
	int Init();
	int PlaySound2D(std::string file, bool loop = false, bool startPaused = false);
	int Drop();

	// Singleton logic, if not created, create the instance, else return our existing instance
	static SoundManager* instance() {
		if (instance_ == nullptr)
			instance_ = new SoundManager();
		return instance_;
	}
};

#endif