/*
 * (C) 2014 Search for a Star
 * 
 */

#if !defined(__GAMESCENE_H__)
#define __GAMESCENE_H__

#include "scene.h"

namespace SFAS2014
{
	
// forward declarations
class GridItem;
class Grid;

/**
 * @enum GameMode
 *
 * @brief Holds identifiers for available game modes
 *
 */
enum GameMode
{
	keClassic,	// The "classic" game mode, where the player has to swap shapes to form groups of three or more identical
				// shapes to gain points
	keMemory,	// An alternate mode where the player has to find pairs of identical shapes as in the card game "Memory"
	keNumberOfGameModes
};

/**
 * @class GameScene
 *
 * @brief Displays and updates the game
 *
 */
class GameScene : public Scene
{

public:
    GameScene( const char* name );
    ~GameScene();

    /**
     * @fn     bool TitleScene::Init( float graphicsScale );
     *
     * @brief  Initialise the game scene
     *
	 * @param  graphicsScale  Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
    bool Init( float graphicsScale );

    /**
     * @fn    void TitleScene::Update(float deltaTime = 0.0f, float alphaMul = 1.0f);
     *
     * @brief Update the game scene.
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
     * @fn     void GameScene::AddScore( int score );
     *
     * @brief Add a score to the total score for the current game.
	 *
     * @param score   The score to add.
     */
	void AddScore( int score );

	/**
     * @fn     void GameScene::NextTurn( void );
     *
     * @brief Get ready for the next turn of the player
	 *
     */
	void NextTurn( void );
	
	/**
     * @fn     void GameScene::SetGameMode( GameMode mode );
     *
     * @brief Set the game mode for the game
	 *
	 * @param mode The game mode to set
     */
	void SetGameMode( GameMode mode );
	
	/**
     * @fn     void GameScene::Pause( void );
     *
     * @brief Pause the game
	 *
     */
	void Pause( void );

	/**
     * @fn     void GameScene::Resume( void );
     *
     * @brief Resume the game from paused state
	 *
     */
	void Resume( void );

private: 

	/**
	 * @enum GameState
	 *
	 * @brief Describes the current state of the game.
	 *
	 */
	enum GameState
	{
		keWaitingForStart,			// Wait for the actual start of the game, initial state
		keWaitingForFirstInput,		// Wait for the user to select a grid field
		keWaitingForSecondInput,	// Wait for the user to select a second field
		keProcessingMove,			// Given the two selected fields, execute some actions
		keReversingMove,			// If the user's selection of fields was invalid, reverse the actions taken
		keProcessingTurn,			// If the user's selection was valid, execute some actions
		keWaitingForGridRefill,		// Wait for the grid to restore 
		keGameFinished				// Game is finished
	};

	static int   m_sGridXOffset;		// Draw the grid this far from the left border of the screen
	static int   m_sGridYOffset;		// Draw the grid this far from the top border of the screen
	static int   m_sGridWidth;			// Number of columns of the grid that will be created by the game
	static int   m_sGridHeight;         // Number of rows of the grid that will be created by the game
	static int   m_sGridFieldWidth;		// Set this to the width of the images used for the grid fields
	static int   m_sGridFieldHeight;    // Set this to the height of the images used for the grid fields
	static float m_sTimeLimitInSeconds; // A game runs this many seconds

	Grid*  m_pGrid;				// The grid that the game will be played on

	CSprite* m_pBackground;		// The image used as background for the scene
	CLabel*  m_pScoreText;		// The text that will display the player's score
	CLabel*  m_pTimeText;		// The text that will display the remaining time
	CSprite* m_pButtonPause;    // The button to activate to get to the pause menu

	GameState m_GameState;		// The current game state
	GameMode  m_GameMode;		// The current game mode
	float     m_Time;			// The time left for this round of the game
	int       m_Score;			// The current score of the player
	bool      m_Paused;         // Tells whether the game is currently paused or running

	int		  m_firstHit[2];    // The x/y grid coordinates of the grid item that the player touched first
	int		  m_secondHit[2];   // The x/y grid coordinates of a second grid item touched by the player

    /**
     * @fn     void GameScene::UpdateMemoryGame(float deltaTime = 0.0f, float alphaMul = 1.0f);
     *
     * @brief Update stuff specific to the memory game mode
     *
     * @param deltaTime   Number of seconds that has passed since Update() was last called.
     * @param alphaMul    The alpha value being passed down to this node. Default value is 1.
     */
    void UpdateMemoryGame(float deltaTime = 0.0f, float alphaMul = 1.0f);

	   /**
     * @fn     void GameScene::UpdateClassicGame(float deltaTime = 0.0f, float alphaMul = 1.0f);
     *
     * @brief Update stuff specific to the classic game mode
     *
     * @param deltaTime   Number of seconds that has passed since Update() was last called.
     * @param alphaMul    The alpha value being passed down to this node. Default value is 1.
     */
    void UpdateClassicGame(float deltaTime = 0.0f, float alphaMul = 1.0f);

	 /**
     * @fn     void GameScene::UpdateHUD();
     *
     * @brief Update the HUD of the game
     *
	 */
	void UpdateHUD( void );

	/**
     * @fn     bool TitleScene::InitUI( float graphicsScale );
     *
     * @brief  Initialise the UI elements of the game scene
     *
	 * @param  graphicsScale Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
	bool InitUI( float graphicsScale );

		/**
     * @fn     void GameScene::EndGame( void );
     *
     * @brief Called when the time for the game has expired and the last turn of the player has been processed
	 *
     */
	void EndGame( void );

	/**
     * @fn    static void GameScene::on_move_processed( Timer* pTimer, void* context );
     *
     * @brief Callback function to be invoked after a user move has been processed (planets swapped or flipped)
	 *
	 * @param timer   The timer calling the callback.
     * @param context   The instance of the GameScene class associated to this callback.
     */
	static void on_move_processed( Timer* pTimer, void* context );

	/**
     * @fn    static void GameScene::on_move_reversed( Timer* pTimer, void* context );
     *
     * @brief Callback function to be invoked after a user move has been reversed as it wasn't valid (planets swapped or flipped back)
	 *
	 * @param timer   The timer calling the callback.
     * @param context   The instance of the GameScene class associated to this callback.
     */
	static void on_move_reversed( Timer* pTimer, void* context );

	/**
     * @fn    static void GameScene::on_turn_processed( Timer* pTimer, void* context );
     *
     * @brief Callback function to be invoked after planets have been destroyed
	 *
	 * @param timer   The timer calling the callback.
     * @param context   The instance of the GameScene class associated to this callback.
     */
	static void on_turn_processed( Timer* pTimer, void* context );
};
}

#endif  // __GAMESCENE_H__


