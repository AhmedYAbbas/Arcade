#include <Core.h>
#include <Core/EntryPoint.h>

#include "Scenes/ArcadeScene.h"

class ArcadeApp : public Core::Application
{
public:
	ArcadeApp()
	{
		PushScene(std::make_unique<ArcadeScene>());
	}
};

Core::Application* Core::CreateApplication()
{
	return new ArcadeApp();
}