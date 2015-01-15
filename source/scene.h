
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
 *
 * Edited for the search for a Star Competition 2014
 *
 */

#if !defined(__SCENE_H__)
#define __SCENE_H__

#include <list>
#include "Iw2DSceneGraph.h"
#include "Timer.h"

using namespace Iw2DSceneGraphCore;
using namespace Iw2DSceneGraph;

/**
 * @class Scene
 *
 * @brief A scene acts as a parent of a group of nodes
 *
 * Scenes act as containers that store an hierarchy of CNodes.
 *
 * Note that the Init(), Update() and Render() methods are meant to be overidden in your own scene class that is derived from Scene to
 * provide functionality and rendering that is specific to your own scene class.
 *
 */

class SceneManager;

class Scene : public CNode
{
public:
    Scene( const char* name );
    virtual ~Scene();

    /**
     * @fn    virtual void Scene::Init() = 0;
     *
     * @brief Initialise the scene.
	 *
	 * @param graphicsScale  The scale to apply to UI elements
	 *
     * @return   True if the initialisation succeeded, false otherwise.
     */
    virtual bool Init( float graphicsScale) = 0;

    /**
     * @fn    virtual void Scene::Reset() = 0;
     *
     * @brief Reset the scene. Should be called before activating the scene.
     */
    virtual void Reset() = 0;

    /**
     * @fn    virtual void Scene::Update(float deltaTime = 0.0f, float alphaMul = 1.0f);
	 *
     * @brief Updates this scene
     *
     * @param deltaTime   Number of seconds that has passed since Update() was last called.
     * @param alphaMul    The alpha value being passed down to this node. Default value is 1.
     */
    virtual void Update(float deltaTime = 0.0f, float alphaMul = 1.0f);
    
	/**
     * @fn    virtual void Scene::Render();
     *
     * @brief Render the scene.
     */
    virtual void Render();

	/**
     * @fn    void Scene::SetName( const char* name );
     *
     * @brief Creates a hash for the given string and sets it as m_NameHash for the scene
	 *
     * @param name   The name for the scene
     */
	void SetName( const char* name );
	
	unsigned int  GetNameHash() const     { return m_NameHash; }
    bool          IsActive() const        { return m_IsActive; }
	bool          IsInputActive() const   { return m_IsInputActive; }
	SceneManager* GetSceneManager() const { return m_SceneManager; }

	void SetNameHash( unsigned int nameHash )         { m_NameHash = nameHash; }
    void SetActive( bool active )                     { m_IsActive = active; }
	void SetInputActive( bool active )                { m_IsInputActive = active; }
	void SetSceneManager( SceneManager* manager )     { m_SceneManager = manager; }
 
protected:
    unsigned int        m_NameHash;                     // Hash string representation of scene name
    bool                m_IsActive;                     // Active state
    bool                m_IsInputActive;                // Input active state
    SceneManager*       m_SceneManager;                 // Scene manager
	TimerManager		m_TimerManager;					// Timer manager
};

/**
 * @class SceneManager
 *
 * @brief Manages a collection of scenes.
 *
 * The Scene Manager manages a collection of scenes. It also manages the update and rendering of those scenes and provides a mechanism to
 * easily switch between them.
 *
 * Example usage:
 * @code
 *    // Create scene manager
 *    g_pSceneManager = new SceneManager();
 *
 *    // Create a scene (Game is a class that is derived from Scene)
 *    Game* game = new Game();
 *    game->SetName("game");
 *    g_pSceneManager->Add(game);
 *
 *    while (!s3eDeviceCheckQuitRequest())
 *    {
 *        // Update scene manager
 *        g_pSceneManager->Update(FRAME_TIME);
 *
 *        // Render scene manager
 *        g_pSceneManager->Render();
 *    }
 *
 *    // Cleanup
 *    delete g_pSceneManager;
 * @endcode
 *
 * The scene manager uses the concept of the "current scene" where only one scene can be active at any given time. To change from one
 * scene tp another you use the SwitchTo() method.
 *
 */
class SceneManager
{
public:

    SceneManager();
    virtual ~SceneManager();

	/**
     * @fn    void SceneManager::Init(void);
     *
     * @brief Initialise the scene manager
     *
	 */
	virtual bool Init();

    /**
     * @fn    void SceneManager::SwitchTo(Scene* scene);
     *
     * @brief Switch to a new scene.
     *
     * @param scene   The new scene.
     */
    virtual void    SwitchTo( Scene* scene );

    /**
     * @fn    void SceneManager::Update(float deltaTime = 0.0f);
     *
     * @brief Updates all scenes managed by the manager
     *
     * @param deltaTime   Number of seconds that has passed since Update() was last called.
     */
    virtual void Update( float deltaTime = 0.0f );

    /**
     * @fn    void SceneManager::Render();
     *
     * @brief Renders all scenes managed by the manager
     */
    void    Render();

    /**
     * @fn    void SceneManager::Add(Scene* scene);
     *
     * @brief Adds a scene to the manager.
     *
     * @param scene   If non-null, the scene to add.
     */
    void    Add( Scene* scene );

    /**
     * @fn    void SceneManager::Remove(Scene* scene);
     *
     * @brief Removes the given scene from the scene manager.
     *
     * @param scene   If non-null, the scene to remove.
     */
    void    Remove( Scene* scene );

    /**
     * @fn    Scene* SceneManager::Find(const char* name);
     *
     * @brief Searches for the named scene.
     *
     * @param name    The name of the scene to find.
     *
     * @return    Null if it fails, else the scene.
     */
    Scene*  Find( const char* name );

	Scene*  GetCurrent() const            { return m_Current; }
	float   GetGraphicsScale( void) const { return m_GraphicsScale; }

protected:
    Scene*              m_Current;              // Currently active scene
    Scene*              m_Next;                 // Next scene (scene that is being switched to)
    std::list<Scene*>   m_Scenes;               // Scenes list

private:
	static int   m_sDesignScreenWidth; // the UI has been designed to work properly for this resolution 
									   // scale UI according to difference from this value
	float        m_GraphicsScale;      // The scale to use for placement of UI elements
};

/**
 * @brief A pointer to the global scene manager object
 */
extern SceneManager* g_pSceneManager;


#endif  // __SCENE_H__



