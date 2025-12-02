#include "Core/Layer.h"

#include "Core/Application.h"

namespace Core {
	void Layer::QueueTransition(std::unique_ptr<Layer> toLayer)
	{

		auto& layerStack = Application::Get().layerStack;

		for (auto& layer : layerStack)
		{
			if (layer.get() == this)
			{
				layer = std::move(toLayer);
				break;
			}
		}
	}
}