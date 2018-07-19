#include "Ship.h"
#include "SpriteComponent.h"
#include "Game.h"

Ship::Ship(Game * game)
	: mDownSpeed(0),
	mRightSpeed(0),
	mXMin(40),
	Actor(game){
	SpriteComponent* sprite = new SpriteComponent(this, 150);	
	sprite->SetTexture(game->GetTexture("Assets/Ship.png"));
}

void Ship::UpdateActor(float deltaTime) {
	Vector2 pos = this->GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;

	// Limit position to left portion of screen
	if (pos.x < mXMin) {
		pos.x = mXMin;
	} else if (pos.x > 500.0f) {
		pos.x = 500.0f;
	}

	if (pos.y < 25.0f) {
		pos.y = 25.0f;
	} else if (pos.y > 743.0f) {
		pos.y = 743.0f;
	}

	SetPosition(pos);
}

void Ship::ActorInput(const uint8_t* keyState) {
	mDownSpeed = 0;
	mRightSpeed = 0;
	if (keyState[SDL_SCANCODE_W]) {
		mDownSpeed = -200;
	}
	if (keyState[SDL_SCANCODE_S]) {
		mDownSpeed = 200;
	}
	if (keyState[SDL_SCANCODE_A]) {
		mRightSpeed = -200;
	}
	if (keyState[SDL_SCANCODE_D]) {
		mRightSpeed = 200;
	}

}
