#ifndef ST_INPUT_EVENTS_H
#define ST_INPUT_EVENTS_H

#include "Core/Event.h"

#include <format>

namespace Core {
	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return keyCode; }
	protected:
		KeyEvent(int p_keycode)
			: keyCode(p_keycode) {
		}

		int keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int p_keycode, bool p_isRepeat)
			: KeyEvent(p_keycode), isRepeat(p_isRepeat) {
		}

		inline bool IsRepeat() const { return isRepeat; }

		std::string ToString() const override
		{
			return std::format("KeyPressedEvent: {} (repeat={})", keyCode, isRepeat);
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool isRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int p_keycode)
			: KeyEvent(p_keycode) {
		}

		std::string ToString() const override
		{
			return std::format("KeyReleasedEvent: {}", keyCode);
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double p_x, double p_y)
			: mouseX(p_x), mouseY(p_y) {
		}

		inline double GetX() const { return mouseX; }
		inline double GetY() const { return mouseY; }

		std::string ToString() const override
		{
			return std::format("MouseMovedEvent: {}, {}", mouseX, mouseY);
		}

		EVENT_CLASS_TYPE(MouseMoved)
	private:
		double mouseX, mouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double p_xOffset, double p_yOffset)
			: xOffset(p_xOffset), yOffset(p_yOffset) {
		}

		inline double GetXOffset() const { return xOffset; }
		inline double GetYOffset() const { return yOffset; }

		std::string ToString() const override
		{
			return std::format("MouseScrolledEvent: {}, {}", xOffset, yOffset);
		}

		EVENT_CLASS_TYPE(MouseScrolled)
	private:
		double xOffset, yOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return button; }
	protected:
		MouseButtonEvent(int p_button)
			: button(p_button) {
		}

		int button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int p_button)
			: MouseButtonEvent(p_button) {
		}

		std::string ToString() const override
		{
			return std::format("MouseButtonPressedEvent: {}", button);
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int p_button)
			: MouseButtonEvent(p_button) {
		}

		std::string ToString() const override
		{
			return std::format("MouseButtonReleasedEvent: {}", button);
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
#endif // ST_INPUT_EVENTS_H