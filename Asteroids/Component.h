#pragma once
#include <cstdint>

class Component {
public:
	//lower update order gets updated first
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keyboardState) {}
	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	//parent
	class Actor* mOwner;
	int mUpdateOrder;
};

