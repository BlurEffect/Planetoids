/*
 * (C) 2014 Search for a Star
 * 
 */

#include "GameScene.h"
#include "resources.h"
#include "IwGx.h"
#include "GridItem.h"
#include "input.h"
#include "Grid.h"
#include "AudioHandler.h"
#include "ResultsScene.h"
#include "TimeConstants.h"

using namespace SFAS2014;

//
//
// GameScene class
//
//

int GameScene::m_sGridXOffset          = 16; 
int GameScene::m_sGridYOffset          = 80;
int GameScene::m_sGridWidth			   = 6;   
int GameScene::m_sGridHeight		   = 8;   
int GameScene::m_sGridFieldWidth       = 128;		
int GameScene::m_sGridFieldHeight      = 128;

float GameScene::m_sTimeLimitInSeconds = 181; 

GameScene::GameScene( const char* name ) : Scene( name ), 
										   m_pGrid( 0 ), 
										   m_pBackground( 0 ), 
										   m_pScoreText( 0 ), 
									  	   m_pTimeText( 0 ), 
									  	   m_pButtonPause( 0 ),
										   m_GameState( keWaitingForStart ), 
										   m_GameMode( keClassic ), 
										   m_Time( 0.0f ), 
										   m_Score(0),
										   m_Paused( false )
{
	m_firstHit[0]  = -1;
	m_firstHit[1]  = -1;
	m_secondHit[0] = -1;
	m_secondHit[1] = -1;
}

GameScene::~GameScene()
{
	delete m_pGrid;
}

bool GameScene::Init( float graphicsScale )
{
	// Initialise the user interface
	if( !InitUI( graphicsScale ) )
	{
		return false;
	}

	// Seed the random number generator with the system time
	IwRandSeed( static_cast<int32>( s3eTimerGetMs() ) );

	// Create the grid
	m_pGrid = new Grid();
	if( 0 == m_pGrid )
	{
		return false;
	}
	
	// Initialise the grid
	if ( !m_pGrid -> Init( this, m_sGridXOffset, m_sGridYOffset, m_sGridHeight, m_sGridWidth, m_sGridFieldWidth, m_sGridFieldHeight, graphicsScale ) )
	{
		return false;
	}

	return true;
}

void GameScene::Reset()
{
	// If the game was paused, just continue to run with the current state, otherwise reset
	if( !m_Paused )
	{
		m_GameState = keWaitingForFirstInput;
		m_Time      = m_sTimeLimitInSeconds;
		m_Score		= 0;

		m_TimerManager.Clear();

		m_firstHit[0]  = -1;
		m_firstHit[1]  = -1;
		m_secondHit[0] = -1;
		m_secondHit[1] = -1;

		// Reset the grid
		m_pGrid -> Reset();

		// Game mode specific reset
		switch( m_GameMode )
		{
		case keClassic:
			g_pAudioHandler -> PlayMusic( AudioHandler::MusicTrack::keClassicGame );
			break;
		case keMemory:
			g_pAudioHandler -> PlayMusic( AudioHandler::MusicTrack::keMemoryGame );
			break;
		}

		// Show the reset score
		// Make sure the score buffer is big enough to hold 8 digits plus a null terminator
		char scoreBuffer[9];
		sprintf(scoreBuffer, "%d", m_Score );
		m_pScoreText->SetText(scoreBuffer);

	}else
	{
		Resume();
	}
}

