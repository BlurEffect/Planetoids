/*
 * (C) 2014 Search for a Star
 * 
 */

#include "CreditsScene.h"
#include "IwGx.h"
#include "resources.h"
#include "input.h"	
#include "AudioHandler.h"

using namespace SFAS2014;

//
//
// TitleScene class
//
//

CreditsScene::CreditsScene( const char* name ) : Scene( name ), 
	                                             m_pBackground( 0 )
{
}

CreditsScene::~CreditsScene()
{

}

bool CreditsScene::Init( float graphicsScale )
{
	return InitUI( graphicsScale );
}

void CreditsScene::Update( float deltaTime, float alphaMul )
{
	if( !m_IsActive )
	{
		return;
	}

	Scene::Update( deltaTime, alphaMul );

	// If a touch is detected, proceed to the mode selection scene
	if ( m_IsInputActive && !g_pInput -> GetTouched() && g_pInput -> GetPrevTouched() )
    {
		// Reset input
		g_pInput->Reset();

		// Switch to the mode selection screen
		m_SceneManager -> SwitchTo( m_SceneManager -> Find( "ModeSelectionScene" ) );

		g_pAudioHandler->PlaySound( AudioHandler::SoundEffect::keSelect );
	}
}

void CreditsScene::Reset()
{
	// Placeholder
}

bool CreditsScene::InitUI( float graphicsScale )
{
	// Create the background sprite

	m_pBackground = new CSprite();
	if( 0 == m_pBackground )
	{
		return false;
	}
	m_pBackground -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pBackground -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.5f;
	m_pBackground -> SetImage( g_pResources -> GetBackgroundCredits() );
	m_pBackground -> m_W       = m_pBackground->GetImage() -> GetWidth();
	m_pBackground -> m_H       = m_pBackground->GetImage() -> GetHeight();
	m_pBackground -> m_AnchorX = 0.5;
	m_pBackground -> m_AnchorY = 0.5;
 
	// Fit background to screen size
	m_pBackground -> m_ScaleX  = static_cast<float>( IwGxGetScreenWidth() ) / m_pBackground -> GetImage() -> GetWidth();
	m_pBackground -> m_ScaleY  = static_cast<float>( IwGxGetScreenHeight() ) / m_pBackground -> GetImage() -> GetHeight();
	AddChild( m_pBackground );

	return true;
}