#include "Grid.h"

#include "IwGx.h"
#include "AudioHandler.h"
#include "TimeConstants.h"
#include "GridItem.h"
#include <algorithm>

using namespace SFAS2014;


//
//
// Grid class
//
//

int Grid::m_sPlanetDestroyedScore = 100;
float Grid::m_sGridFieldScale     = 0.375f;

Grid::Grid( void ) : m_pGridItems( 0 ),
					 m_GameMode( keClassic ),
					 m_pGame( 0 ),
				     m_OffsetX( 0 ),
					 m_OffsetY( 0 ),
					 m_NumberOfRows( 0 ),
					 m_NumberOfColumns( 0 ),
					 m_FieldWidth( 0 ),
					 m_FieldHeight( 0 )
{
}

Grid::~Grid( void )
{
	// Delete all allocated grid items
	if( m_pGridItems != 0 )
	{
		delete[] m_pGridItems;
	}
}

bool Grid::Init( GameScene* pGame, int offsetX, int offsetY, int numberOfRows, int numberOfColumns, int fieldWidth, int fieldHeight, float graphicsScale )
{
	// Set member variables
	m_pGame				= pGame;
	m_OffsetX			= static_cast<int>( static_cast<float>( offsetX ) * graphicsScale );
	m_OffsetY			= static_cast<int>( static_cast<float>( offsetY ) * graphicsScale );
	m_NumberOfRows		= numberOfRows;
	m_NumberOfColumns	= numberOfColumns;
	m_FieldWidth		= static_cast<int>( m_sGridFieldScale * static_cast<float>( fieldWidth ) * graphicsScale );
	m_FieldHeight		= static_cast<int>( m_sGridFieldScale * static_cast<float>( fieldHeight ) * graphicsScale );

	// Prepare the array for the grid items
	m_pGridItems = new GridItem* [numberOfRows * numberOfColumns];

    // Calculate new grid scale
	float fieldScale = static_cast<float>( m_FieldWidth ) / static_cast<float>( fieldWidth );

	// Create the grid items
	float x = 0;
	float y = 0;

	for( int row = 0; row < m_NumberOfRows; ++row )
	{
		for( int column = 0; column < m_NumberOfColumns; ++column )
		{
			int planet = IwRand() % GridItem::Planet::keNumberOfPlanets;

			m_pGridItems[row * m_NumberOfColumns + column] = new GridItem();

			if( 0 == m_pGridItems[row * m_NumberOfColumns + column] )
			{
				return false;
			}

			x = static_cast<float>( m_OffsetX + column * m_FieldWidth );
			y = static_cast<float>( m_OffsetY + row * m_FieldHeight );

			if( !m_pGridItems[row * m_NumberOfColumns + column] -> Init( x, y, fieldScale, GridItem::Planet(planet) ) )
			{
				return false;
			}

			pGame -> AddChild( m_pGridItems[row * m_NumberOfColumns + column] );
		}
	}

	return true;
}

void Grid::ScreenCoordinatesToGridField( int screenX, int screenY, int& outGridX, int& outGridY )
{
	// If the screen coordinates don't match to any grid field, return -1 for both grid coordinates
	if( screenX - m_OffsetX < 0 || screenY - m_OffsetY < 0 || screenY >= m_OffsetY + m_FieldHeight * m_NumberOfRows || screenX >= m_OffsetX + m_FieldWidth * m_NumberOfColumns )
	{
		outGridX = -1;
		outGridY = -1;
		return;
	}

	// Determine coordinates of the grid field
	outGridX = (screenX - m_OffsetX) / m_FieldWidth;
    outGridY = (screenY - m_OffsetY) / m_FieldHeight;
}

void Grid::HighlightField( int x, int y )
{
	m_pGridItems[y * m_NumberOfColumns + x] -> Highlight();
}

void Grid::ResetField( int x, int y )
{
	m_pGridItems[y * m_NumberOfColumns + x] -> Reset();
}

