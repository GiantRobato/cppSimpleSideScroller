#include "BGSpriteComponent.h"
#include <memory>


BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder),
	mScrollSpeed(0.0f){
}


void BGSpriteComponent::Update(float deltaTime) {
	for (auto& bg : mBGTextures) {
		bg.mOffset.x += mScrollSpeed * deltaTime;
		//loop bg scene
		if (bg.mOffset.x < -mScreenSize.x) {
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer) {
	for (auto& bg : mBGTextures) {
		SDL_Rect r;
		//set width/height
		r.w = static_cast<int>(mScreenSize.x);
		r.h = static_cast<int>(mScreenSize.y);
		//center rect based on offset
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 + bg.mOffset.x);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 + bg.mOffset.y);

		//Draw it!
		SDL_RenderCopy(renderer, bg.mtexture, nullptr, &r);
	}
}

//set textures
void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures) {
	int count = 0;
	for (auto tex : textures) {
		BGTexture tmp;
		tmp.mtexture = tex;
		tmp.mOffset.x = count * mScreenSize.x;
		tmp.mOffset.y = 0;
		mBGTextures.emplace_back(tmp);
		count++;
	}
}