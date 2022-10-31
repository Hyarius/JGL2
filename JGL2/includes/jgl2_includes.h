#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

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