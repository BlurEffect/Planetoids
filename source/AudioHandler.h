#if !defined(__AUDIOHANDLER_H__)
#define __AUDIOHANDLER_H__

#include "audio.h"


namespace SFAS2014
{

/**
 * @class AudioHandler
 *
 * @brief Application specific interface to the audio class
 */
class AudioHandler
{
public:

	AudioHandler( void );
	~AudioHandler( void );

	/**
	 * @enum SoundEffect
	 *
	 * @brief Holds identifiers for all sound effects used by the game
	 */
	enum SoundEffect
	{
		keSelect,
		keDenied,
		keSuccess,
		keFail,
		keGameOver,
		keNumberOfSoundEffects
	};

	/**
	 * @enum MusicTrack
	 *
	 * @brief Holds identifiers for all music tracks used by the game
	 */
	enum MusicTrack
	{
		keMainMenu,
		keClassicGame,
		keMemoryGame,
		keNumberOfMusicTracks
	};

	/**
     * @fn	   void AudioHandler::Init( void );
     *
     * @brief  Initialise the audio handler
	 *
	 * @return True if the initialisation was successful, false if an error occurred
     */
	bool Init( void );

	/**
     * @fn    void AudioHandler::Update( void )
     *
     * @brief Updates the audio system
     */
	void Update( void );

	/**
     * @fn	  void AudioHandler::PlaySound( SoundEffect sound );
     *
     * @brief Plays a sound effect
	 *
	 * @param sound The sound effect to play
     */
	void PlaySound( SoundEffect sound );

	/**
     * @fn    void AudioHandler::PlayMusic( MusicTrack track );
     *
     * @brief Plays a music track
	 *
	 * @param track The music track to play
     */
	void PlayMusic( MusicTrack track );

	/**
     * @fn    void AudioHandler::StopMusic( void );
     *
     * @brief Stops any music currently playing
     */
    void StopMusic( void );

private:
	Audio*		m_pAudio;                             // The audio object wrapped by this handler, to which calls are forwarded

	const char* m_SoundFiles[keNumberOfSoundEffects]; // Contains the filenames for all sound effects
	const char* m_MusicFiles[keNumberOfMusicTracks];  // Contains the filenames for all music tracks
};

}

/**
 * @brief A pointer to the global audio handler
 */
extern SFAS2014::AudioHandler* g_pAudioHandler;


#endif //__AUDIOHANDLER_H__