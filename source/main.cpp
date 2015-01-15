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
 *
 * Edited for the Search for a Star Competition 2014
 *
 */

#include "Iw2D.h"
#include "input.h"
#include "AudioHandler.h"
#include "resources.h"
#include "GameSceneManager.h" 

// FRAME_TIME is the amount of time that a single frame should last in seconds
#define FRAME_TIME  (30.0f / 1000.0f)

using namespace SFAS2014;

// forward declaration
void CleanUp( void );

int main()
{
    // Initialise the 2D graphics system
    Iw2DInit();

    // Create resources
    g_pResources = new Resources();
	if( 0 == g_pResources )
	{
		CleanUp();
		return 0;
	}

    // Set up input systems
    g_pInput = new Input();
	if( 0 == g_pInput )
	{
		CleanUp();
		return 0;
	}

    // Set up and initialise audio system
    g_pAudioHandler = new SFAS2014::AudioHandler();
	if( 0 == g_pAudioHandler )
	{
		CleanUp();
		return 0;
	}

	if( !g_pAudioHandler ->Init() )
	{
		CleanUp();
		return 0;
	}

    // Create scene manager
	g_pSceneManager = new SFAS2014::GameSceneManager();
	if( 0 == g_pSceneManager )
	{
		CleanUp();
		return 0;
	}

	if( !g_pSceneManager -> Init() )
	{
		CleanUp();
		return 0;
	}

	// HACK!
	// For some reason the first sound is never actually played, so "play" one sound here (that won't be heard)
	// for all following ones to be played (I didn't find the reason for this behaviour)
	g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keSelect);

    // Loop forever, until the user or the OS performs some action to quit the app
    while (!s3eDeviceCheckQuitRequest())
    {
		// Used to lock the framerate
		uint64 newTime = s3eTimerGetMs();
		
        // Update input system
        g_pInput->Update();

        // Update audio system
        g_pAudioHandler->Update();

        // Update scene manager
        g_pSceneManager->Update(FRAME_TIME);

        // Clear the drawing surface
        Iw2DSurfaceClear(0xffff0000);

        // Render scene manager
        g_pSceneManager->Render();

        // Show the drawing surface
        Iw2DSurfaceShow();

		// Determine the time it took for the frame to complete and yield to the OS until
		// the FRAME_TIME has passed to ensure a steady framerate.
        int yield = (int)(FRAME_TIME * 1000 - (s3eTimerGetMs() - newTime));
        if (yield < 0)
		{
            yield = 0;
		}

        // Yield to the OS
        s3eDeviceYield(yield);
    }

    CleanUp();

    return 0;
}

void CleanUp()
{
	delete g_pSceneManager;
    delete g_pAudioHandler;
    delete g_pInput;
    delete g_pResources;

    Iw2DTerminate();
}