/*
 * (C) 2014 Search for a Star
 * 
 */

#if !defined(__GAMESCENEMANAGER_H__)
#define __GAMESCENEMANAGER_H__

#include "scene.h"
#include "Timer.h"
#include "IwTween.h"

using namespace IwTween;

namespace SFAS2014
{
/**
 * @class GameSceneManager
 *
 * @brief Manages the state of the game and how scene switches are handled.
 *
 */
class GameSceneManager : public SceneManager
{

public:
    GameSceneManager();
    ~GameSceneManager();

	/**
     * @fn    void GameSceneManager::Init(void);
     *
     * @brief Initialise the scene manager
     *
	 */
	bool Init();

    /**
     * @fn    void GameSceneManager::SwitchTo(Scene* scene);
     *
     * @brief Switch to a new scene.
     *
     * @param scene   The new scene.
     */
    void    SwitchTo( Scene* scene );

    /**
     * @fn    void GameSceneManager::Update(float deltaTime = 0.0f);
     *
     * @brief Updates all scenes managed by the manager
     *
     * @param deltaTime   Number of seconds that has passed since Update() was last called.
     */
    void Update( float deltaTime = 0.0f );

private: 
	CTweenManager m_Tweener;      // For animations
	TimerManager  m_TimerManager; // For timers

	float         m_TimeInGame;   // The total time spent playing (currently not used)          

	/**
     * @fn    static void GameSceneManager::on_scene_transition_complete( Timer* pTimer, void* context );
     *
     * @brief Callback function to be invoked after the scene transition is complete
	 *
	 * @param timer   The timer calling the callback.
     * @param context The instance of the Grid class associated to this callback.
     */
	static void on_scene_transition_complete( Timer* pTimer, void* context );

	/**
     * @fn    static void GameSceneManager::FinaliseSceneSwitch( void );
     *
     * @brief Completes the switching to a new scene.
     */
	void FinaliseSceneSwitch( void );
};

}

#endif  // __GAMESCENEMANAGER_H__



