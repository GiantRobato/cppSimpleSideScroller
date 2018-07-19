#include "Game.h"
#include "Actor.h"
#include <SDL/SDL_image.h>
#include "Ship.h"
#include "BGSpriteComponent.h"

Game::Game() 
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mUpdatingActors(false)
	, mIsRunning(true) {

}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to init SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Space Sidescroller",
		100, 100,
		1024, 768,
		0
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	//enables AA
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	LoadData();
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	//handle exit with esc key
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame() {
	float deltaTime = GetDeltaTime();
	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	//move pending actors
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	//clean dead actors
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	//delete list of dead actors
	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	SDL_SetRenderDrawColor(mRenderer, 0, 255, 255, 255);
	SDL_RenderClear(mRenderer);

	for (auto sprite : mSprites) {
		sprite->Draw(mRenderer);
	}

	//swap with back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::ShutDown() {
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

float Game::GetDeltaTime() {
	//rate limit delta time to 60fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	//convert to seconds
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	mTicksCount = SDL_GetTicks();

	//clamp deltaTime
	if (deltaTime > 0.05f) deltaTime = 0.05f;
	return deltaTime;
}

//actor code

// If we're updating actors, need to add to pending
void Game::AddActor(Actor* actor) {
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	} else {
		mActors.emplace_back(actor);
	}
}

//Removes actor wherever it may be in pending or in current actors
void Game::RemoveActor(Actor* actor) {
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	//Check the same thing but in current actors
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

//Util functions
SDL_Texture* Game::GetTexture(const std::string& fileName) {
	SDL_Texture* tex = nullptr;

	//if we already have the texture, return it
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end()) {
		tex = iter->second;
		return tex;
	}

	//Otherwise load the image into a surf and texture
	SDL_Surface* surf = IMG_Load(fileName.c_str());
	if (!surf) {
		SDL_Log("Failed to load texture file %s", fileName.c_str());
		return nullptr;
	}

	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	if (!tex) {
		SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
		return nullptr;
	}

	mTextures.emplace(fileName, tex);
	return tex;
}

void Game::AddSprite(class SpriteComponent* sprite) {
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for ( ; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(class SpriteComponent* sprite) {
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Game::LoadData() {
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(75.0f, 384.0f));
	mShip->SetRotation(Math::PiOver2);
	mShip->SetScale(0.25f);

	//Create background items
	Actor* bgActor = new Actor(this);
	bgActor->SetPosition(Vector2(512.0f, 384.0f));
	//set main space bg
	BGSpriteComponent* bg = new BGSpriteComponent(bgActor);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgTexes = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/FarBack02.png")
	};
	bg->SetBGTextures(bgTexes);
	bg->SetScrollSpeed(-100.0f);

	//set accent stars bg
	bg = new BGSpriteComponent(bgActor);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgTexes = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgTexes);
	bg->SetScrollSpeed(-200.0f);
	
}

void Game::UnloadData() {
	//delete all actors
	while (!mActors.empty()) {
		delete mActors.back();
	}

	//free textures
	for (auto iter : mTextures) {
		SDL_DestroyTexture(iter.second);
	}
	mTextures.clear();
}