void Grid::SwapPlanets( int x1, int y1, int x2, int y2 )
{
	// Horizontal swap
	if( x1 == x2 )
	{
		m_Tweener.Tween( kfSwapGridItemsAnimationLength,
                        FLOAT, &m_pGridItems[y1 * m_NumberOfColumns + x1] -> m_Y, m_pGridItems[y2 * m_NumberOfColumns + x2] -> m_Y,
                        FLOAT, &m_pGridItems[y2 * m_NumberOfColumns + x2] -> m_Y, m_pGridItems[y1 * m_NumberOfColumns + x1] -> m_Y,
                        EASING, Ease::sineInOut,
                        END );
	}

	// Vertical swap
	if( y1 == y2 )
	{
		m_Tweener.Tween( kfSwapGridItemsAnimationLength,
                        FLOAT, &m_pGridItems[y1 * m_NumberOfColumns + x1] -> m_X, m_pGridItems[y2 * m_NumberOfColumns + x2] -> m_X,
                        FLOAT, &m_pGridItems[y2 * m_NumberOfColumns + x2] -> m_X, m_pGridItems[y1 * m_NumberOfColumns + x1] -> m_X,
                        EASING, Ease::sineInOut,
                        END );
	}

	// Swap the grid items
    GridItem* temp = m_pGridItems[y1 * m_NumberOfColumns + x1];
	m_pGridItems[y1 * m_NumberOfColumns + x1] = m_pGridItems[y2 * m_NumberOfColumns + x2];
    m_pGridItems[y2 * m_NumberOfColumns + x2] = temp;
}

void Grid::Update( float deltaTime )
{
	m_Tweener.Update( deltaTime );
	m_TimerManager.Update(deltaTime);
}

bool Grid::RemoveGroups( void )
{
	GridItem::Planet currentPlanet = GridItem::Planet( 0 );
	int startIndex				   = 0;					// Begin of the row of identical shapes
	int numOfPlanets			   = 0;				    // Number of identical shapes in a row

	// Find rows of three or more identical planets
	for(int row = 0; row < m_NumberOfRows; ++row)
	{
		// initialise these values for every new row
		currentPlanet    = m_pGridItems[row * m_NumberOfColumns] -> GetPlanet();
		startIndex       = 0;
		numOfPlanets     = 0;

		for(int column = 0; column < m_NumberOfColumns; ++column)
		{
			if( m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet() == currentPlanet )
			{
				// count identical planets placed in a row
				++numOfPlanets;
			}else
			{
				// The row of identical planets was broken by a different shape -> check if there were more than
				// two identical planets in a row
				if( numOfPlanets >= 3 )
				{
					// group detected
					for( int i = startIndex; i < startIndex + numOfPlanets; ++i )
					{
						// Remember the item to destroy it later
						m_RemovedItems.push_back( m_pGridItems[row * m_NumberOfColumns + i] );
					}
				}

				// Set the variables according to the new planet
				startIndex = column;
				numOfPlanets = 1;
				currentPlanet = m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet();
			}
		}

		// Row finished, check if there is a current group not yet added
		// (that's the case when the group runs till the end of the row)
		if( numOfPlanets >= 3 )
		{
			// group detected
			for( int i = startIndex; i < startIndex + numOfPlanets; ++i )
			{
				m_RemovedItems.push_back( m_pGridItems[row * m_NumberOfColumns + i] );
			}
		}
	}

	// Now repeat the procedure but check for the columns for groups of identical planets

	// Find columns of three or more identical planets
	for(int column = 0; column < m_NumberOfColumns; ++column)
	{
		currentPlanet    = m_pGridItems[column] -> GetPlanet();
		startIndex       = 0;
		numOfPlanets     = 0;

		for(int row = 0; row < m_NumberOfRows; ++row)
		{
			if( m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet() == currentPlanet )
			{
				++numOfPlanets;
			}else
			{
				if( numOfPlanets >= 3 )
				{
					// group detected
					for( int i = startIndex; i < startIndex + numOfPlanets; ++ i )
					{
						// Only add the item if it is not yet contained
						if( std::find_if( m_RemovedItems.begin(), m_RemovedItems.end(), FindGridItem( m_pGridItems[i * m_NumberOfColumns + column] ) ) == m_RemovedItems.end() )
						{
							m_RemovedItems.push_back( m_pGridItems[i * m_NumberOfColumns + column] );
						}
					}
				}

				// Set the variables according to the new planets
				startIndex = row;
				numOfPlanets = 1;
				currentPlanet = m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet();
			}
		}

		// Column finished, check if there is a current group not yet added
		// (that's the case when the group runs till the end of the column)
		if( numOfPlanets >= 3 )
		{
			// group detected
			for( int i = startIndex; i < startIndex + numOfPlanets; ++ i )
			{
				// Only add the item if it is not yet contained
				if( std::find_if( m_RemovedItems.begin(), m_RemovedItems.end(), FindGridItem( m_pGridItems[i * m_NumberOfColumns + column] ) ) == m_RemovedItems.end() )
				{
					m_RemovedItems.push_back( m_pGridItems[i * m_NumberOfColumns + column] );
				}
			}
		}
	}

	// Remove all the grid items that are in groups
	for( std::list<GridItem*>::iterator it = m_RemovedItems.begin(); it != m_RemovedItems.end(); ++it )
	{
		DestroyItem(*it);
	}

	if(m_RemovedItems.size() > 0)
	{
		g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keSuccess);
	}

	// Return true if items have been removed, false otherwise
	return (m_RemovedItems.size() > 0);
}

