#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <vector>
#include <unordered_map>
#include "SpriteComponent.h"
#include "Ship.h"

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void ShutDown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	//sprites and textures
	SDL_Texture* GetTexture(const std::string& filename);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	//Level Data
	void LoadData();
	void UnloadData();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	float GetDeltaTime(); //helper function

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	Uint32 mTicksCount;

	//Actors Data
	bool mUpdatingActors;
	std::vector<class Actor *> mActors;
	std::vector<class Actor *> mPendingActors;

	//Sprites Vars
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class SpriteComponent*> mSprites;

	//Game specific
	class Ship* mShip;
};
