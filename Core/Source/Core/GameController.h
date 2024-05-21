#pragma once

#include <vector>

#include "InputAction.h"

namespace Core
{
	class GameController
	{
	public:
		GameController();

		const KeyboardInputAction GetKeyboardInputAction(InputKey key) const;
		void AddKeyboardButtonAction(const KeyboardButtonAction& buttonAction);
		void Clear();

		static bool IsPressed(InputState state);
		static bool IsReleased(InputState state);
		static InputKey ActionKey();
		static InputKey CancelKey();
		static InputKey LeftKey();
		static InputKey RightKey();
		static InputKey UpKey();
		static InputKey DownKey();

		inline const MouseMovedAction& GetMouseMovedAction() const { return m_MouseMovedAction; }
		inline void SetMouseMovedAction(const MouseMovedAction& mouseMovedAction) { m_MouseMovedAction = mouseMovedAction; }

		const MouseInputAction GetMouseInputAction(MouseButton button) const;
		void AddMouseButtonAction(const MouseButtonAction& mouseButtonAction);

		static MouseButton LeftMouseButton();
		static MouseButton RightMouseButton();

	private:
		std::vector<KeyboardButtonAction> m_KeyboardButtonActions;
		std::vector<MouseButtonAction> m_MouseButtonActions;
		MouseMovedAction m_MouseMovedAction;
	};
}