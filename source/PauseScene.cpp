
#include "PauseScene.h"
#include "IwGx.h"
#include "resources.h"
#include "input.h"
#include "AudioHandler.h"
#include "GameScene.h"

using namespace SFAS2014;

//
//
// PauseScene class
//
//
PauseScene::PauseScene( const char* name ) : Scene( name ), 
											 m_pBackground( 0 ),
										     m_pButtonResume( 0 ),
										     m_pButtonRestart( 0 ),
										     m_pButtonQuit( 0 )
{
}

PauseScene::~PauseScene()
{
}

bool PauseScene::Init( float graphicsScale )
{
	return InitUI( graphicsScale );
}

void PauseScene::Update( float deltaTime, float alphaMul )
{
	if ( !m_IsActive )
	{
        return;
	}

    Scene::Update(deltaTime, alphaMul);

    // Check if one of the buttons was hit

	if ( m_IsInputActive && !g_pInput -> GetTouched() && g_pInput -> GetPrevTouched() )
    {
		// Reset the input
        g_pInput->Reset();

		int x = g_pInput -> GetX();
		int y = g_pInput -> GetY();

		if ( m_pButtonResume -> HitTest( x, y ) )
        {
			// Resume the game by reloading the game scene without resetting it
            Scene* pScene = m_SceneManager -> Find( "GameScene" );
			m_SceneManager  -> SwitchTo( pScene );
			g_pAudioHandler -> PlaySound( AudioHandler::SoundEffect::keSelect );
        }else if( m_pButtonRestart -> HitTest( x, y ) )
		{
			// Restart the game by reloading the game scene and having it reset in the process
			GameScene* pGame = static_cast<GameScene*>( m_SceneManager -> Find("GameScene") );
			// Set the game to unpaused to have it reset in the process of the scene switch
			pGame -> Resume(); 
			m_SceneManager  -> SwitchTo( pGame );
			g_pAudioHandler -> PlaySound( AudioHandler::SoundEffect::keSelect );
		}else if( m_pButtonQuit -> HitTest( x, y ) )
		{
			// Switch back to the title screen
			GameScene* pGame = static_cast<GameScene*>( m_SceneManager -> Find("GameScene") );
			Scene* pScene    = m_SceneManager -> Find( "TitleScene" );
			pGame -> Resume();
			m_SceneManager  -> SwitchTo( pScene );
			g_pAudioHandler -> PlaySound( AudioHandler::SoundEffect::keSelect );
		}else
		{
			// Another spot on the screen was hit
			g_pAudioHandler -> PlaySound( AudioHandler::SoundEffect::keDenied );
		}
    }
}

void PauseScene::Reset()
{
	// Placeholder
}

bool PauseScene::InitUI( float graphicsScale )
{
	// Create background sprite

	m_pBackground = new CSprite();
	if( 0 == m_pBackground )
	{
		return false;
	}
	m_pBackground -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pBackground -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.5f;
	m_pBackground -> SetImage( g_pResources -> GetBackgroundPauseMenu() );
	m_pBackground -> m_W       = m_pBackground->GetImage() -> GetWidth();
	m_pBackground -> m_H       = m_pBackground->GetImage() -> GetHeight();
	m_pBackground -> m_AnchorX = 0.5;
	m_pBackground -> m_AnchorY = 0.5;
 
	// Fit background to screen size
	m_pBackground -> m_ScaleX  = static_cast<float>( IwGxGetScreenWidth() ) / m_pBackground -> GetImage() -> GetWidth();
	m_pBackground -> m_ScaleY  = static_cast<float>( IwGxGetScreenHeight() ) / m_pBackground -> GetImage() -> GetHeight();
	AddChild( m_pBackground );


	// Create the three buttons

	m_pButtonResume = new CSprite();
	if( 0 == m_pButtonResume )
	{
		return false;
	}
	m_pButtonResume -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pButtonResume -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.3f;
	m_pButtonResume -> SetImage( g_pResources -> GetButtonResume() );
	m_pButtonResume -> m_W       = m_pButtonResume -> GetImage() -> GetWidth();
	m_pButtonResume -> m_H       = m_pButtonResume -> GetImage() -> GetHeight();
	m_pButtonResume -> m_AnchorX = 0.5;
	m_pButtonResume -> m_AnchorY = 0.5;
	m_pButtonResume -> m_ScaleX  = 0.5f * graphicsScale;
	m_pButtonResume -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild( m_pButtonResume );

	m_pButtonRestart = new CSprite();
	if( 0 == m_pButtonRestart )
	{
		return false;
	}
	m_pButtonRestart -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pButtonRestart -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.5f;
	m_pButtonRestart -> SetImage( g_pResources -> GetButtonRestart() );
	m_pButtonRestart -> m_W       = m_pButtonRestart -> GetImage() -> GetWidth();
	m_pButtonRestart -> m_H       = m_pButtonRestart -> GetImage() -> GetHeight();
	m_pButtonRestart -> m_AnchorX = 0.5;
	m_pButtonRestart -> m_AnchorY = 0.5;
	m_pButtonRestart -> m_ScaleX  = 0.5f * graphicsScale;
	m_pButtonRestart -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild( m_pButtonRestart );

	m_pButtonQuit = new CSprite();
	if( 0 == m_pButtonQuit )
	{
		return false;
	}
	m_pButtonQuit -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pButtonQuit -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.7f;
	m_pButtonQuit -> SetImage(g_pResources->GetButtonQuit());
	m_pButtonQuit -> m_W       = m_pButtonQuit -> GetImage() -> GetWidth();
	m_pButtonQuit -> m_H       = m_pButtonQuit -> GetImage() -> GetHeight();
	m_pButtonQuit -> m_AnchorX = 0.5;
	m_pButtonQuit -> m_AnchorY = 0.5;
	m_pButtonQuit -> m_ScaleX  = 0.5f * graphicsScale;
	m_pButtonQuit -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild( m_pButtonQuit );

	return true;
}