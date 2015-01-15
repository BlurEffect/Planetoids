/*
 * (C) 2014 Search for a Star
 * 
 */

#if !defined(__GRIDITEM_H__)
#define __GRIDITEM_H__

#include "Iw2DSceneGraph.h"

using namespace Iw2DSceneGraph;

namespace SFAS2014
{

/**
 * @class GridItem
 *
 * @brief Manages the GridItem items position and the state of the GridItem.
 *
 */
class GridItem : public CSprite
{
public:	
	
	/**
	 * @enum  Planet
	 *
	 * @brief Holds all values for the different available planets
	 */
	enum Planet
	{
		kePlanetGrey,
		kePlanetRed,
		kePlanetBlue,
		kePlanetYellow,
		kePlanetGreen,
		keNumberOfPlanets
	};

	GridItem();
     ~GridItem();
	 
	/**
     * @fn     void GridItem::Init( float x, float y, Planet planet );
     *
     * @brief  Initialise the grid item
     *
	 * @param  x The screen x coordinate at which to place this item (in pixels)
	 * @param  y The screen y coordinate at which to place this item (in pixels)
	 * @param  scale The scale that should be applied to this item
	 * @param  planet Determines the image that will be displayed for this item
	 *
	 * @return True if the initialisation was successful, false otherwise
     */
	bool Init( float x, float y, float scale, Planet planet );

    /**
     * @fn    void GridItem::Highlight( void );
     *
     * @brief Highlight the grid item in the highlight colour
     */
	void Highlight( void );

    /**
     * @fn    void GridItem::Reset( void );
     *
     * @brief Puts the grid item back to its normal colour
     *
     */
	void Reset( void );

	/**
     * @fn    void GridItem::SetPlanet( Planet planet );
     *
     * @brief Changes the image displayed by this grid item
     *
	 * @param planet The planet that should be displayed by the item 
     */
	void SetPlanet( Planet planet );

	Planet   GetPlanet() const                { return m_Planet; }
	bool     IsShowingFrontside( void ) const { return m_ShowingFrontside; }
	CSprite* GetBackside(void) const          { return m_pBackside; }

	void SetFrontSideShown( bool doShowFront ) { m_ShowingFrontside = doShowFront; }
	void SetBackside( CSprite* backside )	   { m_pBackside = backside; }
private: 

	static CColor m_sHighlightColour; // The colour used to highlight selected grid items

	Planet    m_Planet;           // The planet displayed by this grid item
	bool      m_ShowingFrontside; // Tells whether the item is currently showing the frontside or backside
	CSprite * m_pBackside;	      // The "backside" of the item (used in memory game mode to hide the actual image)

	/**
     * @fn     CIw2DImage* GridItem::GetPlanetImage( Planet planet );
     *
     * @brief  Gets the image associated to specific planet
     *
	 * @param  planet The planet, the image of which is demanded
	 *
	 * @return A pointer to the CIw2DImage associated to the planet specified by the parameter
     */
	CIw2DImage* GetPlanetImage( Planet planet );
};
}

#endif  // __GRIDITEM_H__


