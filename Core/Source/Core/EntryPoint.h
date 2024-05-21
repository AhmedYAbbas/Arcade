#pragma once

int main(int argc, char** argv)
{
	const auto app = Core::CreateApplication();
	app->Run();
	delete app;

	return 0;
}