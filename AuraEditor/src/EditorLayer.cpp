#include "EditorLayer.h"
#include "imgui_internal.h"
#include "imgui.h"
#include "Aura/ImGui/ImGuiColors.h"
#include "SDL/SDL.h"
#include "Aura/Core/Application.h"
#include "Aura/Core/Window.h"
#include "Aura/ImGui/ImGuiUtilites.h"

static bool show = true;
EditorLayer::EditorLayer() : Layer("Sandbox Layer")
{

}

void EditorLayer::OnImGuiRender()
{
	static bool opt_fullscreen_persistant = true;
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;

	ImGuiWindowFlags window_flags =/* ImGuiWindowFlags_MenuBar |*/ ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	ImGui::PopStyleColor(); // MenuBarBg
	ImGui::PopStyleVar(2);

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	HandleManualWindowResize();
	const float titlebarHeight = DrawTitlebar();

	ImGui::SetCursorPosY(titlebarHeight + ImGui::GetCurrentWindow()->WindowPadding.y);

	// Dockspace
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}
	ImGui::End();
}

float EditorLayer::DrawTitlebar()
{
	const float titlebarHeight = 57.0f;
	const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

	ImGui::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y));
	const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
	const ImVec2 titlebarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - windowPadding.y * 2.0f,
								 ImGui::GetCursorScreenPos().y + titlebarHeight };
	auto* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(titlebarMin, titlebarMax, Colors::Theme::titlebar);

	//ImGui::BeginHorizontal("Titlebar", { ImGui::GetWindowWidth() - windowPadding.y * 2.0f, ImGui::GetFrameHeightWithSpacing() });
	static float moveOffsetX;
	static float moveOffsetY;
	const float w = ImGui::GetContentRegionAvail().x;
	const float buttonsAreaWidth = 94;


	auto* rootWindow = ImGui::GetCurrentWindow()->RootWindow;
	const float windowWidth = (int)rootWindow->RootWindow->Size.x;

	if (ImGui::InvisibleButton("##titleBarDragZone", ImVec2(w - buttonsAreaWidth, titlebarHeight), ImGuiButtonFlags_PressedOnClick))
	{
		ImVec2 point = ImGui::GetMousePos();
		ImRect rect = rootWindow->Rect();
		// Calculate the difference between the cursor pos and window pos
		moveOffsetX = point.x - rect.Min.x;
		moveOffsetY = point.y - rect.Min.y;
	}

	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
	{
		auto* window = static_cast<SDL_Window*>(Aura::Application::Get().GetWindow()->GetNativeWindow());
		bool maximized = SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED;
		if (maximized)
			SDL_RestoreWindow(window);
		else
			Aura::Application::Get().GetWindow()->Maximize();
	}
	else if (ImGui::IsItemActive())
	{
		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			auto* window = static_cast<SDL_Window*>(Aura::Application::Get().GetWindow()->GetNativeWindow());
			bool maximized = SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED;

			if (maximized)
			{
				SDL_RestoreWindow(window);

				int newWidth, newHeight;
				SDL_GetWindowSize(window, &newWidth, &newHeight);
				if (windowWidth - (float)newWidth > 0.0f)
					moveOffsetX *= (float)newWidth / windowWidth;
			}

			ImVec2 point = ImGui::GetMousePos();
			SDL_SetWindowPosition(window, point.x - moveOffsetX, point.y - moveOffsetY);
		}
	}


	return titlebarHeight;
}

void EditorLayer::HandleManualWindowResize()
{
	auto* window = static_cast<SDL_Window*>(Aura::Application::Get().GetWindow()->GetNativeWindow());
	bool maximized = SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED;

	ImVec2 newSize, newPosition;
	if (!maximized && UI::UpdateWindowManualResize(ImGui::GetCurrentWindow(), newSize, newPosition))
	{
		SDL_SetWindowPosition(window, newPosition.x, newPosition.y);
		SDL_SetWindowSize(window, newSize.x, newSize.y);
	}
}