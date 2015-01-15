
/*
 * (C) 2014 Search for a Star
 * 
 */

#include "GameSceneManager.h"

#include "TitleScene.h"
#include "GameScene.h"
#include "ResultsScene.h"
#include "ModeSelectionScene.h"
#include "PauseScene.h"
#include "HighscoresScene.h"
#include "CreditsScene.h"

#include "input.h"
#include "TimeConstants.h"
#include "IwGx.h"

using namespace SFAS2014;

//
//
// GameSceneManager class
//
//
GameSceneManager::GameSceneManager() : m_TimeInGame( 0.0f )
{
}

bool GameSceneManager::Init()
{
	// Create the game scenes 

	Scene* pScene = new TitleScene("TitleScene");
	if( 0 == pScene )
	{
		return false;
	}
	Add( pScene );
	
	pScene = new ModeSelectionScene("ModeSelectionScene");
	if( 0 == pScene )
	{
		return false;
	}
	Add( pScene );

	pScene = new GameScene("GameScene");
	if( 0 == pScene )
	{
		return false;
	}
	Add( pScene );

	pScene = new PauseScene("PauseScene");
	if( 0 == pScene )
	{
		return false;
	}
	Add( pScene );

	pScene = new ResultsScene("ResultsScene");
	if( 0 == pScene )
	{
		return false;
	}
	Add( pScene );

	pScene = new HighscoresScene("HighscoresScene");
	if( 0 == pScene )
	{
		return false;
	}
	Add( pScene );

	pScene = new CreditsScene("CreditsScene");
	if( 0 == pScene )
	{
		return false;
	}
	Add( pScene );

	if( !SceneManager::Init() )
	{
		return false;
	}

	SwitchTo( Find( "TitleScene" ) );

	return true;
}

GameSceneManager::~GameSceneManager()
{
}

void GameSceneManager::Update(float deltaTime)
{	
	SceneManager::Update( deltaTime );	

	m_Tweener.Update( deltaTime );
	m_TimerManager.Update( deltaTime );

	// This now counts the total time spent playing (across several playthroughs, could be used
	// for statistics etc), could be  moved to a separate class though
	if( m_Current == Find( "GameScene") )
	{
		m_TimeInGame += deltaTime;
	}
}

void GameSceneManager::FinaliseSceneSwitch( void )
{
	m_Next    -> SetInputActive(true);
    m_Next    -> SetActive(true);
    m_Current -> Update(0);           // Update one last time to ensure that last tweened values get set because on the next frame the scene will inactive
    m_Current -> SetActive(false);
	m_Current -> m_IsVisible = false;
    m_Current = m_Next;
	
    m_Next = 0;
}

void GameSceneManager::on_scene_transition_complete( Timer* pTimer, void* context )
{
	GameSceneManager* pSceneManager = static_cast<GameSceneManager*>( context );
	pSceneManager -> FinaliseSceneSwitch();
}

void GameSceneManager::SwitchTo(Scene* scene)
{

	if( scene != 0 )
	{
		m_Next = scene;
		if ( 0 == m_Current )
		{
			m_Current = m_Next;
			m_Current ->m_X = 0;
			m_Current->m_IsVisible = true;
			m_Current -> Reset();
			m_Current->SetActive(true);
			m_Current->SetInputActive(true);
			m_Next = 0;
		}
		else
		{
			m_Current -> SetInputActive(false);
			m_Next    -> m_IsVisible = true;
			m_Next    -> SetActive(true);
			m_Next    -> m_X = static_cast<float>( IwGxGetScreenWidth() );
			
			m_Next -> Reset();
			
			// start a slide animation
			m_Tweener.Tween(kfSceneSwitchAnimationLength,
                        FLOAT, &m_Next->m_X, 0.0f,
                        FLOAT, &m_Current->m_X, -static_cast<float>( IwGxGetScreenWidth() ),
                        EASING, Ease::sineIn,
                        END);

			// register a callback with a timer to be called when the slide is completed
			m_TimerManager.Add( "SceneSwitchTimer", kfSceneSwitchTimer, GameSceneManager::on_scene_transition_complete, this, 1 );
		}
	}
}
