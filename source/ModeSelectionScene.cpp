
#include "ModeSelectionScene.h"
#include "IwGx.h"
#include "resources.h"
#include "input.h"
#include "AudioHandler.h"
#include "GameScene.h"

using namespace SFAS2014;

//
//
// ModeSelectionScene class
//
//

ModeSelectionScene::ModeSelectionScene( const char* name ) : Scene( name ), 
															 m_pBackground( 0 ),
															 m_pButtonClassicMode( 0 ),
															 m_pTextClassicMode( 0 ),
															 m_pButtonMemoryMode( 0 ),
															 m_pTextMemoryMode( 0 ),
															 m_pButtonCredits( 0 )
{
}

ModeSelectionScene::~ModeSelectionScene()
{
}

bool ModeSelectionScene::Init( float graphicsScale )
{
	return InitUI( graphicsScale );
}

void ModeSelectionScene::Update(float deltaTime, float alphaMul)
{
	if (!m_IsActive)
        return;

    Scene::Update( deltaTime, alphaMul );

    // Check if one of the game mode buttons was hit

    if ( m_IsInputActive && !g_pInput -> GetTouched() && g_pInput -> GetPrevTouched() )
    {
		// Reset the input
        g_pInput -> Reset();

		int x = g_pInput -> GetX();
		int y = g_pInput -> GetY();

		if ( m_pButtonClassicMode -> HitTest( x, y ) )
        {
			// Switch to Classic game mode
            GameScene* pGame = static_cast<GameScene*>( m_SceneManager -> Find( "GameScene" ) );
			pGame -> SetGameMode( keClassic );
			m_SceneManager -> SwitchTo( pGame );
			g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keSelect);
        }else if( m_pButtonMemoryMode -> HitTest( x, y ) )
		{
			// Switch to Memory game mode
			GameScene* pGame = static_cast<GameScene*>( m_SceneManager -> Find( "GameScene" ) );
			pGame -> SetGameMode( keMemory);
			m_SceneManager -> SwitchTo( pGame );
			g_pAudioHandler->PlaySound( AudioHandler::SoundEffect::keSelect );
		}else if( m_pButtonCredits -> HitTest( x, y ) )
		{
			// Switch to credits
			Scene* pNext = m_SceneManager -> Find( "CreditsScene" );
			m_SceneManager -> SwitchTo( pNext );
			g_pAudioHandler->PlaySound( AudioHandler::SoundEffect::keSelect );
		}else
		{
			// Another spot on the screen was hit
			g_pAudioHandler->PlaySound( AudioHandler::SoundEffect::keDenied );
		}
    }
}

void ModeSelectionScene::Reset()
{
	// Placeholder
}

