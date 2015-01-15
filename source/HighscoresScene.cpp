/*
 * (C) 2014 Search for a Star
 * 
 */

#include "HighscoresScene.h"
#include "IwGx.h"
#include "resources.h"
#include "input.h"
#include "AudioHandler.h"
#include "s3eKeyboard.h"
#include "s3eOSReadString.h"
#include "IwSerialise.h"


using namespace SFAS2014;

//
//
// HighscoresScene class
//
//

int HighscoresScene::m_sNumberOfScores = 8;

const char* HighscoresScene::m_sHighscoreFilenames[] = { "highscores/highscores_classic.bin", 
														 "highscores/highscores_memory.bin" };

HighscoresScene::HighscoresScene( const char* name ) : Scene( name ), 
													   m_pBackground( 0 ),
												       m_pTextPositions( 0 ),
													   m_pTextNames( 0 ),
													   m_pTextScores( 0 ),
													   m_pButtonContinue( 0 ),
													   m_Score( 0 ),
													   m_GameMode( keClassic ),
													   m_Checked( false )
{
	IwUtilInit();
}

HighscoresScene::~HighscoresScene()
{
	IwUtilTerminate();
}

bool HighscoresScene::Init( float graphicsScale )
{
	// Initialisise the highscore entries to placeholders

	for( int i = 0; i < keNumberOfGameModes; ++i )
	{
		for( int j = 0; j < m_sNumberOfScores; ++j )
		{
			m_Entries[i][j].m_Position = j + 1;
			strcpy( m_Entries[i][j].m_Name, "Player" );
			m_Entries[i][j].m_Score = 0;
		}

		// Load existing highscores from the file

		LoadHighscores( GameMode( i ) );
		// SaveHighscores( GameMode( i ) );
	}

	return InitUI( graphicsScale );
}

void HighscoresScene::Reset()
{
	UpdateUI();
	m_Checked = false;
}

void HighscoresScene::LoadHighscores( GameMode mode )
{
	IwSerialiseOpen(m_sHighscoreFilenames[mode], IW_SERIALISE_READ);
	
	for( int i = 0; i < m_sNumberOfScores; ++i )
	{
		m_Entries[mode][i].Serialise();
	}

	IwSerialiseClose();
}
	
void HighscoresScene::SaveHighscores( GameMode mode )
{
	IwSerialiseOpen(m_sHighscoreFilenames[mode], IW_SERIALISE_WRITE);

	for( int i = 0; i < m_sNumberOfScores; ++i )
	{
		m_Entries[mode][i].Serialise();
	}

	IwSerialiseClose();
}

void HighscoresScene::AddNewEntry(  GameMode mode, int position, const char* name, int score )
{
	// If the player didn't provide a name, do not create a new entry
	if( 0 == name )
	{
		return;
	}

	// Move the entries that will be below the new entry down by 1
	for( int i = m_sNumberOfScores - 2; i >= position - 1; --i )
	{
		m_Entries[mode][i+1] = m_Entries[mode][i]; 
		m_Entries[mode][i+1].m_Position += 1;	
	}

	// Set the data for the new entry
	m_Entries[mode][position - 1].m_Position = position;
	strncpy( m_Entries[mode][position - 1].m_Name, name, 10 );
	m_Entries[mode][position - 1].m_Name[10] = '\0';  // make sure there is a null terminator at the end
	m_Entries[mode][position - 1].m_Score    = score;

	// Save the updated highscore list
	SaveHighscores( mode );
}

int HighscoresScene::GetScorePosition( GameMode mode, int score )
{
	// If the score is too small to fit on the list, return immediately
	if( score < m_Entries[mode][m_sNumberOfScores - 1].m_Score )
	{
		return -1;
	}

	// Find the spot at which to insert the new entry
	int i = m_sNumberOfScores - 1;
	while( ( m_Entries[mode][i].m_Score <= score ) && ( i >= 0 ) )
	{
		--i;
	}

	if( i < 0)
	{
		return 1;
	}else
	{
		return i + 2;
	}
}

const char* HighscoresScene::GetPlayerName()
{
	// Brings up a virtual keyboard and prompts the player for his name
	return s3eOSReadStringUTF8("Please enter your name (max 10 characters)", 0);
}

