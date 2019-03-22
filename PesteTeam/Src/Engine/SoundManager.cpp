#include "SoundManager.h"

// Initialize FMOD at high level (mpStudioSystem) and low level (mpSystem)
Implementation::Implementation() {
	mpStudioSystem = NULL;
	//SoundManager::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
	FMOD::Studio::System::create(&mpStudioSystem);
	SoundManager::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL));

	mpSystem = NULL;
	SoundManager::ErrorCheck(mpStudioSystem->getLowLevelSystem(&mpSystem));
}

// Shutdown, it cleans FMOD variables
Implementation::~Implementation() {
	SoundManager::ErrorCheck(mpStudioSystem->unloadAll());
	SoundManager::ErrorCheck(mpStudioSystem->release());
}

// In this function we check if a channel has stopped playing, 
// if it has, we destroy it so we can clear up a channel to use
void Implementation::Update() {
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}
	SoundManager::ErrorCheck(mpStudioSystem->update());
}

// Our Implementation instance
Implementation* sgpImplementation = nullptr;

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

// To initialize low level variable
void SoundManager::Init() {
	sgpImplementation = new Implementation;
}

// Basically calls Implementation update function
void SoundManager::Update() {
	sgpImplementation->Update();
}

// We need to be able to load sounds. We'll take in the filename along with some parameters about streaming, 
// looping, and whether or not it's a 3D sound and then load that sound and store it in our sound map
void SoundManager::LoadSound(const std::string& strSoundName, bool b3d, bool bLooping, bool bStream)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt != sgpImplementation->mSounds.end())
		return;

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;
	SoundManager::ErrorCheck(sgpImplementation->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		sgpImplementation->mSounds[strSoundName] = pSound;
	}
}

// We need to be able to unload sounds to free up memory
void SoundManager::UnLoadSound(const std::string& strSoundName)
{
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
		return;

	SoundManager::ErrorCheck(tFoundIt->second->release());
	sgpImplementation->mSounds.erase(tFoundIt);
}

// We search the sound in the sound map, if not loaded, we load it
// If it is loaded, we just call FMOD playsound
// Sound should be stopped so we don't get a pop in the audio when we set parameters
// If the channel was set right then we update all the possible parameters like volume and position
// and then unpause the sound. 
// Finally we return the channel id encase we refer to it later.
int SoundManager::PlaySoundA(const string& strSoundName, const SoundVector& vPosition, float fVolumedB)
{
	int nChannelId = sgpImplementation->mnNextChannelId++;
	auto tFoundIt = sgpImplementation->mSounds.find(strSoundName);
	if (tFoundIt == sgpImplementation->mSounds.end())
	{
		LoadSound(strSoundName);
		tFoundIt = sgpImplementation->mSounds.find(strSoundName);
		if (tFoundIt == sgpImplementation->mSounds.end())
		{
			return nChannelId;
		}
	}
	FMOD::Channel* pChannel = nullptr;
	SoundManager::ErrorCheck(sgpImplementation->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(vPosition);
			SoundManager::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		SoundManager::ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		SoundManager::ErrorCheck(pChannel->setPaused(false));
		sgpImplementation->mChannels[nChannelId] = pChannel;
	}
	return nChannelId;
}

// Set the position of a sound
void SoundManager::SetChannel3dPosition(int nChannelId, const SoundVector& vPosition)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	SoundManager::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

// Set the volume of a sound
void SoundManager::SetChannelVolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	SoundManager::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}

// Check if a sound is playing 
bool SoundManager::IsPlaying(int nChannelId)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return false;

	bool b_isPlaying = false;
	SoundManager::ErrorCheck(tFoundIt->second->isPlaying(&b_isPlaying));
	
	return b_isPlaying;
}

// Stop an audio
void SoundManager::StopChannel(int nChannelId)
{
	auto tFoundIt = sgpImplementation->mChannels.find(nChannelId);
	if (tFoundIt == sgpImplementation->mChannels.end())
		return;

	SoundManager::ErrorCheck(tFoundIt->second->stop());
}

