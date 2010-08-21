//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Timer class
// ---------------
//

#if !defined __DH_TIMER_H__
#define __DH_TIMER_H__

class TimerData;

class Timer
{
public:
    Timer();
    ~Timer();

    float GetMs();
    float PollMs();
    void WaitMs(float deltams);

private:
    TimerData *data;
};

#endif // __DH_TIMER_H__