void HighscoresScene::Update(float deltaTime, float alphaMul)
{
	if( !m_IsActive )
	{
		return;
	}

	Scene::Update(deltaTime, alphaMul);

	
	if( !m_Checked )
	{
		// First call of update since the activation of the scene

		// Check if the score of the player made it into the highscores
		int pos = GetScorePosition( m_GameMode, m_Score );
		if( pos > 0 )
		{
			// Add new entry to highscore list
			AddNewEntry( m_GameMode, pos, GetPlayerName(), m_Score );
		}
		m_Checked = true;
		UpdateUI();
	}

	// Check if the button was hit
    if( m_IsInputActive && !g_pInput->GetTouched() && g_pInput->GetPrevTouched() )
    {
        g_pInput -> Reset();
		if ( m_pButtonContinue -> HitTest( g_pInput->GetX(), g_pInput->GetY() ) )
        {
			Scene* pNext = m_SceneManager -> Find("TitleScene");
			m_SceneManager -> SwitchTo( pNext );
			g_pAudioHandler -> PlaySound(AudioHandler::SoundEffect::keSelect);
		}else
		{
			g_pAudioHandler -> PlaySound(AudioHandler::SoundEffect::keDenied);
		}
	}
}


void HighscoresScene::UpdateUI()
{
	// Create char buffers for the output text (big enough to hold the maximal possible size)
	char bufferPositions[16]; // 8 single-digit positions + 7 new line chars + 1 null terminator = 16
	char bufferNames[88];     // 8 names with max. 10 chars each + 7 new line chars + 1 null terminator = 88
	char bufferScores[72];    // 8 scores with max. 8 digits + 7 new line chars + 1 null terminator = 72

	// Init buffers
	memset( bufferPositions, 0, 16 );
	memset( bufferNames, 0, 88 );
	memset( bufferScores, 0, 72 );

	// Get the text strings
	GetPositionsString( m_GameMode, bufferPositions );
	GetNamesString( m_GameMode, bufferNames );
	GetScoresString( m_GameMode, bufferScores );

	// Display the text on the screen
	m_pTextPositions -> SetText( bufferPositions );
	m_pTextNames -> SetText( bufferNames );
	m_pTextScores -> SetText( bufferScores );
}

void HighscoresScene::GetPositionsString( GameMode mode, char* outText )
{
	// Build a text column with the position information

	int pos = 0;
	int charsWritten = 0;

	for(int i = 0; i < m_sNumberOfScores; ++i)
	{
		charsWritten = sprintf(&outText[pos], "%d", m_Entries[mode][i].m_Position );
		pos += charsWritten;
		if( m_Entries[mode][i].m_Position != m_sNumberOfScores )
		{
			outText[pos] = '\n';
			++pos;
		}
	}
}

void HighscoresScene::GetNamesString( GameMode mode, char* outText )
{
	// Build a text column with the name information

	int pos = 0;
	int charsWritten = 0;

	for(int i = 0; i < m_sNumberOfScores; ++i)
	{
		charsWritten = sprintf(&outText[pos], "%s", m_Entries[mode][i].m_Name );
		pos += charsWritten;
		if( m_Entries[mode][i].m_Position != m_sNumberOfScores )
		{
			outText[pos] = '\n';
			++pos;
		}
	}
}

void HighscoresScene::GetScoresString( GameMode mode, char* outText )
{
	// Build a text column with the score information

	int pos = 0;
	int charsWritten = 0;

	for(int i = 0; i < m_sNumberOfScores; ++i)
	{
		charsWritten = sprintf(&outText[pos], "%d", m_Entries[mode][i].m_Score );
		pos += charsWritten;
		if( m_Entries[mode][i].m_Position != m_sNumberOfScores )
		{
			outText[pos] = '\n';
			++pos;
		}
	}
}

