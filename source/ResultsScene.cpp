/*
 * (C) 2014 Search for a Star
 * 
 */

#include "ResultsScene.h"
#include "IwGx.h"
#include "resources.h"
#include "input.h"
#include "AudioHandler.h"
#include "HighscoresScene.h"

using namespace SFAS2014;

//
//
// ResultsScene class
//
//

ResultsScene::ResultsScene( const char* name ) : Scene( name ), 
											     m_Score( 0 ),
											     m_GameMode( keClassic ),
											     m_pBackground( 0 ),
											     m_pScoreText( 0 )
{
}

ResultsScene::~ResultsScene()
{
}

bool ResultsScene::Init( float graphicsScale )
{
	return InitUI( graphicsScale );
}

void ResultsScene::Update( float deltaTime, float alphaMul )
{
	if( !m_IsActive )
	{
		return;
	}

	Scene::Update( deltaTime, alphaMul );

	// If a touch is detected, proceed to the highscore screen

	if ( m_IsInputActive && !g_pInput -> GetTouched() && g_pInput -> GetPrevTouched() )
    {
		// Reset input
		g_pInput -> Reset();

		HighscoresScene* pHighscoresScene = static_cast<HighscoresScene*>( GetSceneManager() -> Find("HighscoresScene") );
		
		// Send the data required by the highscore scene
		pHighscoresScene -> SetScore( m_Score );
		pHighscoresScene -> SetGameMode( m_GameMode );

		// Switch to the highscore screen
		GetSceneManager() -> SwitchTo( pHighscoresScene );

		g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keSelect);
	}
}

void ResultsScene::Reset()
{
	// Placeholder
}

void ResultsScene::SetScore( int score )
{
	m_Score = score;

	// Update the UI to show the new score
	char scoreBuffer[9];
	sprintf( scoreBuffer, "%d", m_Score );
	m_pScoreText -> SetText( scoreBuffer );
}

bool ResultsScene::InitUI( float graphicsScale )
{
	// Create the background sprite

	m_pBackground = new CSprite();
	if( 0 == m_pBackground )
	{
		return false;
	}
	m_pBackground -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pBackground -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.5f;
	m_pBackground -> SetImage( g_pResources -> GetBackgroundResults() );
	m_pBackground -> m_W       = m_pBackground->GetImage() -> GetWidth();
	m_pBackground -> m_H       = m_pBackground->GetImage() -> GetHeight();
	m_pBackground -> m_AnchorX = 0.5;
	m_pBackground -> m_AnchorY = 0.5;
 
	// Fit background to screen size
	m_pBackground -> m_ScaleX  = static_cast<float>( IwGxGetScreenWidth() ) / m_pBackground -> GetImage() -> GetWidth();
	m_pBackground -> m_ScaleY  = static_cast<float>( IwGxGetScreenHeight() ) / m_pBackground -> GetImage() -> GetHeight();
	AddChild( m_pBackground );

	m_pScoreText = new CLabel();
	if( 0 == m_pScoreText )
	{
		return false;
	}

	m_pScoreText -> m_X        = static_cast<float>( IwGxGetScreenWidth() ) / 2.0f;
	m_pScoreText -> m_Y        = static_cast<float>( IwGxGetScreenHeight() ) * 0.4f;
	m_pScoreText -> m_W        = static_cast<float>( IwGxGetScreenWidth() );
	m_pScoreText -> m_H        = 200;
	m_pScoreText -> SetFont( g_pResources -> GetFontLarge() );
	m_pScoreText -> SetText( "0" );
	m_pScoreText -> m_AnchorX  = 0.5;
	m_pScoreText -> m_AnchorY  = 0.0;
	m_pScoreText -> m_AlignHor = CIw2DFontAlign::IW_2D_FONT_ALIGN_CENTRE;
	m_pScoreText -> m_Color    = CColor(255,255,255,255);
	m_pScoreText -> m_ScaleX   = graphicsScale;
	m_pScoreText -> m_ScaleY   = graphicsScale;
	AddChild( m_pScoreText );

	return true;
}
