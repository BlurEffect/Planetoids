#include "AudioHandler.h"

using namespace SFAS2014;

AudioHandler* g_pAudioHandler = 0;

//
//
// AudioHandler class
//
//

AudioHandler::AudioHandler() : m_pAudio( 0 )
{
	// Set the filenames for the sound effects

	m_SoundFiles[keSelect]   = "audio/select.wav";
	m_SoundFiles[keDenied]   = "audio/denied.wav";
	m_SoundFiles[keSuccess]  = "audio/success.wav";
	m_SoundFiles[keFail]     = "audio/fail.wav";
	m_SoundFiles[keGameOver] = "audio/game_over.wav";

	// Set the filenames for the music tracks

	m_MusicFiles[keMainMenu]    = "audio/music_menu.mp3";
	m_MusicFiles[keClassicGame] = "audio/music_classic.mp3";
	m_MusicFiles[keMemoryGame]  = "audio/music_memory.mp3";
}

AudioHandler::~AudioHandler() 
{
	delete m_pAudio;
}

bool AudioHandler::Init( void )
{
	// Create a new audio object
	m_pAudio = new Audio();

	if( 0 == m_pAudio )
	{
		return false;
	}
	return true;
}

void AudioHandler::Update( void )
{
	m_pAudio -> Update();
}

void AudioHandler::PlaySound( SoundEffect sound )
{
	m_pAudio -> PlaySound( m_SoundFiles[sound] );
}

void AudioHandler::PlayMusic( MusicTrack track )
{
	Audio::play_music( m_MusicFiles[track] );
}

void AudioHandler::StopMusic( void )
{
	Audio::stop_music();
}