void Grid::DestroyItem( GridItem* pItem )
{
	// Fade the item out
	m_Tweener.Tween( kfDestroyGridItemAnimationLength,
                    FLOAT, &pItem->m_Alpha, 0.0f,
                    EASING, Ease::sineIn,
                    END );

	// Update the score
	m_pGame -> AddScore( m_sPlanetDestroyedScore );

    // Remove the item from the grid

	int gridX = 0;
	int gridY = 0;
	ScreenCoordinatesToGridField( static_cast<int>( pItem -> m_X ), static_cast<int>( pItem -> m_Y ), gridX, gridY );
	m_pGridItems[gridY * m_NumberOfColumns + gridX] = 0;
}

void Grid::RefillGrid( void )
{
switch( m_GameMode )
{
case keClassic:
	ClassicRefill();
	break;
case keMemory:
	// Only refill when the whole grid has been cleared
	if( m_RemovedItems.size() != m_NumberOfRows * m_NumberOfColumns )
	{
		m_pGame -> NextTurn();
	}else
	{
		// Whole grid cleared -> refill
		MemoryRefill();
	}
	break;
}
}

void Grid::ClassicRefill()
{
	// This is the start of a chain of functions that each execute a step towards
	// the regeneration of the grid (there may be several cycles of these functions
	// until the grid is eventually restored to a valid state)
	RefillTopRow();
}

void Grid::RefillTopRow( void )
{
	bool doInsertItems = false;

	// Check the top row for empty spaces and fill in new planets into empty spaces
	for(int column = 0; column < m_NumberOfColumns; ++column)
	{
		if( 0 == m_pGridItems[column] )
		{
			// Reuse previously removed items
			GridItem* newItem = *(m_RemovedItems.begin());

			// Place item at the top
			newItem -> m_X = static_cast<float>( m_OffsetX + column * m_FieldWidth );
			newItem -> m_Y = static_cast<float>( m_OffsetY );
	
			// Assign a new random shape to the grid item
			newItem -> SetPlanet( GridItem::Planet( IwRand() % GridItem::Planet::keNumberOfPlanets ) );

			m_pGridItems[column] = newItem;

			// fade in the new item
			m_Tweener.Tween( kfClassicModeInsertItemsAnimationLength,
                            FLOAT, &m_pGridItems[column] -> m_Alpha, 1.0f,
                            EASING, Ease::sineOut,
                            END );

			// Remove the item from the list
			m_RemovedItems.remove( newItem );

			// Items are being inserted
			doInsertItems = true;
		}
	}

	// If new items have to be inserted at the top, wait for the insertion to complete,
	// otherwise move on to the next refill step.
	if( doInsertItems )
	{
		m_TimerManager.Add( "InsertItems", kfClassicModeInsertItemsTimer, Grid::on_insertion_finished, this, 1 );
	}else
	{
		// Nothing to insert yet -> move items down into empty spaces where possible
		MoveItems();
	}
}

