/*
 * (C) 2014 Search for a Star
 * 
 */

#if !defined(__RESULTSSCENE_H__)
#define __RESULTSSCENE_H__

#include "scene.h"
#include "GameScene.h"

namespace SFAS2014
{

/**
 * @class ResultsScene
 *
 * @brief Displays the final score for the finished game
 *
 */
class ResultsScene : public Scene
{

public:
    ResultsScene( const char* name );
    ~ResultsScene();

     /**
     * @fn     bool ResultsScene::Init( float graphicsScale );
     *
     * @brief  Initialise the results scene
     *
	 * @param  graphicsScale Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
    bool Init( float graphicsScale );

    /**
     * @fn    void ResultsScene::Update(float deltaTime = 0.0f, float alphaMul = 1.0f);
     *
     * @brief Update the results scene.
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
	
	/**
     * @fn    void ResultsScene::SetScore( int score );
     *
     * @brief Set the a score to be displayed on the screen
	 *
     * @param score The score to display on the screen
     */
	void SetScore( int score );

	void SetGameMode( GameMode mode) { m_GameMode = mode; }

private: 
	int		 m_Score;
	GameMode m_GameMode;
	CSprite* m_pBackground;
	CLabel*  m_pScoreText;
	
	/**
     * @fn     bool ResultsScene::InitUI( float graphicsScale );
     *
     * @brief  Initialise the UI elements of the results scene
     *
	 * @param  graphicsScale  Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
	bool InitUI( float graphicsScale );
};

}

#endif  // __RESULTSSCENE_H__


