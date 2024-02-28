#pragma once

#include <SDL.h>

struct Time
{
	float delta;			// time between frames
	float now;				// time of current frame
	float last;				// time of the last frame

	float frameLast;		// the last time fps is calculated
	float frameDelay;		// maximum amount of time a frame should take
	float frameTime;		// how long the current frame took

	Uint32 frameRate;		// frame per second (FPS)
	Uint32 frameCount;		// to help calculate FPS

	Time(){}
	~Time(){}

	void TimeInit(const Uint32& frameRate)
	{
		this->frameRate = frameRate;
		this->frameDelay = 1000.0f / frameRate;
	}

	void TimeUpdate()
	{
		now = static_cast<float>(SDL_GetTicks());
		delta = (now - last) / 1000.0f;
		last = now;

		++frameCount;

		if ((now - last) >= 1000.0f)
		{
			frameRate = frameCount;
			frameCount = 0;
			frameLast = now;
		}
	}


	void TimeUpdateLate()
	{
		frameTime = static_cast<float>(SDL_GetTicks()) - now;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
};