void GameScene::Update(float deltaTime, float alphaMul)
{
	if( !m_IsActive || m_Paused )
	{
		return;
	}

	Scene::Update( deltaTime, alphaMul);

	if( m_GameState != keWaitingForStart )
	{
		// Update the game timer
		if( m_Time - deltaTime <= 0 )
		{
			// Timer has expired
			m_Time = 0;

			// Wait for the last turn of the player to finish
			if( m_GameState == keWaitingForFirstInput || m_GameState == keWaitingForSecondInput )
			{
				EndGame();
			}
		}else
		{
			m_Time -= deltaTime;
		}
	}

	// Update the grid
	m_pGrid -> Update( deltaTime );

	if (m_IsInputActive && !g_pInput->GetTouched() && g_pInput->GetPrevTouched())
    {
		g_pInput -> Reset();

		if ( m_pButtonPause -> HitTest( g_pInput->GetX(), g_pInput->GetY() ) )
        {
			// Player touched the pause menu button

            Scene* pScene = m_SceneManager -> Find("PauseScene");
			
			// Pause the game
			Pause();

			m_SceneManager -> SwitchTo( pScene );
			g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keSelect);
		}else
		{
			if( keWaitingForFirstInput == m_GameState || keWaitingForSecondInput == m_GameState )
			{
				// Depending on the current game mode, choose the proper update function
				switch(m_GameMode)
				{
				case keClassic:
					UpdateClassicGame( deltaTime, alphaMul );
					break;
				case keMemory:
					UpdateMemoryGame( deltaTime, alphaMul );
					break;
				}
			}else
			{
				g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keDenied);
			}
		}
	}

	// Update the HUD
	UpdateHUD();
}

void GameScene::UpdateHUD( void )
{
	// Update time displayed on the screen.

	int minutes, seconds;
	minutes = static_cast<int>( m_Time / 60 );
	seconds = static_cast<int>( m_Time - ( minutes * 60 ) );

	// Make sure the time buffer can hold 5 characters and a null terminator
	char timeBuffer[6];
	sprintf(timeBuffer, "%.2d:%.2d", minutes, seconds );
	m_pTimeText->SetText(timeBuffer);
}

void GameScene::UpdateClassicGame(float deltaTime, float alphaMul)
{
	int gridX = -1;
	int gridY = -1;

	// translate the screen coordinates to get the coordinates of the grid field that was touched
	m_pGrid -> ScreenCoordinatesToGridField( g_pInput->GetX(), g_pInput->GetY(), gridX, gridY );

	if( keWaitingForFirstInput == m_GameState )
	{
		if( gridX >= 0 && gridY >= 0 && m_pGrid -> IsFieldValid(gridX, gridY) ) 
		{
			// Touch hit a grid field, remember the position of the field
			m_firstHit[0] = gridX;
			m_firstHit[1] = gridY;

			// Highlight the selected field of the grid
			m_pGrid -> HighlightField( m_firstHit[0], m_firstHit[1] );

			// Now wait for the player to select second field
			m_GameState = keWaitingForSecondInput;
		}else
		{
			g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keDenied);
		}
	}else if( keWaitingForSecondInput == m_GameState )
	{
		// make sure the swap coordinates are valid and that the two hits are adjacent to each other
		if( ( gridX >= 0 && gridY >= 0) && m_pGrid -> IsFieldValid(gridX, gridY) && ( gridX != m_firstHit[0] || gridY != m_firstHit[1] ) && 
			( ( abs( gridX - m_firstHit[0] ) == 1 ) && ( gridY == m_firstHit[1] ) || ( abs( gridY - m_firstHit[1] ) == 1 ) && ( gridX == m_firstHit[0] ) ) )
		{
			m_secondHit[0] = gridX;
			m_secondHit[1] = gridY;

			m_GameState = keProcessingMove;

			m_pGrid -> ResetField( m_firstHit[0], m_firstHit[1] );

			// swap the two selected planets on the grid
			m_pGrid -> SwapPlanets( m_firstHit[0], m_firstHit[1], m_secondHit[0], m_secondHit[1] );

			// start a timer to wait for the swap to finish
			m_TimerManager.Add( "SwapTimer", kfSwapGridItemsTimer, GameScene::on_move_processed, this, 1 );
				
		}else
		{
			g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keDenied);
			// Reset hits and game state
			m_pGrid -> ResetField( m_firstHit[0], m_firstHit[1] );
			NextTurn();
		}
	}
}

