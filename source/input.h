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

#if !defined(_INPUT_H)
#define _INPUT_H

#include "s3ePointer.h"

#define MAX_TOUCHES     10

/**
 * @class Input
 *
 * @brief Input - The Input class is responsible for handling all pointer input.
 *
 * Example usage:
 * @code
 *    // Set up input systems
 *    g_pInput = new Input();
 *
 *    // Update
 *    while (!s3eDeviceCheckQuitRequest())
 *    {
 *        // Update input system
 *        g_pInput->Update();
 *    }
 *
 *    // Cleanup
 *    delete g_pInput;
 * @endcode
 *
 */
class Input
{
public:
    Input();

    /**
     * @fn    void Input::Update()
     *
     * @brief Updates the input system, called every frame
     */
    void Update();

    /**
     * @fn    void Input::Reset()
     *
     * @brief Resets touch status, usually called after a touch event has been acknowledged
     */
    void Reset();

    // Callbacks

	/**
	 * @fn    void Input::touch_button_cb( s3ePointerEvent* event )
	 *
	 * @brief The system will call this callback when the user touches the screen.
	 *
	 * @param event   If non-null, the event.
	 */
    static void touch_button_cb( s3ePointerEvent* event );

	/**
	 * @fn    void Input::touch_motion_cb( s3ePointerMotionEvent* event )
	 *
	 * @brief The system will call this callback when the user moves their finger on the screen.
	 *
	 * @param event   If non-null, the event.
	 */
    static void touch_motion_cb( s3ePointerMotionEvent* event );

	/**
	 * @fn    void Input::multi_touch_button_cb( s3ePointerTouchEvent* event )
	 *
	 * @brief The system will call this callback when the user touches the screen.
	 *
	 * @param event   If non-null, the event.
	 */
    static void multi_touch_button_cb( s3ePointerTouchEvent* event );

	/**
	 * @fn    void Input::multi_touch_motion_cb( s3ePointerTouchMotionEvent* event )
	 *
	 * @brief The system will call this callback when the user moves their finger on the screen.
	 *
	 * @param event   If non-null, the event.
	 */
    static void multi_touch_motion_cb( s3ePointerTouchMotionEvent* event );

	int GetX( void ) const           { return m_X; }
	int GetY( void ) const           { return m_Y; }
	int GetTouched( void ) const     { return m_Touched; }
	int GetPrevTouched( void ) const { return m_PrevTouched; }

private:
	int  m_X;			// Touched position, x screen coordinate
	int  m_Y;           // Touched position, y screen coordinate
    bool m_Touched;     // Touched status
    bool m_PrevTouched; // Previous touched status
};

/**
 * @brief A pointer to the global input object.
 */
extern Input* g_pInput;


#endif  // _INPUT_H
