

#if !defined(__PAUSESCENE_H__)
#define __PAUSESCENE_H__

#include "scene.h"

namespace SFAS2014
{

/**
 * @class PauseScene
 *
 * @brief Displays a pause menu offering options to resume, restart and quit the current game
 */
class PauseScene : public Scene
{

public:
    PauseScene( const char* name );
    ~PauseScene();

    /**
     * @fn     bool PauseScene::Init( float graphicsScale );
     *
     * @brief  Initialise the pause scene
     *
	 * @param  graphicsScale  Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
    bool Init( float graphicsScale );

    /**
     * @fn    void PauseScene::Update(float deltaTime = 0.0f, float alphaMul = 1.0f);
     *
     * @brief Update the pause scene.
	 *
     * @param deltaTime   Number of seconds that have passed since Update() was called the last time. Default value is 0.0f.
     * @param alphaMul    Determines the alpha value of the scene. Default value is 1.
     */
	void Update(float deltaTime = 0.0f, float alphaMul = 1.0f);

	/**
     * @fn    void Scene::Reset();
     *
     * @brief Prepare the scene for activation by resetting it.
     */
    void Reset();

private: 

	CSprite* m_pBackground;    // The image used as background for this scene
	CSprite* m_pButtonResume;  // The button to activate to resume the currently paused game
	CSprite* m_pButtonRestart; // The button to activate to restart the game
	CSprite* m_pButtonQuit;    // The button to activate to get back to the title screen

	/**
     * @fn     bool PauseScene::InitUI( float graphicsScale );
     *
     * @brief  Initialise the UI elements of the pause scene
     *
	 * @param  graphicsScale  Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
	bool InitUI( float graphicsScale );
};

}

#endif  // __PAUSESCENE_H__


