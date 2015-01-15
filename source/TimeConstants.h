
#if !defined(__TIMECONSTANTS_H__)
#define __TIMECONSTANTS_H__

namespace SFAS2014
{

// Simply a collection of constants determining how long animations and timers run
// during the execution of the game. Bundled here for easier tweaking without having 
// to look at or change the rest of the code (although it's weakening encapsulation).

const float kfSceneSwitchTimer					    = 0.3f;  // Wait this long before activating the new scene
const float kfSceneSwitchAnimationLength			= 0.3f;  // The duration of the sliding animation played on scene switch

const float kfSwapGridItemsTimer					= 0.6f;  // Wait this long for items on the grid to get swapped
const float kfSwapGridItemsAnimationLength		    = 0.5f;  // The duration of the swap animation

const float kfFlipSecondMemoryTileTimer			    = 0.8f;  // Wait this long before checking a pair of grid tiles (includes time for memorisation)
const float kfFlipBackMemoryTilesTimer				= 0.4f;  // Wait this long for the shown memory tiles to get flipped back
const float kfFlipGridItemAnimationLength		    = 0.3f;  // The duration of the flip animation for grid items

const float kfDestroyGroupsTimer                    = 0.3f;  // Wait this long for groups of grid items to get removed from the grid
const float kfDestroyPairTimer                      = 0.3f;  // Wait this long for a pair of memory tiles to get removed from the grid
const float kfDestroyGridItemAnimationLength		= 0.2f;  // The duration of the destroy (fade out) animation for grid items

const float kfMemoryModeInsertItemsTimer            = 0.6f;  // Wait this long for new memory tiles to be inserted into the grid
const float kfMemoryModeInsertItemsAnimationLength  = 0.5f;  // The duration of the insert (fade in) animation for new memory tiles

const float kfClassicModeInsertItemsTimer           = 0.6f;  // Wait this long for new grid items to be inserted into the top row in classic mode
const float kfClassicModeInsertItemsAnimationLength = 0.5f;  // The duration of the insert (fade in) animation for new grid items in classic mode

const float kfClassicModeMoveItemsTimer			    = 0.15f; // Wait this long for grid items to move one row down in classic mode
const float kfClassicModeMoveItemsAnimationLength   = 0.1f;  // The duration of the move animation for grid items in classic mode

}

#endif  // __TIMECONSTANTS_H__







