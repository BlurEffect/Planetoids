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

/*
 * The part of the earth that can be seen on most of the background textures is from:
 * http://www.freeimageslive.com/galleries/space/earth/pics/g09_s66_37907.gif
 */

#include "resources.h"
#include "Iw2D.h"

using namespace SFAS2014;

// Global resources object
Resources* SFAS2014::g_pResources = 0;

//
//
// Resources class
//
//

Resources::Resources()
{
    // Load images

    m_PlanetGrey   = Iw2DCreateImage("textures/PlanetGrey.png"); 
    m_PlanetRed    = Iw2DCreateImage("textures/PlanetRed.png"); 
    m_PlanetBlue   = Iw2DCreateImage("textures/PlanetBlue.png");
    m_PlanetYellow = Iw2DCreateImage("textures/PlanetYellow.png");
	m_PlanetGreen  = Iw2DCreateImage("textures/PlanetGreen.png");
	m_Backside     = Iw2DCreateImage("textures/Backside.png");

	m_Background		      = Iw2DCreateImage("textures/Background.png");
	m_BackgroundTitle		  = Iw2DCreateImage("textures/BackgroundTitle.png");
	m_BackgroundModeSelection = Iw2DCreateImage("textures/BackgroundModeSelection.png");
	m_BackgroundResults       = Iw2DCreateImage("textures/BackgroundResults.png");
	m_BackgroundGame1         = Iw2DCreateImage("textures/BackgroundGame1.png");
	m_BackgroundGame2         = Iw2DCreateImage("textures/BackgroundGame2.png");
	m_BackgroundPauseMenu     = Iw2DCreateImage("textures/BackgroundPauseMenu.png");
	m_BackgroundHighscores    = Iw2DCreateImage("textures/BackgroundHighscores.png");
	m_BackgroundCredits       = Iw2DCreateImage("textures/BackgroundCredits.png");

	m_Button				  = Iw2DCreateImage("textures/Button.png");
	m_ButtonModeSelectClassic = Iw2DCreateImage("textures/ButtonModeSelectClassic.png");
	m_ButtonModeSelectMemory  = Iw2DCreateImage("textures/ButtonModeSelectMemory.png");
	m_ButtonResume			  = Iw2DCreateImage("textures/ButtonResume.png");
	m_ButtonRestart			  = Iw2DCreateImage("textures/ButtonRestart.png");
	m_ButtonQuit			  = Iw2DCreateImage("textures/ButtonQuit.png");
	m_ButtonMenu			  = Iw2DCreateImage("textures/ButtonMenu.png");
	m_ButtonContinue		  = Iw2DCreateImage("textures/ButtonContinue.png");
	m_ButtonCredits			  = Iw2DCreateImage("textures/ButtonCredits.png");

	m_TextModeSelectClassic   = Iw2DCreateImage("textures/TextModeSelectClassic.png");
	m_TextModeSelectMemory    = Iw2DCreateImage("textures/TextModeSelectMemory.png");

	// Load fonts

	m_FontSmall  = Iw2DCreateFont("fonts/font_small.gxfont");
	m_FontMedium = Iw2DCreateFont("fonts/font_medium.gxfont");
	m_FontLarge  = Iw2DCreateFont("fonts/font_large.gxfont");
}

Resources::~Resources()
{
	// Free all resources

    delete m_PlanetGrey;
    delete m_PlanetRed;
    delete m_PlanetBlue;
	delete m_PlanetYellow;
	delete m_PlanetGreen;
	delete m_Backside;
    
	delete m_Background;
	delete m_BackgroundTitle;
	delete m_BackgroundModeSelection;
	delete m_BackgroundResults;
	delete m_BackgroundGame1;
	delete m_BackgroundGame2;
	delete m_BackgroundPauseMenu;
	delete m_BackgroundHighscores;
	delete m_BackgroundCredits;

	delete m_Button;
	delete m_ButtonModeSelectClassic;
	delete m_ButtonModeSelectMemory;
	delete m_ButtonResume;
	delete m_ButtonRestart;
	delete m_ButtonQuit;
	delete m_ButtonMenu;			 
	delete m_ButtonContinue;
	delete m_ButtonCredits;

	delete m_TextModeSelectClassic;
	delete m_TextModeSelectMemory;

	delete m_FontSmall;
	delete m_FontMedium;
	delete m_FontLarge;
}





