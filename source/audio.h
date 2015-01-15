/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */

#if !defined(__AUDIO_H__)
#define __AUDIO_H__

#include <list>

// forward declarations
class CIwSoundData;
class CIwSoundSpec;


/**
 * @struct    AudioSound
 *
 * @brief Audio sound effect class
 *
 */
class AudioSound
{
public:
    AudioSound();
    ~AudioSound();

    /**
     * @fn    bool AudioSound::Load(const char* filename)
     *
     * @brief Loads the given sound effect file.
     *
     * Loads a sound effect file stored in ADPCM IMA mono format and creates a sound spec that can be used to play the sound effect
     *
     * @param filename    The filename to load.
     *
     * @return    true if it succeeds, false if it fails.
     */
    bool Load( const char* filename );

	unsigned int  GetNameHash() const  { return m_NameHash; }
	CIwSoundData* GetSoundData() const { return m_SoundData; }
	CIwSoundSpec* GetSoundSpec() const { return m_SoundSpec; }

	void SetNameHash( unsigned int nameHash )    { m_NameHash = nameHash; }
	void SetSoundData( CIwSoundData* soundData ) { m_SoundData = soundData; }
	void SetSoundSpec( CIwSoundSpec* soundSpec ) { m_SoundSpec = soundSpec; }
private:
	
    unsigned int    m_NameHash;         // Hashed string name of sound effect
    CIwSoundData*   m_SoundData;        // Sound effect data
    CIwSoundSpec*   m_SoundSpec;        // Sound effect specification
};

/**
 * @class Audio
 *
 * @brief Audio manager
 *
 * The audio manager is responsible for management of sound effects, update of the audio system and play back of sound effects and background music.
 *
 * Example usage:
 * @code
 *    // Set up audio systems
 *    g_pAudio = new Audio();
 *
 *    // Update
 *    while (!s3eDeviceCheckQuitRequest())
 *    {
 *        // Update audio system
 *        g_pAudio->Update();
 *    }
 *
 *    // Cleanup
 *    delete g_pAudio;
 *
 * @endcode
 *
 */
class Audio
{

public:
    Audio();
    ~Audio();

    /**
     * @fn    AudioSound* Audio::findSound(unsigned int name_hash)
     *
     * @brief Searches for the named sound effect.
     *
     * @param name_hash   String hash of sound effect name.
     *
     * @return    null if it fails, else the found sound effect.
     */
    AudioSound* FindSound( unsigned int name_hash );

    /**
     * @fn    void Audio::Update()
     *
     * @brief Updates the audio system
     *
     * This should be called each main loop update to ensure that sound effects are played back consistently.
     */
    void Update();

    /**
     * @fn    void Audio::play_music(const char* filename, bool repeat)
     *
     * @brief Play the specified music file.
     *
     * @param filename    Name of the audio file to play (MP3 format only)
     * @param repeat      true to repeat play back.
     *
     */
    static void play_music( const char* filename, bool repeat = true );

    /**
     * @fn    void Audio::stop_music()
     *
     * @brief Stops currently playing music.
     *
     */
    static void stop_music();
 
    /**
     * @fn    void Audio::PlaySound(const char* filename)
     *
     * @brief Play the specified sound effect.
     *
     * Once a sound effect has been played it will be cached in the audio manager so future play back will not be hindered by having to re-load the sound effect from storage.
     *
     * @param filename    Filename of the sound effect.
     */
    void PlaySound( const char* filename );

private:
	std::list<AudioSound*> m_Sounds; // List of sound effects

	/**
     * @fn    AudioSound* Audio::PreloadSound(const char* filename)
     *
     * @brief Preload a sound effect.
     *
     * The sound effect will be added to the audio managers list of loaded sound effects. If the sound effect already exists within the audio manager then no
     * sound effect will be loaded. Its useful to preload larger sound effects during app set-up to prevent delays when playing audio back during game play.
     *
     * @param filename    Name of sound effect file.
     *
     * @return null if it fails, else the sound effect
     */
    AudioSound* PreloadSound(const char* filename);

};


#endif  // __AUDIO_H__

