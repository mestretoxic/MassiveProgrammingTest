#ifndef TIMER_H
#define TIMER_H

struct Timer
{
	float delay;
	float elapsed;

	bool isComplete;
	bool isRunning;

	explicit Timer(const float delay)
	: delay(delay)
	, elapsed(0.f)
	, isComplete(false)
	, isRunning(false)
	{
		elapsed = delay;
	}

	void Restart()
	{
		elapsed = delay;
		Start();
	}

	void Start()
	{
		isRunning = true;
		isComplete = false;
	}

	void Stop()
	{
		isRunning = false;
		isComplete = true;
	}

	void Reset()
	{
		Stop();
		isRunning = false;
		isComplete = false;
	}

	void Update(const float dt)
	{
		if (!isRunning)
			return;

		elapsed -= dt;
		if (elapsed < 0.f)
		{
			Stop();
		}
	}
};

#endif