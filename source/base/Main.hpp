#pragma once

// C

#include <stdint.h>
#include <time.h>

// STL

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#if defined(WIN32) || defined(_WINDOWS)
	#include <windows.h>
	#include <ShlObj.h>
#endif

// OpenGL

#include <GL/glew.h>

// GLFW

#include <GLFW/glfw3.h>

// GLM

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeType

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftbbox.h>

// utf8rewind

#include <utf8rewind.h>

// Tools

#include "base/Utils.hpp"
#include "logging/Logger.hpp"