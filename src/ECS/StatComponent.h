#pragma once

#include "Components.h"

#include <SDL.h>

class StatComponent : public Component
{
public:
	int health[2];			// [0] : current health, [1] : maximum health
	int speed;
	int attackDamage;

	bool isAlive;
	bool isInvincible;

	Uint32 lastInvincibility;

	StatComponent(int health, int speed, int attackDamage) : health{ health, health }, speed(speed), attackDamage(attackDamage)
	{}

	~StatComponent(){}

	void Init() override
	{
		isAlive = true;
		isInvincible = false;
		lastInvincibility = 0;
	}

	void Update(float& deltaTime) override
	{
		if (health[0] <= 0)
		{
			isAlive = false;
		}

		if (isInvincible == true && (lastInvincibility + 2000) <= SDL_GetTicks())
		{
			isInvincible = false;
		}
	}

	int GetHealthPercent()
	{
		return (health[0] / health[1]) * 100;
	}
};