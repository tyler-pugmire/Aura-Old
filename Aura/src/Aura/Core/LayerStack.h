#pragma once

#include <vector>

namespace Aura
{
	class Layer;
    class LayerStack
    {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		Layer* operator[](size_t index)
		{
			return layers[index];
		}

		const Layer* operator[](size_t index) const
		{
			return layers[index];
		}

		size_t Size() const { return layers.size(); }

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }
	private:
		std::vector<Layer*> layers;
		unsigned int layerInsertIndex = 0;
    };
}