#pragma once
#include "Actor.h"
class Ship : public Actor {
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
	float GetDownSpeed() const { return mDownSpeed; }
	float GetRightSpeed() const { return mRightSpeed; }
private:
	float mDownSpeed;
	float mRightSpeed;
	float mXMin;
};

