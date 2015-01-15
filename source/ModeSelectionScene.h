

#if !defined(__MODESELECTIONSCENE_H__)
#define __MODESELECTIONSCENE_H__

#include "scene.h"

namespace SFAS2014
{

/**
 * @class ModeSelectionScene
 *
 * @brief Displays the mode selection screen
 *
 */
class ModeSelectionScene : public Scene
{

public:
    ModeSelectionScene( const char* name );
    ~ModeSelectionScene();

    /**
     * @fn     bool ModeSelectionScene::Init( float graphicsScale );
     *
     * @brief  Initialise the mode selection scene
     *
	 * @param  graphicsScale  Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
    bool Init( float graphicsScale );

    /**
     * @fn    void ModeSelectionScene::Update(float deltaTime = 0.0f, float alphaMul = 1.0f);
     *
     * @brief Update the mode selection scene.
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

	CSprite* m_pBackground;			// The background image for the scene
	CSprite* m_pButtonClassicMode;  // The button to activate to get to the Classic game mode
	CSprite* m_pTextClassicMode;    // An Image holding text describing the Classic game mode
	CSprite* m_pButtonMemoryMode;   // The button to activate to get to the Memory game mode
	CSprite* m_pTextMemoryMode;     // An Image holding text describing the Memory game mode
	CSprite* m_pButtonCredits;      // The button to activate to display the credits for the game

	/**
     * @fn     bool ModeSelectionScene::InitUI( float graphicsScale );
     *
     * @brief  Initialise the UI elements of the mode selection scene
     *
	 * @param  graphicsScale  Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
	bool InitUI( float graphicsScale );

};

}

#endif  // __MODESELECTIONSCENE_H__


