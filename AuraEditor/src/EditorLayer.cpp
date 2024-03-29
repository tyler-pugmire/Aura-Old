#include "EditorLayer.h"
#include "imgui_internal.h"
#include "imgui.h"
#include "Aura/ImGui/ImGuiColors.h"
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

	return titlebarHeight;
}

void EditorLayer::HandleManualWindowResize()
{

}