void Grid::MoveItems( void )
{
	bool itemsToMove = false;

	// Check each grid item if it there is an empty space below it and
	// move items down if there is en empty space below them.
	// (No need to check bottom row; moving from second lowest row to the top)

	for( int row = m_NumberOfRows - 2; row >= 0; --row )
	{
		for( int column = 0; column < m_NumberOfColumns; ++column )
		{
			// Check if there is an empty space below this item
			if( 0 == m_pGridItems[(row + 1) * m_NumberOfColumns + column] &&  m_pGridItems[row * m_NumberOfColumns + column] != 0 )
			{
				GridItem* pItemToMove = m_pGridItems[row * m_NumberOfColumns + column];

				// Move the item to the empty space below
				m_Tweener.Tween( kfClassicModeMoveItemsAnimationLength,
						        FLOAT, &pItemToMove -> m_Y, static_cast<float>(m_OffsetY + (row + 1) * m_FieldHeight),
								EASING, Ease::sineInOut,
								END );

				// Update the grid according to the movement
				m_pGridItems[(row + 1) * m_NumberOfColumns + column] = pItemToMove;

				m_pGridItems[row * m_NumberOfColumns + column] = 0; // by setting the moving item to 0, the items above will be moved as well
			 
				itemsToMove = true;
			}
		}
	}

	// If there are items that have to be moved, make sure to wait for a timer to finish,
	// otherwise proceed to the next step
	if( itemsToMove )
	{
		m_TimerManager.Add( "MoveItems", kfClassicModeMoveItemsTimer, Grid::on_movement_finished, this, 1 );
	}else
	{
		// Refill the top row one last time for this cycle
		RefillTopRow();
	}

}

void Grid::on_insertion_finished( Timer* pTimer, void* context )
{
	Grid* pGrid = static_cast<Grid*>(context);

	if( keClassic == pGrid->m_GameMode )
	{
		if( pGrid -> m_RemovedItems.size() > 0 )
		{
			// If there are still items to insert, move items to free spaces at the top for new insertions
			pGrid -> MoveItems();
		}else
		{
			
			// If all items have been inserted, check for groups to be deleted
			if( pGrid -> RemoveGroups() )
			{
				// New planets to delete (wait for their destruction and start inserting/moving planets again)
				pGrid -> m_TimerManager.Add( "DestroyItems", kfDestroyGroupsTimer, Grid::on_destruction_finished, pGrid, 1 );
			}else
			{
				// ready for next move of the player
				pGrid -> m_pGame -> NextTurn();
			}
		}
	}else
	{
		// ready for next move of the player
		pGrid -> m_pGame -> NextTurn();
	}
}

void Grid::on_movement_finished( Timer* pTimer, void* context )
{
	Grid* pGrid = static_cast<Grid*>(context);
	// Restart refill cycle
	pGrid->RefillTopRow();
}

void Grid::on_destruction_finished( Timer* pTimer, void* context )
{
	Grid* pGrid = static_cast<Grid*>(context);
	// Restart refill cycle
	pGrid->RefillTopRow();
}

