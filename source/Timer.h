
#if !defined(__TIMER_H__)
#define __TIMER_H__

#include <list>

class Timer;
typedef void (*Callback)(Timer* pTimer, void* context); // The type required by callbacks to register with the timer

// forward declaration
class TimerManager;

/**
 * @class Timer
 *
 * @brief A simple timer class.
 *
 */
class Timer
{
public:

	Timer( unsigned int m_NameHash, float duration, Callback pCallback, void* pContext, int numOfRepeats, TimerManager* pManager );
    ~Timer();

	/**
     * @fn     void Timer::Update( float deltaTime);
     *
     * @brief  Update the timer.
     *
     * @param  deltaTime  The time passed since the last frame.
	 *
	 * @return False if the timer is expired, true otherwise
     */
    bool Update( float deltaTime );

    /**
     * @fn    void Timer::Stop();
     *
     * @brief Stop the timer at the current time left
     */
    void Stop();

    /**
     * @fn    void Timer::Start();
     *
     * @brief Start the timer with the time left on the clock.
     */
    void Start();

	unsigned int GetNameHash() {return m_NameHash;}

private:
    unsigned int    m_NameHash;         // Hash string representation of timer name
    bool            m_IsActive;         // Timer active or paused
    float           m_Duration;         // Duration of the countdown
    float           m_TimeLeft;         // Time left on the clock
    
	Callback        m_pOnTimeout;       // The callback function to be called when the timer expires
    void*           m_pContext;         // Used as argument for the callback (pointer to the object that added the timer)
    int             m_NumberOfRepeats;  // Repeat the timer this many times
	TimerManager*   m_pManager;         // Timer manager
};

/**
 * @class TimerManager
 *
 * @brief Manages a collection of timers
 *
 */
class TimerManager
{
public:

    TimerManager();
    ~TimerManager();

    /**
     * @fn    void TimerManager::Update(float deltaTime);
     *
     * @brief Updates all timers that are managed by this this timer manager.
     *
     * @param deltaTime The time passed since the last frame
     */
    void Update( float deltaTime );

    /**
     * @fn    void TimerManager::Add( const char* name, bool startImmediately, float duration, Callback pCallback, void* pContext, int numOfRepeats );
     *
     * @brief Adds a timer to this manager.
	 *
	 * If the manager cannot create a timer, it will immediately call back the caller with the passed in callback function.
     * In that case, the timer parameter of the callback will be set to null.
	 *
     * @param name			The name of the timer
	 * @param duration      The duration of the timer in seconds
	 * @param pCallback     The callback function to call when the timer expires
	 * @param pContext      The context that should be passed by the callback function
	 * @param numOfRepeats  Determines how many times the timer will be restarted after expiring for the first time, 0 means infinite repeats
     */
    void Add( const char* name, float duration, Callback pCallback, void* pContext, int numOfRepeats );

    /**
     * @fn    void TimerManager::Cancel( const char* name );
     *
     * @brief Cancels the timer specified by the parameter.
     *
     * @param name The name of the timer to cancel
     */
    void Cancel( const char* name );

    /**
     * @fn    void TimerManager::Clear();
     *
     * @brief Cancels and destroys all timers managed by this manager.
     */
    void Clear();

    /**
     * @fn    void TimerManager::Stop();
     *
     * @brief Stops all timers managed by this timer manager.
     */
    void Stop();

	/**
     * @fn    void TimerManager::Stop( const char* name );
     *
     * @brief Stops the timer specified by the parameter
	 *
     * @param name    The name of the timer to stop
     */
    void Stop( const char* name );

    /**
     * @fn    void TimerManager::Start();
     *
     * @brief Starts all timers managed by this timer manager.
     */
    void Start();

	/**
     * @fn    void TimerManager::Start( const char* name );
     *
     * @brief Starts the time specified by the parameter.
	 *
     * @param name    The name of the timer to start.
     */
    void Start(const char* name);

private:
    std::list<Timer*> m_Timers; // List of timers

	/**
     * @fn    Timer* TimerManager::Find( const char* name );
     *
     * @brief Finds the named timer.
     *
     * @param name The name of the timer.
     *
     * @return  A pointer to the timer if the manager contains a timer with the given name, false otherwise
     */
    Timer* Find(const char* name);
};


#endif  // __TIMER_H__




