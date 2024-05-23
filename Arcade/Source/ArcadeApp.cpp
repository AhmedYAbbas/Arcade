#include <Core.h>
#include <Core/EntryPoint.h>

#include "Scenes/ArcadeScene.h"
#include "Breakout/Scenes/BreakoutScene.h"

class ArcadeApp : public Core::Application
{
public:
	ArcadeApp()
	{
		PushScene(std::make_unique<ArcadeScene>());
		//PushScene(std::make_unique<BreakoutScene>());
	}
};

Core::Application* Core::CreateApplication()
{
	return new ArcadeApp();
}