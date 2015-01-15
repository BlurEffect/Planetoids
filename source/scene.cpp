/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */

#include "scene.h"
#include "IwGx.h"
#include "input.h"
#include "TimeConstants.h"


SceneManager* g_pSceneManager = 0;

//
//
// Scene class
//
//

Scene::Scene(const char* name) :  m_IsActive(false), m_IsInputActive(false), m_SceneManager( 0 )
{
	SetName(name);
    m_IsVisible = false;
}

Scene::~Scene()
{
}

void Scene::SetName(const char* name)
{
    m_NameHash = IwHashString(name);
}

void Scene::Update(float deltaTime, float alphaMul)
{
    if (!m_IsActive)
	{
        return;
	}

	m_TimerManager.Update( deltaTime );

    CNode::Update(deltaTime, alphaMul);
}

void Scene::Render()
{
    CNode::Render();
}

//
//
// SceneManager class
//
//

int SceneManager::m_sDesignScreenWidth    = 320; //320

SceneManager::SceneManager() : m_Current(0), m_Next(0), m_GraphicsScale( 1.0f)
{
	
}

SceneManager::~SceneManager()
{
    for (std::list<Scene*>::iterator it = m_Scenes.begin(); it != m_Scenes.end(); ++it)
	{
        delete *it;
	}
}

bool SceneManager::Init()
{
	// Determine the scale required to properly place UI elements
	m_GraphicsScale = static_cast<float>( IwGxGetScreenWidth() ) / m_sDesignScreenWidth;

	// Initialise the scenes
	for( std::list<Scene*>::iterator it = m_Scenes.begin(); it != m_Scenes.end(); ++it )
	{
		(*it) -> Init( GetGraphicsScale() );
		(*it) -> SetActive( false );
		(*it) -> SetSceneManager( this );
	}

	return true;
}

void SceneManager::Add( Scene* scene )
{
    m_Scenes.push_back(scene);
}

void SceneManager::Remove( Scene* scene )
{
    m_Scenes.remove(scene);
}

Scene* SceneManager::Find( const char* name )
{
    unsigned int name_hash = IwHashString(name);
    for ( std::list<Scene*>::iterator it = m_Scenes.begin(); it != m_Scenes.end(); ++it )
    {
        if ( name_hash == (*it) -> GetNameHash() )
		{
            return *it;
		}
    }

    return 0;
}

void SceneManager::Update(float deltaTime)
{
    for (std::list<Scene*>::iterator it = m_Scenes.begin(); it != m_Scenes.end(); ++it)
	{
        (*it)->Update(deltaTime);
	}
}

void SceneManager::Render()
{
    for (std::list<Scene*>::iterator it = m_Scenes.begin(); it != m_Scenes.end(); ++it)
	{
        (*it)->Render();
	}
}

void SceneManager::SwitchTo( Scene* scene )
{
	if( scene != 0 )
	{
		m_Next = scene;
		if ( 0 == m_Current )
		{
			m_Current = m_Next;
			m_Current -> m_IsVisible = true;
			m_Current -> Reset();
			m_Current -> SetActive( true );
			m_Current -> SetInputActive( true );
			m_Next = 0;
		}
		else
		{
			// Activate and make visible the new scene
			m_Next -> Reset();
			m_Next -> SetActive( true );
			m_Next -> SetInputActive( true );
			m_Next -> m_IsVisible = true;

			// Deactivate and hide the old scene
			m_Current -> SetInputActive( false );
			m_Current -> SetActive( false );
			m_Current -> m_IsVisible = false;

			m_Current = m_Next;
			m_Next = 0;
		}
	}
}
