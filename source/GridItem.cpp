/*
 * (C) 2014 Search for a Star
 * 
 */

#include "GridItem.h"
#include "resources.h"


using namespace Iw2DSceneGraphCore;
using namespace Iw2DSceneGraph;
using namespace SFAS2014;			

//
//
// GridItem class
//
//

// Highlight selected grid items in this colour
CColor GridItem::m_sHighlightColour = CColor( 128, 128, 128, 255 );

GridItem::GridItem() : m_Planet(Planet( 0 ) ), 
					   m_ShowingFrontside( true ), 
					   m_pBackside( 0 )
{
}

GridItem::~GridItem()
{
}

bool GridItem::Init( float x, float y, float scale, Planet planet )
{
	m_X = x;
	m_Y = y;

	SetPlanet( planet );

	m_W = GetImage()->GetWidth();
	m_H = GetImage()->GetHeight();
	m_AnchorX = 0.0f;
	m_AnchorY = 0.0f;
	
	// Fit grid to screen size
	m_ScaleX = scale;
	m_ScaleY = scale;

	// Create the backside

	m_pBackside = new CSprite();
	if( 0 == m_pBackside )
	{
		return false;
	}
	m_pBackside -> SetImage(g_pResources->GetBackside());

	// Initialise the backside and add it as a child to this grid item

	m_pBackside -> m_X = 0;
	m_pBackside -> m_Y = 0;
	m_pBackside -> m_W = m_W;
	m_pBackside -> m_H = m_H;
	m_pBackside -> m_AnchorX = 0.0f;
	m_pBackside -> m_AnchorY = 0.0f;
	m_pBackside -> m_ScaleX = 1.0f;
	m_pBackside -> m_ScaleY = 1.0f;

	AddChild(m_pBackside);

	// Make the backface invisible by default

	m_pBackside -> m_Alpha = 0;

	return true;
}


void GridItem::Highlight()
{
	m_Color = m_sHighlightColour;
}

void GridItem::Reset()
{
	m_Color = CColor( 255, 255, 255, 255 );
}

void GridItem::SetPlanet( Planet planet )
{
	m_Planet = planet;
	SetImage( GetPlanetImage( planet ) );
}

CIw2DImage* GridItem::GetPlanetImage( Planet planet )
{
	CIw2DImage* pImage = 0;

	switch( m_Planet )
	{
	case kePlanetGrey:
		pImage = g_pResources->GetPlanetGrey();
		break;
	case kePlanetRed:
		pImage = g_pResources->GetPlanetRed();
		break;
	case kePlanetBlue:
		pImage = g_pResources->GetPlanetBlue();
		break;
	case kePlanetYellow:
		pImage = g_pResources->GetPlanetYellow();
		break;
	case kePlanetGreen:
		pImage = g_pResources->GetPlanetGreen();
		break;
	}

	return pImage;
}