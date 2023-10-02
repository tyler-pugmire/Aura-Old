#pragma once

#include "Base.h"
#include "Aura/Renderer/RenderContext.h"

#include <string>
#include <functional>
#include <unordered_map>


namespace
{
	struct WindowImpl;
}

union SDL_event;

namespace Aura
{
	class Event;
	class RenderContext;

	struct WindowSpecification
	{
		std::string title = "Aura Editor";
		uint32_t width = 1600;
		uint32_t height = 900;
		bool decorated = true;
		bool fullscreen = false;
		bool vSync = false;
	};

	class Window : RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}

		virtual void Init() = 0;
		virtual void ProcessEvents() = 0;
		virtual void SwapBuffer() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
		virtual std::pair<float, float> GetWindowPos() const = 0;

		virtual void Maximize() = 0;
		virtual void CenterWindow() = 0;

		virtual void SetEventCallback(EventCallbackFn const& fn) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetResizable(bool resizable) const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual Ref<RendererContext> GetRenderContext() { return m_rendererContext; }
		//virtual VulkanSwapChain& GetSwapChain() = 0;

		static Window* Create(const WindowSpecification& specification);
	protected:
		Ref<RendererContext> m_rendererContext;
	};
}