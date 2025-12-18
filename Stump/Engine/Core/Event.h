#ifndef ST_EVENT_H
#define ST_EVENT_H

#include <string>
#include <functional>

namespace Core
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }

	class Event
	{
	public:
		bool Handled = false;
		virtual ~Event() {}
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
	};


	class EventDispatcher
	{
		template<typename T>
		using EventFunc = std::function<bool(T&)>;
	public:

		EventDispatcher(Event& r_event) : event(r_event) {}

		template<typename T>
		bool Dispatch(EventFunc<T> p_func)
		{
			if (event.GetEventType() == T::GetStaticType())
			{
				event.Handled = p_func(static_cast<T&>(event));
				return true;
			}
			return false;
		}
	private:
		Event& event;
	};
}
#endif // ST_EVENT_H