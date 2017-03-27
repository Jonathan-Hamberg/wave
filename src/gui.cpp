#pragma once

#include <iostream>
#include <algorithm>

#include "nfd.h"
#include "imgui.h"

#include "gui.h"

#include "wav.h"


void gui_main(GLFWwindow *window, std::set<int> keys)
{
	static wav waveform;
	static float speed = 1.0f;								// Determines the speed of the waveform animation.
	static bool is_paused = true;							// Determined if the waveform animation is paused / running.
	static std::vector<float> wave_data;					// Stores the waveform float data that will be used to plot the points to the graph.
	static double cursor_begin = 0.0f;						// The cursor begin and end positions in seconds.
	static uint32_t icursor_begin, icursor_end;				// The cursor begin and end position in data points.
	static float hzoom_level = 1.0f;						// The horizontal zoom level of the displayed waveform.
	static bool show_file_open = false;						// Determines if the file open dialog is shown.
	static bool show_app_about = false;						// Determines if the about screen is shown.

	// Calculate the delta time and set the last time value in seconds.
	static double last_time = glfwGetTime();
	double delta_time = glfwGetTime() - last_time;
	last_time = glfwGetTime();

	// Get the screen size.
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	// Set up the main menu for the application.
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Open", "CTRL+O", &show_file_open);
			ImGui::MenuItem("About", nullptr, &show_app_about);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	// Increment the time timestep if the animation is running.
	if (!is_paused)
	{
		cursor_begin += delta_time * speed;
	}

	if (keys.count(GLFW_KEY_SPACE))
	{
		is_paused = !is_paused;
	}
	if (keys.count(GLFW_KEY_PAGE_UP))
	{
		speed *= 1.5;
	}
	if (keys.count(GLFW_KEY_PAGE_DOWN))
	{
		speed /= 1.5;
	}
	if (keys.count(GLFW_KEY_LEFT))
	{
		cursor_begin = std::max(double(0), cursor_begin - hzoom_level);
	}
	if (keys.count(GLFW_KEY_RIGHT))
	{
		cursor_begin = cursor_begin + hzoom_level;
	}
	if (keys.count(GLFW_KEY_KP_ADD))
	{
		hzoom_level /= 1.5;
	}
	if (keys.count(GLFW_KEY_KP_SUBTRACT))
	{
		hzoom_level *= 1.5;
	}

	// Get the sample index locations for the waveform data.
	icursor_begin = waveform.TimeToSample(cursor_begin);
	icursor_end = waveform.TimeToSample(cursor_begin + hzoom_level);

	if (icursor_begin > icursor_end)
	{
		std::swap(icursor_begin, icursor_end);
	}
	icursor_begin = std::max(icursor_begin, (uint32_t)0);
	icursor_end = std::min(icursor_end, waveform.NumSamples());

	uint32_t waveGraphWidth = uint32_t(windowWidth - 200);

	// Make sure there is enough space for all the data points.
	if (waveform.SampleRate() != 0 && wave_data.size() != waveGraphWidth)
	{
		wave_data.resize(waveGraphWidth);
	}

	// Read the waveform data from the wav file.
	waveform.GetSamples(wave_data.data(), waveGraphWidth, icursor_begin, icursor_end);


	if (show_file_open)
	{
		nfdchar_t *out_path = nullptr;
		nfdresult_t nfd_result = NFD_OpenDialog("wav", nullptr, &out_path);

		if (nfd_result == NFD_OKAY)
		{
			waveform = wav(out_path);
			free(out_path);
		}

		show_file_open = false;
	}

	if (show_app_about)
	{
		ImGui::Begin("About", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Waveform Viewer by Jonathan Hamberg");
		ImGui::Separator();
		ImGui::Text("Keyboard Shortcuts");
		ImGui::Text("  Space Bar   - Pause / Unpause playback.");
		ImGui::Text("  Page Down   - Decrease the playback speed.");
		ImGui::Text("  Page Up     - Increase the playback speed.");
		ImGui::Text("  Left Arrow  - Step back waveform.");
		ImGui::Text("  Right Arrow - Step forward waveform.");
		ImGui::Text("  + Key       - Increase the horizontal zoom.");
		ImGui::Text("  - Key       - Decrease the horizontal zoom.");
		ImGui::End();
	}

	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 19.0f));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight - 19.0f));
	if (ImGui::Begin("Waveform", nullptr, window_flags))
	{
		ImGui::PlotLines("", wave_data.data(), int(wave_data.size()), 0, nullptr, -1.0f, 1.0f, ImVec2(windowWidth, (windowHeight - 19.0f) / 2.0f));
	}
	ImGui::End();
}