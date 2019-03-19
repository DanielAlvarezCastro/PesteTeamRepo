#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

// To place sound in 3D
struct SoundVector {
	float x;
	float y;
	float z;

	SoundVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

// This will setup FMOD at high and low level
struct Implementation {
	Implementation();
	~Implementation();

	void Update();

	FMOD::Studio::System* mpStudioSystem;
	FMOD::System* mpSystem;

	int mnNextChannelId;

	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;
	typedef map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef map<string, FMOD::Studio::Bank*> BankMap;

	BankMap mBanks;
	EventMap mEvents;
	SoundMap mSounds;
	ChannelMap mChannels;
};

// Principal class
class SoundManager
{
private:
	static SoundManager* instance_;
public:
	// Constructor | Deconstructor
	SoundManager();
	~SoundManager();
	
	// Principal functions
	static void Init();
	static void Update();
	static void Shutdown();
	static int ErrorCheck(FMOD_RESULT result);

	// Every action that we can use with our FMOD API
	void LoadBank(const string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const string& strEventName);
	void LoadSound(const string& strSoundName, bool b3d = true, bool bLooping = false, bool bStream = false);
	void UnLoadSound(const string& strSoundName);
	//void Set3dListenerAndOrientation(const SoundVector& vPos = SoundVector{ 0, 0, 0 }, float fVolumedB = 0.0f);
	int PlaySound(const string& strSoundName, const SoundVector& vPos = SoundVector{ 0, 0, 0 }, float fVolumedB = 0.0f);
	void PlayEvent(const string& strEventName);
	void StopChannel(int nChannelId);
	void StopEvent(const string& strEventName, bool bImmediate = false);
	void GetEventParameter(const string& strEventName, const string& strEventParameter, float* parameter);
	void SetEventParameter(const string& strEventName, const string& strParameterName, float fValue);
	void StopAllChannels();
	void SetChannel3dPosition(int nChannelId, const SoundVector& vPosition);
	void SetChannelVolume(int nChannelId, float fVolumedB);
	bool IsPlaying(int nChannelId);
	bool IsEventPlaying(const string& strEventName) const;
	float dbToVolume(float db);
	float VolumeTodb(float volume);
	FMOD_VECTOR VectorToFmod(const SoundVector& vPosition);

	// Singleton logic, if not created, create the instance, else return our existing instance
	static SoundManager* instance() {
		if (instance_ == nullptr)
			instance_ = new SoundManager();
		return instance_;
	}
};

#endif