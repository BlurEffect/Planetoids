#include "Timer.h"
#include "IwHashString.h"

//
//
// Timer class
//
//

Timer::Timer( unsigned int nameHash, float duration, Callback pCallback, 
			  void* pContext, int numOfRepeats, TimerManager* pManager) : m_NameHash( nameHash ),       
																		  m_IsActive( true ), 
																		  m_Duration( duration ), 
																		  m_TimeLeft( duration ),    
																		  m_pOnTimeout( pCallback ), 
																		  m_pContext( pContext ), 
																	      m_NumberOfRepeats( numOfRepeats ), 
																		  m_pManager( pManager )
{

}

Timer::~Timer()
{
}

bool Timer::Update( float deltaTime )
{
	if( m_IsActive )
	{
		m_TimeLeft -= deltaTime;

		if( m_TimeLeft <= 0 )
		{
			// Timer expired -> Callback
			m_pOnTimeout( this, m_pContext );
			if (m_NumberOfRepeats > 0)
			{
				--m_NumberOfRepeats;
				if (m_NumberOfRepeats == 0)
				{
					m_IsActive = false;

					// Timer expired and out of repeats
					return false;
				}
			}
			// Reset the timer (If there are repeats left, the timer will start counting down again)
			m_TimeLeft += m_Duration;
		}
		return true;
	}
	return false;
}


void Timer::Stop()
{
	m_IsActive = false;
}

void Timer::Start()
{
	m_IsActive = true;
}

//
//
// TimerManager class
//
//

TimerManager::TimerManager()
{
}

TimerManager::~TimerManager()
{
	// Release all allocated timers
	for (std::list<Timer*>::iterator it = m_Timers.begin(); it != m_Timers.end(); ++it)
	{
		delete *it;
	}
}

void TimerManager::Update(float deltaTime)
{
	for ( std::list<Timer*>::iterator it = m_Timers.begin(); it != m_Timers.end(); ++it )
	{
		// Update all timers
		if ( !(*it) -> Update( deltaTime ) )
        {
			// if the timer expired and has no repeats left, get rid of it
            delete *it;
            it = m_Timers.erase(it);
        }
	}
}

void TimerManager::Add( const char* name, float duration, Callback pCallback, void* pContext, int numOfRepeats )
{
	// Create a new timer
	unsigned int nameHash = IwHashString(name);

	Timer* pTimer = new Timer( nameHash, duration, pCallback, pContext, numOfRepeats, this );
	if( 0 == pTimer )
	{
		// If the timer couldn't be created, call back the caller immediately
		pCallback( 0, pContext );
	}else
	{
		m_Timers.push_back( pTimer );
	}
}

void TimerManager::Cancel( const char* name )
{
	Timer* pTimer = Find( name );
	if( pTimer != 0 )
	{
		m_Timers.remove( pTimer );
	}
}

void TimerManager::Clear()
{
	for (std::list<Timer*>::iterator it = m_Timers.begin(); it != m_Timers.end(); ++it)
	{
        delete *it;
	}
    m_Timers.clear();
}

Timer* TimerManager::Find( const char* name )
{
    unsigned int nameHash = IwHashString( name );
	for ( std::list<Timer*>::iterator it = m_Timers.begin(); it != m_Timers.end(); ++it )
    {
        if ( nameHash == (*it) -> GetNameHash() )
		{
			return *it;
		}
            
    }
	return 0;
}

void TimerManager::Stop()
{
	// Stop all timers
	for ( std::list<Timer*>::iterator it = m_Timers.begin(); it != m_Timers.end(); ++it )
    {
        (*it) -> Stop();
    }
}

void TimerManager::Stop( const char* name )
{
	Timer* pTimer = Find( name );
	if( pTimer != 0 )
	{
		pTimer -> Stop();
	}
}

void TimerManager::Start()
{
	// Start all timers
	for (std::list<Timer*>::iterator it = m_Timers.begin(); it != m_Timers.end(); ++it)
    {
        (*it) -> Start();
    }
}

void TimerManager::Start(const char* name)
{
	Timer* pTimer = Find( name );
	if( pTimer != 0 )
	{
		pTimer -> Start();
	}
}