// Stop all audios
void SoundManager::StopAllChannels() 
{
	for (auto it = sgpImplementation->mChannels.begin(), itEnd = sgpImplementation->mChannels.end(); it != itEnd; ++it)
	{
		if (it != sgpImplementation->mChannels.end())
			SoundManager::ErrorCheck(it->second->stop());
	}
}

// Banks are what stores all the sounds and information for each event 
// We load them much like we loaded the sounds
void SoundManager::LoadBank(const std::string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags) {
	auto tFoundIt = sgpImplementation->mBanks.find(strBankName);
	if (tFoundIt != sgpImplementation->mBanks.end())
		return;
	FMOD::Studio::Bank* pBank;
	SoundManager::ErrorCheck(sgpImplementation->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
	if (pBank) {
		sgpImplementation->mBanks[strBankName] = pBank;
	}
}

// Each event stored in a bank has to be loaded separately which helps save memory 
// We load Events like everything else except we load it in two parts: EventDescription and EventInstance. 
// The description is the information and the instance is what actually plays the sound.
void SoundManager::LoadEvent(const std::string& strEventName) {
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit != sgpImplementation->mEvents.end())
		return;
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	SoundManager::ErrorCheck(sgpImplementation->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
	if (pEventDescription) {
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		SoundManager::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) {
			sgpImplementation->mEvents[strEventName] = pEventInstance;
		}
	}
}

// We just look to see that the event has been loaded, if it hasn't we load it, and then tell it to play
void SoundManager::PlayEvent(const string &strEventName) {
	auto tFoundit = sgpImplementation->mEvents.find(strEventName);
	if (tFoundit == sgpImplementation->mEvents.end()) {
		LoadEvent(strEventName);
		tFoundit = sgpImplementation->mEvents.find(strEventName);
		if (tFoundit == sgpImplementation->mEvents.end())
			return;
	}
	tFoundit->second->start();
}

// We do the same thing for stopping the event except we don't care if it's loaded
void SoundManager::StopEvent(const string &strEventName, bool bImmediate) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	SoundManager::ErrorCheck(tFoundIt->second->stop(eMode));
}

// It's always important to see if an event is playing or not. 
// To do this we have to get the playback state of the event which tells us if it's currently being played
bool SoundManager::IsEventPlaying(const string &strEventName) const {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return true;
	}
	return false;
}

// With this function we can get parameters of events dynamically
void SoundManager::GetEventParameter(const string &strEventName, const string &strParameterName, float* parameter) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	SoundManager::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	SoundManager::ErrorCheck(pParameter->getValue(parameter));
}

// With this function we can set parameters of events dynamically
void SoundManager::SetEventParameter(const string &strEventName, const string &strParameterName, float fValue) {
	auto tFoundIt = sgpImplementation->mEvents.find(strEventName);
	if (tFoundIt == sgpImplementation->mEvents.end())
		return;

	FMOD::Studio::ParameterInstance* pParameter = NULL;
	SoundManager::ErrorCheck(tFoundIt->second->getParameter(strParameterName.c_str(), &pParameter));
	SoundManager::ErrorCheck(pParameter->setValue(fValue));
}

// Simple aux function to reduce how much code we have to write
FMOD_VECTOR SoundManager::VectorToFmod(const SoundVector& vPosition) {
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

// Simple aux function to reduce how much code we have to write
float SoundManager::dbToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

// Simple aux function to reduce how much code we have to write
float SoundManager::VolumeTodb(float volume)
{
	return 20.0f * log10f(volume);
}

// Little function that does FMOD error checking for us
int SoundManager::ErrorCheck(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		cout << "FMOD ERROR " << result << endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}

// Function to clean everything up and this just deletes the Implementation
void SoundManager::Shutdown() {
	delete sgpImplementation;
}