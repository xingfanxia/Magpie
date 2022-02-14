#include "pch.h"
#include "UIDrawer.h"
#include "App.h"
#include "DeviceResources.h"
#include <imgui.h>
#include "imgui_impl_magpie.h"
#include "imgui_impl_dx11.h"
#include "Renderer.h"
#include "CursorDrawer.h"
#include "GPUTimer.h"


UIDrawer::~UIDrawer() {
	if (_handlerID != 0) {
		App::GetInstance().UnregisterWndProcHandler(_handlerID);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplMagpie_Shutdown();
	ImGui::DestroyContext();
}

float GetDpiScale() {
	return GetDpiForWindow(App::GetInstance().GetHwndHost()) / 96.0f;
}

bool UIDrawer::Initialize(ID3D11Texture2D* renderTarget) {
	auto& dr = App::GetInstance().GetDeviceResources();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavNoCaptureKeyboard | ImGuiConfigFlags_NoMouseCursorChange;
	//io.IniFilename = nullptr;

	float dpiScale = GetDpiScale();

	ImGui::StyleColorsDark();
	ImGui::GetStyle().WindowRounding = 6;
	ImGui::GetStyle().FrameBorderSize = 1;
	ImGui::GetStyle().ScaleAllSizes(dpiScale);

	io.Fonts->AddFontFromFileTTF(".\\assets\\NotoSansSC-Regular.otf", std::floor(20.0f * dpiScale), NULL);
	//ImFont* font = io.Fonts->AddFontFromMemoryTTF(();
	//font->FontSize = std::floor(ImGui::GetFont()->FontSize * 1.5f);
	//ImGui::PushFont(font);

	ImGui_ImplMagpie_Init(App::GetInstance().GetHwndHost());
	ImGui_ImplDX11_Init(dr.GetD3DDevice(), dr.GetD3DDC());

	dr.GetRenderTargetView(renderTarget, &_rtv);

	_handlerID = App::GetInstance().RegisterWndProcHandler(_WndProcHandler);

	return true;
}

void DrawUI() {
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::Text(fmt::format("FPS: {}", App::GetInstance().GetRenderer().GetGPUTimer().GetFramesPerSecond()).c_str());

	ImGui::End();
}

void UIDrawer::Draw() {
	ImGui_ImplMagpie_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();

	if (ImGui::GetIO().WantCaptureMouse) {
		if (!_cursorOnUI) {
			HWND hwndHost = App::GetInstance().GetHwndHost();
			LONG_PTR style = GetWindowLongPtr(hwndHost, GWL_EXSTYLE);
			SetWindowLongPtr(hwndHost, GWL_EXSTYLE, style & ~WS_EX_TRANSPARENT);

			_cursorOnUI = true;
		}
	} else {
		if (_cursorOnUI) {
			HWND hwndHost = App::GetInstance().GetHwndHost();
			LONG_PTR style = GetWindowLongPtr(hwndHost, GWL_EXSTYLE);
			SetWindowLongPtr(hwndHost, GWL_EXSTYLE, style | WS_EX_TRANSPARENT);

			_cursorOnUI = false;
		}
	}

	bool show = true;
	DrawUI();

	ImGui::Render();

	App::GetInstance().GetDeviceResources().GetD3DDC()->OMSetRenderTargets(1, &_rtv, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool UIDrawer::_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	ImGui_ImplMagpie_WndProcHandler(hWnd, msg, wParam, lParam);
	return false;
}