bool ModeSelectionScene::InitUI( float graphicsScale )
{
	// Create background sprite

	m_pBackground = new CSprite();
	if( 0 == m_pBackground )
	{
		return false;
	}
	m_pBackground -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pBackground -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.5f;
	m_pBackground -> SetImage( g_pResources -> GetBackgroundModeSelection() );
	m_pBackground -> m_W       = m_pBackground->GetImage() -> GetWidth();
	m_pBackground -> m_H       = m_pBackground->GetImage() -> GetHeight();
	m_pBackground -> m_AnchorX = 0.5;
	m_pBackground -> m_AnchorY = 0.5;
 
	// Fit background to screen size
	m_pBackground -> m_ScaleX  = static_cast<float>( IwGxGetScreenWidth() ) / m_pBackground -> GetImage() -> GetWidth();
	m_pBackground -> m_ScaleY  = static_cast<float>( IwGxGetScreenHeight() ) / m_pBackground -> GetImage() -> GetHeight();
	AddChild( m_pBackground );

	// Create the two buttonss

	m_pButtonClassicMode = new CSprite();
	if( 0 == m_pButtonClassicMode )
	{
		return false;
	}
	m_pButtonClassicMode -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pButtonClassicMode -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.3f;
	m_pButtonClassicMode -> SetImage(g_pResources->GetButtonModeSelectClassic());
	m_pButtonClassicMode -> m_W       = m_pButtonClassicMode->GetImage()->GetWidth();
	m_pButtonClassicMode -> m_H       = m_pButtonClassicMode->GetImage()->GetHeight();
	m_pButtonClassicMode -> m_AnchorX = 0.5;
	m_pButtonClassicMode -> m_AnchorY = 0.5;
	m_pButtonClassicMode -> m_ScaleX  = 0.5f * graphicsScale;
	m_pButtonClassicMode -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild(m_pButtonClassicMode);

	m_pButtonMemoryMode = new CSprite();
	if( 0 == m_pButtonMemoryMode )
	{
		return false;
	}
	m_pButtonMemoryMode -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pButtonMemoryMode -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.6f;
	m_pButtonMemoryMode -> SetImage(g_pResources->GetButtonModeSelectMemory());
	m_pButtonMemoryMode -> m_W       = m_pButtonMemoryMode->GetImage()->GetWidth();
	m_pButtonMemoryMode -> m_H       = m_pButtonMemoryMode->GetImage()->GetHeight();
	m_pButtonMemoryMode -> m_AnchorX = 0.5;
	m_pButtonMemoryMode -> m_AnchorY = 0.5;
	m_pButtonMemoryMode -> m_ScaleX  = 0.5f * graphicsScale;
	m_pButtonMemoryMode -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild( m_pButtonMemoryMode );

	// Create the game mode descriptions

	m_pTextClassicMode = new CSprite();
	if( 0 == m_pTextClassicMode )
	{
		return false;
	}
	m_pTextClassicMode -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pTextClassicMode -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.5f;
	m_pTextClassicMode -> SetImage(g_pResources->GetTextModeSelectClassic());
	m_pTextClassicMode -> m_W       = m_pTextClassicMode->GetImage()->GetWidth();
	m_pTextClassicMode -> m_H       = m_pTextClassicMode->GetImage()->GetHeight();
	m_pTextClassicMode -> m_AnchorX = 0.5;
	m_pTextClassicMode -> m_AnchorY = 0.5;
	m_pTextClassicMode -> m_ScaleX  = 0.5f * graphicsScale;
	m_pTextClassicMode -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild( m_pTextClassicMode );

	m_pTextMemoryMode = new CSprite();
	if( 0 == m_pTextMemoryMode )
	{
		return false;
	}
	m_pTextMemoryMode -> m_X = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pTextMemoryMode -> m_Y = static_cast<float>( IwGxGetScreenHeight() ) * 0.8f;
	m_pTextMemoryMode -> SetImage(g_pResources->GetTextModeSelectMemory());
	m_pTextMemoryMode -> m_W = m_pTextMemoryMode->GetImage()->GetWidth();
	m_pTextMemoryMode -> m_H = m_pTextMemoryMode->GetImage()->GetHeight();
	m_pTextMemoryMode -> m_AnchorX = 0.5;
	m_pTextMemoryMode -> m_AnchorY = 0.5;
	m_pTextMemoryMode -> m_ScaleX = 0.5f * graphicsScale;
	m_pTextMemoryMode -> m_ScaleY = 0.5f * graphicsScale;
	AddChild( m_pTextMemoryMode );

	m_pButtonCredits = new CSprite();
	if( 0 == m_pButtonCredits )
	{
		return false;
	}
	m_pButtonCredits -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pButtonCredits -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.9f;
	m_pButtonCredits -> SetImage(g_pResources->GetButtonCredits());
	m_pButtonCredits -> m_W       = m_pButtonCredits->GetImage()->GetWidth();
	m_pButtonCredits -> m_H       = m_pButtonCredits->GetImage()->GetHeight();
	m_pButtonCredits -> m_AnchorX = 0.5;
	m_pButtonCredits -> m_AnchorY = 0.5;
	m_pButtonCredits -> m_ScaleX  = 0.5f * graphicsScale;
	m_pButtonCredits -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild( m_pButtonCredits );

	return true;
}