void GameScene::UpdateMemoryGame(float deltaTime, float alphaMul)
{
	int gridX = -1;
	int gridY = -1;

	m_pGrid -> ScreenCoordinatesToGridField( g_pInput->GetX(), g_pInput->GetY(), gridX, gridY );

	if( keWaitingForFirstInput == m_GameState )
	{
		if( gridX >= 0 && gridY >= 0 && m_pGrid -> IsFieldValid(gridX, gridY) ) 
		{
			// Touch hit a grid field, remember the position of the field
			m_firstHit[0] = gridX;
			m_firstHit[1] = gridY;

			// Now wait for the player to select second field
			m_GameState = keWaitingForSecondInput;

			// Flip the first item
			m_pGrid -> FlipItem( m_firstHit[0], m_firstHit[1] );
		}else
		{
			g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keDenied);
		}
	}else if( keWaitingForSecondInput == m_GameState )
	{
		// make sure the swap coordinates are valid and that not the first item is hit again
		if( ( gridX >= 0 && gridY >= 0) && ( gridX != m_firstHit[0] || gridY != m_firstHit[1] ) && m_pGrid -> IsFieldValid(gridX, gridY))
		{
			m_secondHit[0] = gridX;
			m_secondHit[1] = gridY;

			m_GameState = keProcessingMove;

			// Flip the second item
			m_pGrid -> FlipItem( m_secondHit[0], m_secondHit[1] );

			// wait for second item to be completely flipped before taking further action and add a bit of time for the user
			// to memorize the item
			m_TimerManager.Add( "FlipShapes", kfFlipSecondMemoryTileTimer, GameScene::on_move_processed, this, 1 );
		}else
		{
			g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keDenied);
		}
	}
}

void GameScene::on_move_processed( Timer* pTimer, void* context )
{
	// The user's move has been processed, now decide how to proceed

	GameScene* game = static_cast<GameScene*>( context );

	switch( game -> m_GameMode )
	{
	case keClassic:
		{
			if( !game -> m_pGrid -> RemoveGroups() )
			{
				// If there are no groups of identical planets, reverse the user's move and swap back the planets
				game -> m_GameState = keReversingMove;
				game -> m_pGrid -> SwapPlanets( game -> m_secondHit[0], game -> m_secondHit[1], game -> m_firstHit[0], game -> m_firstHit[1] );

				g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keFail);

				// start a timer to wait for the swap back to finish
				game -> m_TimerManager.Add( "SwapBackTimer", kfSwapGridItemsTimer, GameScene::on_move_reversed, game, 1 );
			}else
			{
				// if there are groups of identical shapes, wait for their destruction to finish
				game -> m_GameState = keProcessingTurn;
		
				// start a timer to wait for grid items to be destroyed
				game -> m_TimerManager.Add( "DestroyGroupsTimer", kfDestroyGroupsTimer, GameScene::on_turn_processed, game, 1 );
			}
		}
		break;
	case keMemory:
		{
			if( !game -> m_pGrid -> CheckPair( game->m_firstHit[0], game->m_firstHit[1], game->m_secondHit[0], game->m_secondHit[1] ) )
			{
				// If the pair does not consist of two identical planets, reverse the user's move and flip both items
				game -> m_GameState = keReversingMove;
				game -> m_pGrid -> FlipItem( game -> m_firstHit[0], game -> m_firstHit[1] );
				game -> m_pGrid -> FlipItem( game -> m_secondHit[0], game -> m_secondHit[1] );

				g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keFail);

				// start a timer to wait for the swap back to finish
				game -> m_TimerManager.Add( "FlipBackTimer", kfFlipBackMemoryTilesTimer, GameScene::on_move_reversed, game, 1 );
			}else
			{
				// If the pair consists of identical planets, wait for their removal from the grid
				game -> m_GameState = keProcessingTurn;
		
				// start a timer to wait for grid items to be destroyed
				game -> m_TimerManager.Add( "DestroyPairTimer", kfDestroyPairTimer, GameScene::on_turn_processed, game, 1 );
			}
		}
		break;
	}

	/*
	if( keClassic == game -> m_GameMode )
	{
		if( !game -> m_pGrid -> RemoveGroups() )
		{
			// If there are no groups of identical planets, reverse the user's move and swap back the planets
			game -> m_GameState = keReversingMove;
			game -> m_pGrid -> SwapPlanets( game -> m_secondHit[0], game -> m_secondHit[1], game -> m_firstHit[0], game -> m_firstHit[1] );

			g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keFail);

			// start a timer to wait for the swap back to finish
			game -> m_TimerManager.Add( "SwapBackTimer", kfSwapGridItemsTimer, GameScene::on_move_reversed, game, 1 );
		}else
		{
			// if there are groups of identical shapes, wait for their destruction to finish
			game -> m_GameState = keProcessingTurn;
		
			// start a timer to wait for grid items to be destroyed
			game -> m_TimerManager.Add( "DestroyGroupsTimer", kfDestroyGroupsTimer, GameScene::on_turn_processed, game, 1 );
		}
	}else if( keMemory == game -> m_GameMode )
	{
		if( !game -> m_pGrid -> CheckPair( game->m_firstHit[0], game->m_firstHit[1], game->m_secondHit[0], game->m_secondHit[1] ) )
		{
			// If the pair does not consist of two identical planets, reverse the user's move and flip both items
			game -> m_GameState = keReversingMove;
			game -> m_pGrid -> FlipItem( game -> m_firstHit[0], game -> m_firstHit[1] );
			game -> m_pGrid -> FlipItem( game -> m_secondHit[0], game -> m_secondHit[1] );

			g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keFail);

			// start a timer to wait for the swap back to finish
			game -> m_TimerManager.Add( "FlipBackTimer", kfFlipBackMemoryTilesTimer, GameScene::on_move_reversed, game, 1 );
		}else
		{
			// If the pair consists of identical planets, wait for their removal from the grid
			game -> m_GameState = keProcessingTurn;
		
			// start a timer to wait for grid items to be destroyed
			game -> m_TimerManager.Add( "DestroyPairTimer", kfDestroyPairTimer, GameScene::on_turn_processed, game, 1 );
		}
	}*/
}

