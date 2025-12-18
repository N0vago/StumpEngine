#ifndef ST_LAYER_H
#define ST_LAYER_H

#include "Core/Event.h"

#include <memory>

namespace Core {
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& r_event) {}

		virtual void OnUpdate(float p_ts) {}
		virtual void OnRender() {}

		template<std::derived_from<Layer> T, typename... Args>
		void TransitionTo(Args&&... r_args)
		{
			QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(r_args)...)));
		}
	private:
		void QueueTransition(std::unique_ptr<Layer> p_toLayer);

	};
}
#endif // ST_LAYER_H