/*
 * (C) 2014 Search for a Star
 * 
 */

#if !defined(__CREDITSSCENE_H__)
#define __CREDITSSCENE_H__

#include "scene.h"

namespace SFAS2014
{

/**
 * @class CreditsScene
 *
 * @brief Displays the credits screen 
 *
 */
class CreditsScene : public Scene
{

public:
    CreditsScene( const char* name );
    ~CreditsScene();

    /**
     * @fn     bool CreditsScene::Init( float graphicsScale );
     *
     * @brief  Initialise the credits scene
     *
	 * @param  graphicsScale  Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
    bool Init( float graphicsScale );

    /**
     * @fn    void CreditsScene::Update(float deltaTime = 0.0f, float alphaMul = 1.0f);
     *
     * @brief Update the credits scene.
	 *
     * @param deltaTime Number of seconds that have passed since Update() was called the last time. Default value is 0.0f.
     * @param alphaMul  Determines the alpha value of the scene. Default value is 1.
     */
	void Update(float deltaTime = 0.0f, float alphaMul = 1.0f);

	/**
     * @fn    void Scene::Reset();
     *
     * @brief Prepare the scene for activation by resetting it.
     */
    void Reset();

private: 
	CSprite* m_pBackground; // The background image used in the scene

	/**
     * @fn     bool CreditsScene::InitUI( float graphicsScale );
     *
     * @brief  Initialise the UI elements of the credits scene
     *
	 * @param  graphicsScale Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
	bool InitUI( float graphicsScale );
};

}

#endif  // __CREDITSSCENE_H__


