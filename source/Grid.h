#if !defined(__GRID_H__)
#define __GRID_H__

#include "IwTween.h"
#include "GameScene.h"
#include <list>

using namespace IwTween;

namespace SFAS2014
{
	// forward declaration
	class GridItem;

/**
 * @class Grid
 *
 * @brief Manages a grid of GridItems
 *
 */
class Grid
{
public:
	Grid( void );
	~Grid( void );

	/**
     * @fn    bool Grid::Init( GameScene* pGame, int offsetX, int offsetY, int numberOfRows, int numberOfColumns, int fieldWidth, int fieldHeight, float graphicsScale );
     *
     * @brief Initialise the grid.
     *
	 * @param offsetX			Determines where the grid is drawn in relation to the left and right border the screen.
     * @param offsetY			Determines where the grid is drawn in relation to the bottom border of the screen.
     * @param parentNode		The scene that the grid items will be added to as children.
     * @param numberOfRows		The number of rows in the grid.
	 * @param numberOfColumns   The number of columns in the grid.
     * @param fieldWidth		The width of a field of the grid.
	 * @param fieldHeight		The height of a field of the grid.
	 * @param graphicsScale		Use this to scale the grid
	 *
	 * @return  true if the initialisation succeeds, false otherwise
     */
	bool Init( GameScene* pGame, int offsetX, int offsetY, int numberOfRows, int numberOfColumns, int fieldWidth, int fieldHeight, float graphicsScale );

	/**
     * @fn    void Grid::ScreenCoordinatesToGridField( int screenX, int screenY, int& outGridX, int& outGridY );
     *
     * @brief Returns the position of the grid field for a given pair of screen coordinates
     *
     * @param screenX	The x coordinate of the screen position
     * @param screenY	The y coordinate of the screen position
	 * @param outGridX  The x coordinate of the grid field associated to the given screen coordinates, -1 if not on the grid
     * @param outGridY	The y coordinate of the grid field associated to the given screen coordinates, -1 if not on the grid
     */
	void ScreenCoordinatesToGridField( int screenX, int screenY, int& outGridX, int& outGridY );

	/**
     * @fn    void Grid::HighlightField( int x, int y );
     *
     * @brief	Highlights the grid field at the given position.
     *
     * @param x	The x coordinate of the grid position to highlight
     * @param y	The y coordinate of the grid position to highlight
     */
	void HighlightField( int x, int y );

	/**
     * @fn    void Grid::ResetField( int x, int y );
     *
     * @brief	Removes the highlight from the grid field at the given position.
     *
     * @param x	The x coordinate of the grid position to reset
     * @param y	The y coordinate of the grid position to reset
     */
	void ResetField( int x, int y );

	/**
     * @fn    void Grid::SwapPlanets( int x1, int y1, int x2, int y2 );
     *
     * @brief	Swaps the grid positions of the two shapes at the given grid positions.
     *
     * @param x1	The x coordinate of the grid position of the first planet
     * @param y1	The y coordinate of the grid position of the first planet
	 * @param x2	The x coordinate of the grid position of the second planet
     * @param y2	The y coordinate of the grid position of the second planet
	 * @param onCompleteCallback	The callback function to be called when the swap has finished
	 * @param context	The instance to be called back when swapping is finished
     */
	 void SwapPlanets( int x1, int y1, int x2, int y2 );

	 /**
     * @fn    void Grid::Update( float deltaTime );
     *
     * @brief	Updates the grid.
     *
     * @param deltaTime	The time in seconds passed since the last frame
     */
	 void Update( float deltaTime );

	 /**
     * @fn     bool Grid::RemoveGroups( void );
     *
     * @brief	Removes groups of three adjacent identical planet forming a row or column
     *
     * @return True if at least one group was removed, false otherwise
     */
	 bool RemoveGroups( void );

	 /**
     * @fn     void Grid::RefillGrid( void );
     *
     * @brief	Refills the grid after planets have been destroyed
     *
     */
	 void RefillGrid( void );

	 /**
     * @fn     void Grid::FlipItem( int x, int y );
     *
     * @brief	Flips a grid item to show the reverse side (frontside -> backside, backside -> frontside)
     *
	 * @param x	The grid x coordinate of the item to flip
	 * @param y	The grid y coordinate of the item to flip
     */
	 void FlipItem( int x, int y );

	 /**
     * @fn    bool Grid::CheckPair( int x1, int y1, int x2, int y2 );
     *
     * @brief	Check if the pair matches and remove them from the grid if that's the case
     *
     * @param x1	The x coordinate of the grid position of the first shape
     * @param y1	The y coordinate of the grid position of the first shape
	 * @param x2	The x coordinate of the grid position of the second shape
     * @param y2	The y coordinate of the grid position of the second shape
	 *
	 * @return      True if the pair matches (identical planet), false otherwise
     */
	 bool CheckPair( int x1, int y1, int x2, int y2 );

