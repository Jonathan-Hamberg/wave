#pragma once

#include <GLFW/glfw3.h>

/*
 * Documentation for the GUI panel.
 * Controls for the waveform viewer.
 * Page Up     - increase the playback speed.
 * Page Down   - decrease the playback speed.
 * Spacebar    - pause / resume playback.
 * Left Arrow  - step waveform back.
 * Right Arrow - step waveform forward.
 * + Key - Zoom the waveform in.
 * - Key - Zoom the waveform out.
*/
void gui_main(GLFWwindow *window);