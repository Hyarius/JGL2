#pragma once

#ifdef _WIN32
#define NOMINMAX
#endif // _WIN32

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

#include <algorithm>

#include <string>
#include <deque>
#include <vector>
#include <map>

#include <windows.h>

#define GLEW_STATIC
#define WGL_WGLEXT_PROTOTYPES
#include "GL/glew.h"
#include "GL/glut.h"
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/wglew.h>
#pragma comment(lib, "opengl32.lib")