void Grid::DistributePlanets()
{
	// Set a random planet for every grid item and show the fronface
	for( int i = 0; i < m_NumberOfRows * m_NumberOfColumns; ++i )
	{
		m_pGridItems[i] -> SetPlanet( GridItem::Planet( IwRand() % GridItem::Planet::keNumberOfPlanets ) );
		m_pGridItems[i] -> m_Alpha = 1.0f;
		m_pGridItems[i] -> GetBackside() -> m_Alpha = 0.0f;
		m_pGridItems[i] -> SetFrontSideShown( true );
	}
}

void Grid::ResolveGroups( void )
{
	// Make sure there is no group of three or more identical planets in any row or column
	// Used for the initialisation of the grid in Classic mode

	bool allGroupsRemoved = false;

	// Repeat until all groups have been removed
	while( !allGroupsRemoved )
	{
		allGroupsRemoved = true; 

		GridItem::Planet currentPlanet = GridItem::Planet( 0 );
		int startIndex				   = 0; // Begin of the row of identical shapes
		int numOfPlanets			   = 0; // Number of identical shapes in a row

		// Find rows of three or more identical planets
		for(int row = 0; row < m_NumberOfRows; ++row)
		{
			currentPlanet    = m_pGridItems[row * m_NumberOfColumns] -> GetPlanet();
			numOfPlanets     = 0;

			for(int column = 0; column < m_NumberOfColumns; ++column)
			{
				if( m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet() == currentPlanet )
				{
					++numOfPlanets;
				
					if( 3 == numOfPlanets )
					{
						// group detected -> change the planet of the current item to break the group

						// Get a new planet for the item
						int newPlanet = (m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet() + 1) % GridItem::Planet::keNumberOfPlanets;
						// Change the planet of the item
						m_pGridItems[row * m_NumberOfColumns + column] -> SetPlanet( GridItem::Planet(newPlanet) );

						currentPlanet = GridItem::Planet( newPlanet );
						numOfPlanets = 1;

						if(allGroupsRemoved)
						{
							// A group was found in this iteration, make sure there is another iteration
							// to ensure there are no new groups created by resolving the old ones
							allGroupsRemoved = false;
						}
					}
				}else
				{
					currentPlanet = m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet();
					numOfPlanets = 1;
				}
			}
		}

		// Find columns of three or more identical planets
		for(int column = 0; column < m_NumberOfColumns; ++column)
		{
			currentPlanet    = m_pGridItems[column] -> GetPlanet();
			numOfPlanets     = 0;

			for(int row = 0; row < m_NumberOfRows; ++row)
			{
				if( m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet() == currentPlanet )
				{
					++numOfPlanets;
				
					if( 3 == numOfPlanets )
					{
						// group detected -> change the planet of the current item to break the group

						// Get a new planet for the item
						int newPlanet = (m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet() + 1) % GridItem::Planet::keNumberOfPlanets;
						// Change the planet of the item
						m_pGridItems[row * m_NumberOfColumns + column] -> SetPlanet( GridItem::Planet(newPlanet) );

						currentPlanet = GridItem::Planet( newPlanet );
						numOfPlanets = 1;

						if(allGroupsRemoved)
						{
							// A group was found in this iteration, make sure there is another iteration
							// to ensure there are no new groups created by resolving the old ones
							allGroupsRemoved = false;
						}
					}
				}else
				{
					currentPlanet = m_pGridItems[row * m_NumberOfColumns + column] -> GetPlanet();
					numOfPlanets = 1;
				}
			}
		}
	}
}

void Grid::FlipItem( int x, int y )
{
	GridItem* pItem = m_pGridItems[y * m_NumberOfColumns + x];

	// Depending on the current side shown, "flip" the grid item by fading out
	// the current side while fading in the other side.

	if( pItem -> IsShowingFrontside() )
	{
		m_Tweener.Tween( kfFlipGridItemAnimationLength,
						FLOAT, &pItem -> m_Alpha, 0.0f,
						FLOAT, &pItem -> GetBackside() -> m_Alpha, 1.0f,
						EASING, Ease::sineInOut,
						END );
	}else
	{
		m_Tweener.Tween( kfFlipGridItemAnimationLength,
						FLOAT, &pItem -> m_Alpha, 1.0f,
						FLOAT, &pItem -> GetBackside() -> m_Alpha, 0.0f,
						EASING, Ease::sineInOut,
						END );
	}

	pItem -> SetFrontSideShown( !( pItem -> IsShowingFrontside() ) );
}

