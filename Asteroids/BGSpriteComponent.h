#pragma once
#include "SpriteComponent.h"
#include "Actor.h"
#include <vector>

class BGSpriteComponent : public SpriteComponent {
public:
	//setting draw order lower so it's behind everything
	BGSpriteComponent(Actor* owner, int drawOrder = 10);

	//Ovverrides
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }

	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
private:
	//all our bg textures can be scrolling
	struct BGTexture {
		SDL_Texture* mtexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	float mScrollSpeed;
	Vector2 mScreenSize;
};

