#include "Core/Layer.h"

#include "Core/Engine.h"

namespace Core {
	void Layer::QueueTransition(std::unique_ptr<Layer> p_toLayer)
	{

		auto& layerStack = Engine::Get().layerStack;

		for (auto& layer : layerStack)
		{
			if (layer.get() == this)
			{
				layer = std::move(p_toLayer);
				break;
			}
		}
	}
}