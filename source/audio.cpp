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

#include "audio.h"
#include "s3e.h"
#include "IwHashString.h"
#include "IwSound.h"

//
//
// AudioSound class
//
//

AudioSound::AudioSound() : m_SoundData(0), 
						   m_SoundSpec(0)
{

}

bool AudioSound::Load( const char* filename )
{
    m_NameHash = IwHashString( filename );
    m_SoundData = CIwSoundWAV::Create( filename, 0, 0 );
    if ( 0 == m_SoundData )
	{
		return false;
	}
    m_SoundSpec = new CIwSoundSpec();
    m_SoundSpec -> SetData( m_SoundData );

    return true;
}

AudioSound::~AudioSound()
{
    if ( m_SoundSpec != 0 )
	{
        delete m_SoundSpec;
	}

    if ( m_SoundData != 0 )
	{
        delete m_SoundData;
	}
}

//
// Audio class
//
Audio::Audio()
{
    // Initialise IwSound
    IwSoundInit();
}

Audio::~Audio()
{
    for ( std::list<AudioSound*>::iterator it = m_Sounds.begin(); it!= m_Sounds.end(); ++it )
	{
        delete *it;
	}
    IwSoundTerminate();
}

AudioSound* Audio::FindSound( unsigned int name_hash )
{
    for ( std::list<AudioSound*>::iterator it = m_Sounds.begin(); it!= m_Sounds.end(); ++it )
	{
		if ( name_hash == (*it) -> GetNameHash() )
		{
            return *it;
		}
	}
    return 0;
}

void Audio::Update()
{
    IwGetSoundManager() -> Update();
}

void Audio::play_music( const char* filename, bool repeat )
{
    // We only support play back of MP3 music
    if ( s3eAudioIsCodecSupported( S3E_AUDIO_CODEC_MP3 ) )
	{
        s3eAudioPlay( filename, repeat ? 0 : 1 );
	}
}

void Audio::stop_music()
{
    s3eAudioStop();
}

AudioSound* Audio::PreloadSound( const char* filename )
{
    AudioSound* sound = FindSound( IwHashString( filename ) );
    if ( 0 == sound )
    {
        sound = new AudioSound();
        if ( !sound -> Load( filename ) )
        {
            delete sound;
            return 0;
        }
        m_Sounds.push_back( sound );
    }

    return sound;
}

void Audio::PlaySound( const char* filename )
{
    AudioSound* sound = PreloadSound( filename );
    if ( sound != 0 )
	{
		sound -> GetSoundSpec() -> Play();
	}
}



