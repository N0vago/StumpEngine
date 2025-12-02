#ifndef ST_LAYER_H
#define ST_LAYER_H

#include <memory>

#include "Core/Event.h"

namespace Core {
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}

		template<std::derived_from<Layer> T, typename... Args>
		void TransitionTo(Args&&... args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}
	private:
		void QueueTransition(std::unique_ptr<Layer> layer);

	};
	#endif // ST_LAYER_H
}