bool HighscoresScene::InitUI( float graphicsScale )
{
	// Create the background sprite

	m_pBackground = new CSprite();
	if( 0 == m_pBackground )
	{
		return false;
	}
	m_pBackground -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pBackground -> m_Y       = static_cast<float>( IwGxGetScreenHeight() ) * 0.5f;
	m_pBackground -> SetImage( g_pResources -> GetBackgroundHighscores() );
	m_pBackground -> m_W       = m_pBackground->GetImage() -> GetWidth();
	m_pBackground -> m_H       = m_pBackground->GetImage() -> GetHeight();
	m_pBackground -> m_AnchorX = 0.5;
	m_pBackground -> m_AnchorY = 0.5;
 
	// Fit background to screen size
	m_pBackground -> m_ScaleX  = static_cast<float>( IwGxGetScreenWidth() ) / m_pBackground -> GetImage() -> GetWidth();
	m_pBackground -> m_ScaleY  = static_cast<float>( IwGxGetScreenHeight() ) / m_pBackground -> GetImage() -> GetHeight();
	AddChild( m_pBackground );

	m_pTextPositions = new CLabel();
	if( 0 == m_pTextPositions )
	{
		return false;
	}
	m_pTextPositions -> m_X        = static_cast<float>( IwGxGetScreenWidth() ) * 0.1f;
	m_pTextPositions -> m_Y        = static_cast<float>( IwGxGetScreenHeight() ) * 0.225f;
	m_pTextPositions -> m_W        = static_cast<float>( IwGxGetScreenWidth() );
	m_pTextPositions -> m_H		   = 200;
	m_pTextPositions -> SetFont( g_pResources -> GetFontSmall() );
	m_pTextPositions -> SetText("0");
	m_pTextPositions -> m_AnchorX  = 1.0;
	m_pTextPositions -> m_AnchorY  = 0.0;
	m_pTextPositions -> m_AlignHor = CIw2DFontAlign::IW_2D_FONT_ALIGN_RIGHT;
	m_pTextPositions -> m_Color    = CColor( 255,255,255,255 );
	m_pTextPositions -> m_ScaleX   = graphicsScale;
	m_pTextPositions -> m_ScaleY   = graphicsScale;
	AddChild( m_pTextPositions );

	m_pTextNames = new CLabel();
	if( 0 == m_pTextNames )
	{
		return false;
	}
	m_pTextNames -> m_X		   = static_cast<float>( IwGxGetScreenWidth() ) * 0.15f;
	m_pTextNames -> m_Y		   = static_cast<float>( IwGxGetScreenHeight() ) * 0.225f;
	m_pTextNames -> m_W        = static_cast<float>( IwGxGetScreenWidth() );
	m_pTextNames -> m_H        = 200;
	m_pTextNames -> SetFont( g_pResources -> GetFontSmall() );
	m_pTextNames -> SetText( "Player" );
	m_pTextNames -> m_AnchorX  = 0.0;
	m_pTextNames -> m_AnchorY  = 0.0;
	m_pTextNames -> m_AlignHor = CIw2DFontAlign::IW_2D_FONT_ALIGN_LEFT;
	m_pTextNames -> m_Color    = CColor(255,255,255,255);
	m_pTextNames -> m_ScaleX   = graphicsScale;
	m_pTextNames -> m_ScaleY   = graphicsScale;
	AddChild( m_pTextNames );

	m_pTextScores = new CLabel();
	if( 0 == m_pTextScores )
	{
		return false;
	}
	m_pTextScores -> m_X        = static_cast<float>( IwGxGetScreenWidth() ) * 0.9f;
	m_pTextScores -> m_Y        = static_cast<float>( IwGxGetScreenHeight() ) * 0.225f;
	m_pTextScores -> m_W        = static_cast<float>( IwGxGetScreenWidth() );
	m_pTextScores -> m_H        = 200;
	m_pTextScores -> SetFont( g_pResources -> GetFontSmall() );
	m_pTextScores -> SetText("0");
	m_pTextScores -> m_AnchorX  = 1.0;
	m_pTextScores -> m_AnchorY  = 0.0;
	m_pTextScores -> m_AlignHor = CIw2DFontAlign::IW_2D_FONT_ALIGN_RIGHT;
	m_pTextScores -> m_Color    = CColor(255,255,255,255);
	m_pTextScores -> m_ScaleX   = graphicsScale;
	m_pTextScores -> m_ScaleY   = graphicsScale;
	AddChild( m_pTextScores );

	// add continue button
	m_pButtonContinue = new CSprite();
	if( 0 == m_pButtonContinue )
	{
		return false;
	}
	m_pButtonContinue -> m_X       = static_cast<float>( IwGxGetScreenWidth() ) * 0.5f;
	m_pButtonContinue -> m_Y       = static_cast<float>(IwGxGetScreenHeight() ) * 0.725f;
	m_pButtonContinue -> SetImage( g_pResources -> GetButtonContinue() );
	m_pButtonContinue -> m_W       = m_pButtonContinue->GetImage()->GetWidth();
	m_pButtonContinue -> m_H       = m_pButtonContinue->GetImage()->GetHeight();
	m_pButtonContinue -> m_AnchorX = 0.5;
	m_pButtonContinue -> m_AnchorY = 0.5;
	m_pButtonContinue -> m_ScaleX  = 0.5f * graphicsScale;
	m_pButtonContinue -> m_ScaleY  = 0.5f * graphicsScale;
	AddChild( m_pButtonContinue );

	return true;
}