bool Grid::CheckPair( int x1, int y1, int x2, int y2 )
{
	// Check if the two grid items at the given coordinates have identical planets
	// attached to them and remove them from the grid if that's the case

	if( m_pGridItems[y1 * m_NumberOfColumns + x1] -> GetPlanet() == m_pGridItems[y2 * m_NumberOfColumns + x2] -> GetPlanet() )
	{
		m_RemovedItems.push_back( m_pGridItems[y1 * m_NumberOfColumns + x1] );
		m_RemovedItems.push_back( m_pGridItems[y2 * m_NumberOfColumns + x2] );

		g_pAudioHandler->PlaySound(AudioHandler::SoundEffect::keSuccess);

		// Remove the pair
		DestroyItem(m_pGridItems[y1 * m_NumberOfColumns + x1]);
		DestroyItem(m_pGridItems[y2 * m_NumberOfColumns + x2]);

		return true;
	}

	return false;
}

void Grid::MemoryRefill()
{
	// Make sure there is an even number of every planet and there is about
	// the same number of each planet present on the grid.

	int numberOfFields = m_NumberOfRows * m_NumberOfColumns;

	// This many pairs will be placed on the grid
	int numberOfPairs = numberOfFields / 2;
	// The remaining fields (0 for even or 1 for odd number of fields)
	int rest = numberOfFields % 2;
	// Stores how many shapes of each kind will be placed on the grid
	int itemsPerShape[GridItem::Planet::keNumberOfPlanets] = {0};

	// Determine the number of pairs for every kind of planet
	int i = 0;
	while( numberOfPairs > 0 )
	{
		itemsPerShape[i] += 2;
		--numberOfPairs;
		i = (i + 1) % GridItem::Planet::keNumberOfPlanets;
	}

	// Fills all empty spaces of the grid and shows the backside to hide the actual shape
	// ( One grid field might be left empty if there is an odd number of fields )
	std::list<GridItem*>::iterator it = m_RemovedItems.begin();
	int count = 0;
	while( count < numberOfFields - rest)
	{
		int gridX = 0;
		int gridY = 0;
		ScreenCoordinatesToGridField(  static_cast<int>( (*it) -> m_X ), static_cast<int>( (*it) -> m_Y ), gridX, gridY );

		int itemIndex = gridY * m_NumberOfColumns + gridX;

		m_pGridItems[itemIndex] = (*it);

		// Set the new planet
		GridItem::Planet chosenPlanet = GridItem::Planet( 0 );
		do
		{
			chosenPlanet = GridItem::Planet( IwRand() % GridItem::Planet::keNumberOfPlanets );
		}while( itemsPerShape[chosenPlanet] == 0 );

		--itemsPerShape[chosenPlanet];
		m_pGridItems[itemIndex] -> SetPlanet( chosenPlanet );
		
		// Make sure the frontside is not shown
		m_pGridItems[itemIndex] -> m_Alpha = 0.0f;
		m_pGridItems[itemIndex] -> SetFrontSideShown(false);

		// Fade in the backside
		m_Tweener.Tween( kfMemoryModeInsertItemsAnimationLength,
						FLOAT, &m_pGridItems[itemIndex] -> GetBackside() -> m_Alpha, 1.0f,
                        EASING, Ease::sineInOut,
                        END );

		// Start a timer and wait for the "insertion" (fade in) to complete.
		m_TimerManager.Add( "InsertItems", kfMemoryModeInsertItemsTimer, Grid::on_insertion_finished, this, 1 );

		++count;
		++it;
	}		 

	// No more empty spaces now, clear the list
	if( 0 == rest )
	{
		m_RemovedItems.clear();
	}else
	{
		// Make sure the one item left is not deleted from the list (might be reused later)
		m_RemovedItems.erase( m_RemovedItems.begin(), it);
	}
}