void GameScene::on_move_reversed( Timer* pTimer, void* context )
{
	// The user's move was invalid and thus was reversed, setup the game for the next turn
	GameScene* game = static_cast<GameScene*>( context );
	game -> NextTurn();
}

void GameScene::on_turn_processed( Timer* pTimer, void* context )
{
	// The user's move was valid and it has been processed, now allow the grid to refill.
	GameScene* game = static_cast<GameScene*>( context );
	game -> m_GameState = keWaitingForGridRefill;
	game -> m_pGrid -> RefillGrid();
}

void GameScene::AddScore( int score )
{
	// It is highly unlikely that such a high score can be reached in the given time limit but make sure
	// it is not surpassed as the HUD can only display 8 digits for the score number and errors would occur
	// otherwise.
	if( m_Score + score <= 99999999 )
	{
		m_Score += score;
	}else
	{
		m_Score = 99999999;
	}

	// Make sure the score buffer is big enough to hold 8 digits plus a null terminator
	char scoreBuffer[9];
	sprintf(scoreBuffer, "%d", m_Score );
	m_pScoreText->SetText(scoreBuffer);
}

void GameScene::NextTurn( void )
{
	m_GameState = keWaitingForFirstInput;

	m_firstHit[0]  = -1;
	m_firstHit[1]  = -1;
	m_secondHit[0] = -1;
	m_secondHit[1] = -1;

	g_pInput -> Reset();
}

void GameScene::EndGame( void )
{
	m_GameState = keGameFinished;

	ResultsScene* pNext = static_cast<ResultsScene*>( m_SceneManager -> Find("ResultsScene") );

	pNext -> SetScore( m_Score );
	pNext -> SetGameMode( m_GameMode );
	g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keGameOver);

	// Switch to the mode selection scene
	m_SceneManager -> SwitchTo( pNext );
}

void GameScene::SetGameMode( GameMode mode )
{
	m_GameMode = mode;
	m_pGrid->SetGameMode( mode );

	// Choose the correct background for the game mode
	switch( m_GameMode )
	{
	case keClassic:
		m_pBackground->SetImage(g_pResources->GetBackgroundGame1());
		break;
	case keMemory:
		m_pBackground->SetImage(g_pResources->GetBackgroundGame2());
		break;
	}
}

