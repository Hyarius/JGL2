#pragma once

#ifdef _WIN32
#define NOMINMAX
#endif // _WIN32

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <mutex>

#include <algorithm>
#include <functional>

#include <string>
#include <deque>
#include <vector>
#include <map>
#include <set>
#include <variant>

#include <random>

#include "jgl2_dirent.h"

#define _CRT_RAND_S
#include <stdlib.h>

#include <windows.h>

#define GLEW_STATIC
#define WGL_WGLEXT_PROTOTYPES
#include "GL/glew.h"
#include "GL/glut.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/wglew.h>
#pragma comment(lib, "opengl32.lib")

#include <bitset>

#define ASIO_STANDALONE
#include <ExternalLibraries/asio.hpp>
#include <ExternalLibraries/asio/ts/buffer.hpp>
#include <ExternalLibraries/asio/ts/internet.hpp>

#include <ExternalLibraries/glm/glm.hpp>
#include <ExternalLibraries/glm/ext.hpp>