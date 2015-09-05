#pragma once

class TestAZDOApp
{
public:
	TestAZDOApp(uint32_t screenWidth, uint32_t screenHeight, LinearAllocator &allocator, ScopeStack &initStack);
	~TestAZDOApp();

	void Update();
	void Render();
private:

};