bool GameScene::InitUI( float graphicsScale )
{
	// Create the background sprite

	m_pBackground = new CSprite();
	if( 0 == m_pBackground )
	{
		return false;
	}
	m_pBackground -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pBackground -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.5f;
	m_pBackground -> SetImage( g_pResources -> GetBackgroundGame1() );
	m_pBackground -> m_W       = m_pBackground->GetImage() -> GetWidth();
	m_pBackground -> m_H       = m_pBackground->GetImage() -> GetHeight();
	m_pBackground -> m_AnchorX = 0.5;
	m_pBackground -> m_AnchorY = 0.5;
 
	// Fit background to screen size
	m_pBackground -> m_ScaleX  = static_cast<float>( IwGxGetScreenWidth() ) / m_pBackground -> GetImage() -> GetWidth();
	m_pBackground -> m_ScaleY  = static_cast<float>( IwGxGetScreenHeight() ) / m_pBackground -> GetImage() -> GetHeight();
	AddChild( m_pBackground );

	// Create and initialise the score text

	m_pScoreText = new CLabel();
	if( 0 == m_pScoreText )
	{
		return false;
	}
	
	m_pScoreText -> m_X        = static_cast<float>( IwGxGetScreenWidth() - 10 );
	m_pScoreText -> m_Y        = 10;
	m_pScoreText -> m_W        = 60;
	m_pScoreText -> m_H        = 30;
	m_pScoreText -> SetFont( g_pResources -> GetFontSmall() );
	m_pScoreText -> SetText("0");
	m_pScoreText -> m_AnchorX  = 1.0;
	m_pScoreText -> m_AnchorY  = 0.0;
	m_pScoreText -> m_AlignHor = CIw2DFontAlign::IW_2D_FONT_ALIGN_RIGHT;
	m_pScoreText -> m_Color    = CColor(255,255,255,255);
	m_pScoreText -> m_ScaleX   = graphicsScale;
	m_pScoreText -> m_ScaleY   = graphicsScale;
	AddChild( m_pScoreText );

	// Create and initialise the time text

	m_pTimeText = new CLabel();
	if( 0 == m_pTimeText )
	{
		return false;
	}
	m_pTimeText -> m_X        = 10;
	m_pTimeText -> m_Y        = 10;
	m_pTimeText -> m_W        = 60;
	m_pTimeText -> m_H        = 30;
	m_pTimeText -> SetFont( g_pResources->GetFontSmall() );
	m_pTimeText -> SetText("00:00");
	m_pTimeText -> m_AnchorX  = 0.0;
	m_pTimeText -> m_AnchorY  = 0.0;
	m_pTimeText -> m_AlignHor = CIw2DFontAlign::IW_2D_FONT_ALIGN_LEFT;
	m_pTimeText -> m_Color    = CColor(255,255,255,255);
	m_pTimeText -> m_ScaleX   = graphicsScale;
	m_pTimeText -> m_ScaleY   = graphicsScale;
	AddChild( m_pTimeText );

	// Create the button for the pause menu

	m_pButtonPause = new CSprite();
	if( 0 == m_pButtonPause )
	{
		return false;
	}
	m_pButtonPause -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pButtonPause -> m_Y       = 10;
	m_pButtonPause -> SetImage(g_pResources->GetButtonMenu());
	m_pButtonPause -> m_W       = m_pButtonPause->GetImage()->GetWidth();
	m_pButtonPause -> m_H       = m_pButtonPause->GetImage()->GetHeight();
	m_pButtonPause -> m_AnchorX = 0.5;
	m_pButtonPause -> m_AnchorY = 0.0;
	m_pButtonPause -> m_ScaleX  = 0.5f * graphicsScale;
	m_pButtonPause -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild(m_pButtonPause);

	return true;
}

void GameScene::Pause()
{
	m_Paused = true;
}

void GameScene::Resume()
{
	m_Paused = false;
}