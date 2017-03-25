#pragma once

#include <iostream>
#include <algorithm>

#include "nfd.h"
#include "imgui.h"

#include "gui.h"

#include "wav.h"

void gui_menu()
{

}

void gui_main(GLFWwindow *window)
{
	static wav waveform;
	static float speed = 1.0f;								// Determines the speed of the waveform animation.
	static bool is_paused = true;							// Determined if the waveform animation is paused / running.
	static std::vector<float> wave_data;					// Stores the waveform float data that will be used to plot the points to the graph.
	static float cursor_begin = 0.0f, cursor_end = 2.0f;	// The cursor begin and end positions in seconds.
	static uint32_t icursor_begin, icursor_end;				// The cursor begin and end position in data points.
	static float hzoom_level = 1.0f;						// The horizontal zoom level of the displayed waveform.
	static float speed_level = 1.0f;						// The speed multiplier for the waveform.

	// Set up the main menu for the application.
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t *out_path = nullptr;
				nfdresult_t nfd_result = NFD_OpenDialog("wav", nullptr, &out_path);

				if (nfd_result == NFD_OKAY)
				{
					waveform = wav(out_path);
					free(out_path);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	
	// Get the sample index locations for the waveform data.
	icursor_begin = waveform.TimeToSample(cursor_begin);
	icursor_end = waveform.TimeToSample(cursor_end);
	if (icursor_begin > icursor_end)
	{
		std::swap(icursor_begin, icursor_end);
	}
	icursor_begin = std::max(icursor_begin, (uint32_t)0);
	icursor_end = std::min(icursor_end, waveform.NumSamples());

	// Calculate the delta time and set the last time value in seconds.
	static double last_time = glfwGetTime();
	double deltaTime = glfwGetTime() - last_time;
	last_time = glfwGetTime();

	// Make sure there is enough space for all the data points.
	if (wave_data.capacity() < icursor_end - icursor_begin)
	{
		wave_data.resize(icursor_end - icursor_begin);
	}

	// Read the waveform data from the wav file.
	waveform.GetSamples(wave_data.data(), icursor_end - icursor_begin, icursor_begin);



	ImGuiWindowFlags window_flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse;

	// Get the screen size.
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	ImGui::SetNextWindowPos(ImVec2(0.0f, 19.0f));
	ImGui::SetNextWindowSize(ImVec2(200.0f, windowHeight - 19.0f));
	if (ImGui::Begin("Settings", nullptr, window_flags))
	{
		ImGui::DragFloat("speed", &speed, 0.005f, 1.0f, 10000.0f, "%0.0f", 10.0f);

		if (ImGui::Button("Run/Pause"))
		{
			is_paused = !is_paused;
		}

		if (ImGui::Button("Step Forward"))
		{

		}

		if (ImGui::Button("Step Back"))
		{

		}
	}
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(200.0f, 19.0f));
	ImGui::SetNextWindowSize(ImVec2(windowWidth - 200.0f, windowHeight - 19.0f));
	if (ImGui::Begin("Waveform", nullptr, window_flags))
	{
		ImGui::PlotLines("", wave_data.data(), icursor_end - icursor_begin, 0, nullptr, -1.0f, 1.0f, ImVec2(windowWidth - 200, (windowHeight - 19.0f) / 2.0f));
	}
	ImGui::End();
}