void Grid::InitMemoryGrid()
{
	// Make sure there is an even number of every planet and there is about
	// the same number of each planet present on the grid.

	int numberOfFields = m_NumberOfRows * m_NumberOfColumns;

	// This many pairs will be placed on the grid
	int numberOfPairs = numberOfFields / 2;
	// The remaining fields (0 for even or 1 for odd number of fields)
	int rest = numberOfFields % 2;
	// Stores how many shapes of each kind will be placed on the grid
	int itemsPerShape[GridItem::Planet::keNumberOfPlanets] = {0};

	// Determine the number of pairs for every kind of planet
	int i = 0;
	while( numberOfPairs > 0 )
	{
		itemsPerShape[i] += 2;
		--numberOfPairs;
		i = (i + 1) % GridItem::Planet::keNumberOfPlanets;
	}

	// One grid field might be left empty if there is an odd number of fields
	// Fills all empty spaces of the grid and shows the backside to hide the actual planet
	for( int i = 0; i < numberOfFields - rest; ++i )
	{
		// Set the new planet
		GridItem::Planet chosenPlanet = GridItem::Planet( 0 );
		do
		{
			chosenPlanet = GridItem::Planet( IwRand() % GridItem::Planet::keNumberOfPlanets );
		}while( itemsPerShape[chosenPlanet] == 0 );

		--itemsPerShape[chosenPlanet];
		m_pGridItems[i] -> SetPlanet( chosenPlanet );
		
		// Make sure the frontside is not shown
		m_pGridItems[i] -> GetBackside() -> m_Alpha = 1.0f;
		m_pGridItems[i] -> m_Alpha = 0.0f;
		m_pGridItems[i] -> SetFrontSideShown(false);
	}	

	if( 1 == rest )
	{
		m_pGridItems[numberOfFields - 1] -> GetBackside() -> m_Alpha = 0.0f;
		m_pGridItems[numberOfFields - 1] -> m_Alpha = 0.0f;
		m_pGridItems[numberOfFields - 1] -> SetFrontSideShown(true);

		m_RemovedItems.push_back( m_pGridItems[numberOfFields - 1] );
		m_pGridItems[numberOfFields - 1] = 0;
	}
}

void Grid::Reset()
{
	// Prepare the grid for a new game

	m_Tweener.Clear();
	m_TimerManager.Clear();

	RestoreGridItems();

	// Game mode specific reset
	switch( m_GameMode )
	{
	case keClassic:
		// Randomly place shapes
		DistributePlanets();
		// Get rid of all groups 
		ResolveGroups();
		break;
	case keMemory:
		// Make sure there are only pairs of shapes
		InitMemoryGrid();
		break;
	}
}

 bool Grid::IsFieldValid( int x, int y )
 {
	 // The field is only valid if there currently is an item associated to it
	 return m_pGridItems[y * m_NumberOfColumns + x] != 0;
 }

 void Grid::RestoreGridItems()
 {
	 for( int row = 0; row < m_NumberOfRows; ++row )
	 {
		 for( int column = 0; column < m_NumberOfColumns; ++column )
		 {
			 if( m_pGridItems[row * m_NumberOfColumns + column] == 0 )
			 {
				 m_pGridItems[row * m_NumberOfColumns + column] = *(m_RemovedItems.begin());
				 m_pGridItems[row * m_NumberOfColumns + column] -> m_X = static_cast<float>( m_OffsetX + column * m_FieldWidth );
				 m_pGridItems[row * m_NumberOfColumns + column] -> m_Y = static_cast<float>( m_OffsetY + row * m_FieldHeight );

				 m_RemovedItems.erase(m_RemovedItems.begin());
			 }
		 }
	 }
 }