	 /**
     * @fn    void Grid::Reset( );
     *
     * @brief	Resets the grid
     *
	 */
	 void Reset();

	 /**
     * @fn    void Grid::IsFieldValid( int x, int y);
     *
     * @brief	Checks if a grid item is valid
     *
     * @param x	The x coordinate of the grid position to check
     * @param y	The y coordinate of the grid position to check
	 *
	 * @return True if the field is valid, false otherwise (currently no item at this coordinate)
     */
	 bool IsFieldValid( int x, int y );

	void SetGameMode( GameMode mode ){ m_GameMode = mode; }
private:

	static float m_sGridFieldScale;       // This scale is originally applied to grid items
	static int   m_sPlanetDestroyedScore; // The amount of points awarded for each destroyed planet on the grid

	GridItem** m_pGridItems;        // Pointers to the grid items
	GameMode   m_GameMode;			// The current game mode	
	GameScene* m_pGame;             // A pointer to the game scene to which this grid belongs
	int		   m_OffsetX;           // Grid offset in pixels from the left border of the screen
	int		   m_OffsetY;		    // Grid offset in pixels from the top border of the screen
	int		   m_NumberOfRows;      // The grid has this many rows of items
	int		   m_NumberOfColumns;   // The grid has this many columns of items
	int		   m_FieldWidth;        // The width of a grid field in pixels
	int		   m_FieldHeight;       // The height of a grid field in pixels

	CTweenManager m_Tweener;       // TweenManager for animations
	TimerManager  m_TimerManager;  // TimerManager for timings

	std::list<GridItem*> m_RemovedItems; // Temporarily stores grid items that have been removed from the grid (will be reused, inserted again)

	// private functor to find a specific grid item within an stl container
	class FindGridItem
	{
	public:
		FindGridItem( GridItem* pItemToFind) : m_pItemToFind( pItemToFind ){}
		bool operator()( GridItem* pItemToCheck ) const
		{
			return m_pItemToFind == pItemToCheck;
		}
	private:
		GridItem* m_pItemToFind;
	};

	/**
     * @fn    static void Grid::on_insertion_finished( Timer* pTimer, void* context );
     *
     * @brief Callback function to be invoked after new planets have been inserted
	 *
	 * @param timer   The timer calling the callback.
     * @param context   The instance of the Grid class associated to this callback.
     */
	static void on_insertion_finished( Timer* pTimer, void* context );
	
	/**
     * @fn    static void Grid::on_destruction_finished( Timer* pTimer, void* context );
     *
     * @brief Callback function to be invoked after planets have been moved down on the grid into empty spaces
	 *
	 * @param timer   The timer calling the callback.
     * @param context   The instance of the Grid class associated to this callback.
     */
	static void on_movement_finished( Timer* pTimer, void* context );

	/**
     * @fn    static void Grid::on_destruction_finished( Timer* pTimer, void* context );
     *
     * @brief Callback function to be invoked after Planets have been destroyed
	 *
	 * @param timer   The timer calling the callback.
     * @param context   The instance of the Grid class associated to this callback.
     */
	static void on_destruction_finished( Timer* pTimer, void* context );

	 /**
     * @fn     void Grid::DestroyItem( void );
     *
     * @brief	Removes an item from the grid
     *
	 * @param pItem	A pointer to the item to remove
     */
	 void DestroyItem( GridItem* pItem );

	/**
     * @fn     void Grid::RefillTopRow( void );
     *
     * @brief	Insert new items at the top of the grid to fill empty spaces
     *
     */
	 void RefillTopRow( void );

	 /**
     * @fn     void Grid::MoveItems( void );
     *
     * @brief	Move grid items in order to fill empty spaces.
     *
     */
	 void MoveItems( void );

	 /**
     * @fn     void Grid::DistributePlanets( void );
     *
     * @brief	Assign random planets to the grid items 
     *
     */
	 void DistributePlanets(void);

	 /**
     * @fn     void Grid::ResolveGroups( void );
     *
     * @brief	Makes sure there are no groups of three identical planets
     *
     */
	 void ResolveGroups( void );

	/**
     * @fn    void Grid::MemoryRefill( );
     *
     * @brief	Sets up a new grid with pairs of planets after all items have been cleared
     *
	 */
	 void MemoryRefill();

	 /**
     * @fn    void Grid::InitMemoryGrid( );
     *
     * @brief	Initial setup for the grid for a round of memory
     *
	 */
	 void InitMemoryGrid();

	 /**
     * @fn    void Grid::ClassicRefill( );
     *
     * @brief	Refills the grid after groups have been removed in classic play mode
     *
	 */
	 void ClassicRefill();

	 /**
     * @fn    void Grid::RestoreGridItems();
     *
     * @brief	Bring back previously removed grid items
     */
	 void RestoreGridItems();
};

}

#endif //__GRID_H__