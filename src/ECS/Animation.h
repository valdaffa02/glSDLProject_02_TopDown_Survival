#pragma once


struct Animation
{
	int index;			// animation index in the spriteSheet (vertical index)
	int frames;			// number of frames in the animation (horizontal index)
	int speed;

	Animation(){}

	Animation(int index, int frames, int speed) : index(index), frames(frames), speed(speed)
	{}
};