#include "InputManager.h"

void InputManager::ListenEvent(Core::Event& r_event)
{
	Core::EventDispatcher dispatcher = Core::EventDispatcher(r_event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) {
		MouseButtonPressed(e);
		return false;
		});
	dispatcher.Dispatch<Core::MouseButtonReleasedEvent>([this](Core::MouseButtonReleasedEvent& e) {
		MouseButtonReleased(e);
		return false;
		});
	dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) {
		KeyPressed(e);
		return false;
		});
	dispatcher.Dispatch<Core::KeyReleasedEvent>([this](Core::KeyReleasedEvent& e) {
		KeyReleased(e);
		return false;
		});
	dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) {
		mousePosition = Vector2((float)e.GetX(), (float)e.GetY());
		return false;
		});

	dispatcher.Dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) {
		scrollOffset = Vector2((float)e.GetXOffset(), (float)e.GetYOffset());
		return false;
		});
}
