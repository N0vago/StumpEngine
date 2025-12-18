#ifndef ST_ INPUT_MANAGER_H
#define ST_INPUT_MANAGER_H

#include "Core/Event.h"
#include "Core/InputEvents.h"
#include "Math/Vector2.h"
using namespace Math;
namespace Core {
	class InputManager
	{
		bool keys[512];

		bool mouseButtons[8];

		Vector2 mousePosition;

		Vector2 scrollOffset;
	public:

		InputManager() = default;

		~InputManager() = default;

		void ListenEvent(Core::Event& r_event);

		bool IsKeyPressed(int p_keycode) const { return keys[p_keycode]; }

		bool IsMouseButtonPressed(int p_buttoncode) const { return mouseButtons[p_buttoncode]; }

		Vector2 GetMousePosition() const { return mousePosition; }

		Vector2 GetScrollOffset() const { return scrollOffset; }

	private:
		void KeyPressed(Core::KeyPressedEvent& event) { keys[event.GetKeyCode()] = true; }
		void KeyReleased(Core::KeyReleasedEvent& event) { keys[event.GetKeyCode()] = false; }

		void MouseButtonPressed(Core::MouseButtonPressedEvent& event) { mouseButtons[event.GetMouseButton()] = true; }
		void MouseButtonReleased(Core::MouseButtonReleasedEvent& event) { mouseButtons[event.GetMouseButton()] = false; }

	};
}
#endif // ST_INPUT_MANAGER_H

