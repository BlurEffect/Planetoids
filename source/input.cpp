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

#include "Input.h"

Input* g_pInput = 0;

//
//
// Input class
//
//

// static callbacks


void Input::touch_button_cb( s3ePointerEvent* event )
{
    g_pInput -> m_PrevTouched = g_pInput -> m_Touched;
    g_pInput -> m_Touched     = event -> m_Pressed != 0;
    g_pInput -> m_X           = event -> m_x;
    g_pInput -> m_Y           = event -> m_y;
}

void Input::touch_motion_cb(s3ePointerMotionEvent* event)
{
    g_pInput -> m_X = event -> m_x;
    g_pInput -> m_Y = event -> m_y;
}

void Input::multi_touch_button_cb(s3ePointerTouchEvent* event)
{
    g_pInput -> m_PrevTouched = g_pInput -> m_Touched;
    g_pInput -> m_Touched     = event -> m_Pressed != 0;
    g_pInput -> m_X           = event -> m_x;
    g_pInput -> m_Y           = event -> m_y;
}

void Input::multi_touch_motion_cb( s3ePointerTouchMotionEvent* event )
{
    g_pInput -> m_X = event -> m_x;
    g_pInput -> m_Y = event -> m_y;
}

///

Input::Input() : m_X( 0 ), 
				 m_Y( 0 ), 
				 m_Touched( false ), 
				 m_PrevTouched( false )
{
    // Set touch event callback handlers, single and multi-touch devices have different callbacks assigned
    if ( s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE ) != 0 )
    {
        s3ePointerRegister( S3E_POINTER_TOUCH_EVENT, (s3eCallback) multi_touch_button_cb, 0 );
        s3ePointerRegister( S3E_POINTER_TOUCH_MOTION_EVENT, ( s3eCallback)multi_touch_motion_cb, 0 );
    }
    else
    {
        s3ePointerRegister( S3E_POINTER_BUTTON_EVENT, (s3eCallback)touch_button_cb, 0 );
        s3ePointerRegister( S3E_POINTER_MOTION_EVENT, (s3eCallback)touch_motion_cb, 0 );
    }
}

void Input::Update()
{
    s3ePointerUpdate();
}

void Input::Reset()
{
    m_PrevTouched = false;
    m_Touched = false;
}




