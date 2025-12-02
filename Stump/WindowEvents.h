#ifndef ST_WINDOW_EVENTS_H
#define ST_WINDOW_EVENTS_H

#include "Core/Event.h"

#include <format>

namespace Core {
	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t p_width, uint32_t p_height)
			: width(p_width), height(p_height) {
		}

		inline uint32_t GetWidth() const { return width; }
		inline uint32_t GetHeight() const { return height; }

		std::string ToString() const override
		{
			return std::format("WindowResizeEvent: {}, {}", width, height);
		}

		EVENT_CLASS_TYPE(WindowResize)
	private:
		uint32_t width, height;
	};

#endif // ST_WINDOW_EVENTS_H
}