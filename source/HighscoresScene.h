/*
 * (C) 2014 Search for a Star
 * 
 */

#if !defined(__HIGHSCORESSCENE_H__)
#define __HIGHSCORESSCENE_H__

#include "scene.h"
#include "GameScene.h"

namespace SFAS2014
{

/**
 * @class HighscoresScene
 *
 * @brief Displays a list of highscores for the game mode played
 *
 */
class HighscoresScene : public Scene
{

public:
    HighscoresScene( const char* name );
    ~HighscoresScene();

    /**
     * @fn     bool HighscoresScene::Init( float graphicsScale );
     *
     * @brief  Initialise the highscores scene
     *
	 * @param  graphicsScale  Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
    bool Init( float graphicsScale );

    /**
     * @fn    void HighscoresScene::Update(float deltaTime = 0.0f, float alphaMul = 1.0f);
     *
     * @brief Update the highscores scene.
	 *
     * @param deltaTime Number of seconds that have passed since Update() was called the last time. Default value is 0.0f.
     * @param alphaMul  Determines the alpha value of the scene. Default value is 1.
     */
	void Update(float deltaTime = 0.0f, float alphaMul = 1.0f);

	/**
     * @fn    void HighscoresScene::Reset();
     *
     * @brief Prepare the scene for activation by resetting it.
     */
    void Reset();

	void SetGameMode( GameMode mode ) {m_GameMode = mode;}
	void SetScore(int score)          {m_Score = score;}

private: 

	static int         m_sNumberOfScores;						   // The number of entries in a highscore list
	static const char* m_sHighscoreFilenames[keNumberOfGameModes]; // The filenames of the files containing the highscore lists for the different game modes

	/**
	 * @struct HighscoreEntry
	 *
	 * @brief  Bundles a highscore entry
	 */
	struct HighscoreEntry
	{
		HighscoreEntry(): m_Position( 0 ), m_Score( 0 )
		{
			memset( m_Name, 0, 11 );
		}

		int m_Position;  // The position of the entry within the highscore list (from 1 to m_sNumberOfScores)
		char m_Name[11]; // The name of the player associated to the entry (10 characters plus null terminator)
		int m_Score;     // The score associated to the entry

		/**
		 * @fn    void HighscoreEntry::Serialise();
		 *
		 * @brief Serialise the highscore entry to/from the current serialisation file
		 */
		void Serialise()
		{
			IwSerialiseInt32( m_Position );
			IwSerialiseChar( m_Name[0], 11 );
			IwSerialiseInt32( m_Score );
		}
	};

	CSprite* m_pBackground;		 // The image used as background for this scene
	CLabel*  m_pTextPositions;   // The screen text containing the positions of the highscore entries
	CLabel*  m_pTextNames;		 // The screen text containing the names of the highscore entries	
	CLabel*  m_pTextScores;		 // The screen text containing the scores of the highscore entries
	CSprite* m_pButtonContinue;  // The button to activate to get to the title screen
	int		 m_Score;            // The score achieved by the player in the current game
	GameMode m_GameMode;         // The game mode that the game finished last was played in
	bool     m_Checked;          // Used to determine the moment when the scene becomes active

	HighscoreEntry m_Entries[keNumberOfGameModes][8]; // Contains the highscore entries for the different game modes

	/**
     * @fn    void HighscoresScene::LoadHighscores( GameMode mode );
     *
     * @brief Loads the highscores for a given game mode from the correspoding file to the entries member array
	 *
     * @param mode The game mode for which to load the highscores
     */
	void LoadHighscores( GameMode mode );

	/**
     * @fn    void HighscoresScene::SaveHighscores( GameMode mode );
     *
     * @brief Save the highscores for a given game mode from the entries member array to the correspoding file 
	 *
     * @param mode The game mode for which to save the highscores
     */
	void SaveHighscores( GameMode mode );

	/**
     * @fn    void HighscoresScene::AddNewEntry( GameMode mode, int position, const char* name, int score );
     *
     * @brief Adds a new highscore entry into the list and saves it to the associated highscore file.
	 *
     * @param mode      The game mode for which to add a new highscore entry
     * @param position  The position at which to place the new entry
	 * @param name      The name of the player that achieved the new highscore
	 * @param score     The score achieved by the player
     */
	void AddNewEntry( GameMode mode, int position, const char* name, int score );

	/**
     * @fn    void HighscoresScene::GetScorePosition( GameMode mode );
     *
     * @brief Returns the position of a score within the highscore list-
	 *
     * @param mode  The game mode for which to save the highscores
	 * @param score The score to check the highscore list against
	 *
	 * @return The position of the highscore entry (between 1 and m_sNumberOfScores). Negative if score is too small to be featured in highscore list
     */
	int GetScorePosition( GameMode mode, int score );

	/**
     * @fn    void HighscoresScene::GetPlayerName( void );
     *
     * @brief  Get the name of the player through a virtual keyboard
	 *
     * @return A string containing the name of the player or null when the player didn't enter a name
     */
	const char* GetPlayerName( void );

	/**
     * @fn    void HighscoresScene::GetPositionsString( GameMode mode, char* outText );
     *
     * @brief Builds a string containing the positions for the highscore text
	 *
     * @param mode     The game mode for which to build the positions string 
     * @param outText  Store the created string in this char array
     */
	void GetPositionsString( GameMode mode, char* outText );

	/**
     * @fn    void HighscoresScene::GetNamesString( GameMode mode, char* outText );
     *
     * @brief Builds a string containing the player names for the highscore text
	 *
     * @param mode     The game mode for which to build the names string 
     * @param outText  Store the created string in this char array
     */
	void GetNamesString( GameMode mode, char* outText );

	/**
     * @fn    void HighscoresScene::GetScoresString( GameMode mode, char* outText );
     *
     * @brief Builds a string containing the scores for the highscore text
	 *
     * @param mode     The game mode for which to build the scores string 
     * @param outText  Store the created string in this char array
     */
	void GetScoresString( GameMode mode, char* outText );

	/**
     * @fn    void HighscoresScene::UpdateUI( void );
     *
     * @brief Update the user interface of the highscore scene to show the current list entries
     */
	void UpdateUI( void );

	/**
     * @fn     bool TitleScene::InitUI( float graphicsScale );
     *
     * @brief  Initialise the UI elements of the title scene
     *
	 * @param  graphicsScale Use this value to scale UI elements
	 *
     * @return True if the initialisation succeeded, false if an error occurred.
     */
	bool InitUI( float graphicsScale );
};

}

#endif  // __HIGHSCORESSCENE_H__


