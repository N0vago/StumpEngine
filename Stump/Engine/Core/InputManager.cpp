#include "Core/InputManager.h"
namespace Core {
	void InputManager::ListenEvent(Core::Event& r_event)
	{
		Core::EventDispatcher dispatcher = Core::EventDispatcher(r_event);
		dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) {
			MouseButtonPressed(e);
			currentCategory = EventCategory::EventCategoryMouse;
			return false;
			});
		dispatcher.Dispatch<Core::MouseButtonReleasedEvent>([this](Core::MouseButtonReleasedEvent& e) {
			MouseButtonReleased(e);
			currentCategory = EventCategory::EventCategoryMouse;
			return false;
			});
		dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) {
			KeyPressed(e);
			currentCategory = EventCategory::EventCategoryKeyboard;
			return false;
			});
		dispatcher.Dispatch<Core::KeyReleasedEvent>([this](Core::KeyReleasedEvent& e) {
			KeyReleased(e);
			currentCategory = EventCategory::EventCategoryKeyboard;
			return false;
			});
		dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) {
			mousePosition = Vector2((float)e.GetX(), (float)e.GetY());
			currentCategory = EventCategory::EventCategoryMouse;
			return false;
			});

		dispatcher.Dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) {
			scrollOffset = Vector2((float)e.GetXOffset(), (float)e.GetYOffset());
			currentCategory = EventCategory::EventCategoryMouse;
			return false;
			});
	}

	bool InputManager::IsMouseEvent() {
		if (currentCategory == EventCategory::EventCategoryMouse) {
			return true;
		}
		return false;
	}
	bool InputManager::IsKeyEvent() {
		if (currentCategory == EventCategory::EventCategoryKeyboard) {
			return true;
		}
		return false;
	}
}