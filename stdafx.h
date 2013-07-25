// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#ifdef WIN32
  #include <SDL.h>
  #include <SDL_ttf.h>
  #include <SDL_image.h>
  #include <SDL_opengl.h>
  #include <SDL_audio.h>
#else
  #include <SDL/SDL.h>
  #include <SDL_ttf/SDL_ttf.h>
  #include <SDL_image/SDL_image.h>
  #include <SDL/SDL_opengl.h>
  #include <SDL/SDL_audio.h>
#endif


#include <cstdlib>     
#include <ctime> 
#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <map>

#undef min
#undef max

#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/clamp.hpp>
#include <boost/algorithm/minmax.hpp>
#include <boost/algorithm/minmax_element.hpp>

#include "Core/CommonMacros.h"
#include "Core/StrFormat.h"
#include "Core/Log.h"
#include "Core/Point.hpp"
#include "Core/Rect.hpp"



// TODO: reference additional headers your program requires here
