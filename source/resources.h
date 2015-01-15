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
 */

#if !defined(__RESOURCES_H__)
#define __RESOURCES_H__


// forward declarations
class CIw2DImage;
class CIw2DFont;

namespace SFAS2014
{

/**
 * @class Resources
 *
 * @brief Resources class
 *
 * The resource class manages the lifetime of the following types of resources:
 * - Images
 * - Fonts
 */
class Resources
{
public:
    Resources();
    ~Resources();

    CIw2DImage* GetPlanetGrey( void ) const   { return m_PlanetGrey; }
    CIw2DImage* GetPlanetRed( void ) const	  { return m_PlanetRed; }
    CIw2DImage* GetPlanetBlue( void ) const   { return m_PlanetBlue; }
    CIw2DImage* GetPlanetYellow( void ) const { return m_PlanetYellow; }
	CIw2DImage* GetPlanetGreen( void ) const  { return m_PlanetGreen; }
	CIw2DImage* GetBackside( void ) const     { return m_Backside; }

	CIw2DImage* GetBackground( void )	const			   { return m_Background; }
    CIw2DImage* GetBackgroundTitle( void ) const		   { return m_BackgroundTitle; }
	CIw2DImage* GetBackgroundModeSelection( void ) const   { return m_BackgroundModeSelection; }
	CIw2DImage* GetBackgroundResults( void ) const	       { return m_BackgroundResults; }
	CIw2DImage* GetBackgroundGame1( void ) const		   { return m_BackgroundGame1; }
	CIw2DImage* GetBackgroundGame2( void ) const		   { return m_BackgroundGame2; }
	CIw2DImage* GetBackgroundPauseMenu( void ) const	   { return m_BackgroundPauseMenu; }
	CIw2DImage* GetBackgroundHighscores( void ) const	   { return m_BackgroundHighscores; }
	CIw2DImage* GetBackgroundCredits( void ) const		   { return m_BackgroundCredits; }

	CIw2DImage* GetButton( void ) const		 		     { return m_Button; }
	CIw2DImage* GetButtonModeSelectClassic( void ) const { return m_ButtonModeSelectClassic; }
    CIw2DImage* GetButtonModeSelectMemory( void ) const  { return m_ButtonModeSelectMemory; }

	CIw2DImage* GetButtonResume( void ) const	{ return m_ButtonResume; }
	CIw2DImage* GetButtonRestart( void ) const  { return m_ButtonRestart; }
	CIw2DImage* GetButtonQuit( void ) const	    { return m_ButtonQuit; }
	CIw2DImage* GetButtonMenu( void ) const	    { return m_ButtonMenu; }
	CIw2DImage* GetButtonContinue( void ) const { return m_ButtonContinue; }
	CIw2DImage* GetButtonCredits( void ) const  { return m_ButtonCredits; }

	CIw2DImage* GetTextModeSelectClassic( void ) const { return m_TextModeSelectClassic; }
    CIw2DImage* GetTextModeSelectMemory( void ) const  { return m_TextModeSelectMemory; }

	CIw2DFont*	GetFontSmall( void ) const  { return m_FontSmall; }
	CIw2DFont*	GetFontMedium( void ) const { return m_FontMedium; }
	CIw2DFont*	GetFontLarge( void ) const  { return m_FontLarge; }

private:

	// Images for the grid items

    CIw2DImage* m_PlanetGrey;
    CIw2DImage* m_PlanetRed;
    CIw2DImage* m_PlanetBlue;
    CIw2DImage* m_PlanetYellow;
	CIw2DImage* m_PlanetGreen;
	CIw2DImage* m_Backside;

	// Background images

	CIw2DImage*	m_Background;
	CIw2DImage*	m_BackgroundTitle;
	CIw2DImage*	m_BackgroundModeSelection;
	CIw2DImage*	m_BackgroundResults;
	CIw2DImage*	m_BackgroundGame1;
	CIw2DImage*	m_BackgroundGame2;
	CIw2DImage*	m_BackgroundPauseMenu;
	CIw2DImage*	m_BackgroundHighscores;
	CIw2DImage* m_BackgroundCredits;

	// Button images

	CIw2DImage* m_Button;
	CIw2DImage* m_ButtonModeSelectClassic;
	CIw2DImage* m_ButtonModeSelectMemory;
	CIw2DImage* m_ButtonResume;
	CIw2DImage* m_ButtonRestart;
	CIw2DImage* m_ButtonQuit;
	CIw2DImage* m_ButtonMenu;
	CIw2DImage* m_ButtonContinue;
	CIw2DImage* m_ButtonCredits;

	// Images that hold the game mode descriptions

	CIw2DImage* m_TextModeSelectClassic;
	CIw2DImage* m_TextModeSelectMemory;

	// Fonts of different sizes

    CIw2DFont*  m_FontSmall;  // small-sized font (size 14)
	CIw2DFont*  m_FontMedium; // medium-sized font (size 24)
	CIw2DFont*  m_FontLarge;  // large-sized font (size 36)
};

/**
 * @brief A pointer to the global resources object
 */
extern Resources* g_pResources;

}

#endif  // __RESOURCES_H__


