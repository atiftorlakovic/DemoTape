#pragma once

// Global settings for JUCE modules
// This file should be included before any JUCE module headers

// Disable web browser
#define JUCE_WEB_BROWSER 0

// Disable CURL
#define JUCE_USE_CURL 0

// Disable VST2 replacement
#define JUCE_VST3_CAN_REPLACE_VST2 0

// Enable OpenGL
#define JUCE_OPENGL 1

// Disable some unused features to reduce binary size
#define JUCE_USE_CAMERA 0
#define JUCE_USE_MP3AUDIOFORMAT 0
#define JUCE_USE_OGGVORBIS 0 