#pragma once

#include <functional>

namespace Core
{

	using InputKey = uint8_t;
	using InputState = uint8_t;
	using MouseButton = uint8_t;

	using KeyboardInputAction = std::function<void(uint32_t dt, InputState state)>;

	struct KeyboardButtonAction
	{
		InputKey Key;
		KeyboardInputAction Action;
	};

	struct MousePosition
	{
		int32_t X, Y;
	};

	using MouseMovedAction = std::function<void(const MousePosition& mousePosition)>;
	using MouseInputAction = std::function<void(InputState state, const MousePosition& position)>;

	struct MouseButtonAction
	{
		MouseButton MouseButton;
		MouseInputAction MouseInputAction;
	};
}