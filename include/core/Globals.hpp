#ifndef FAG_CORE_GLOBALS_HPP
#define FAG_CORE_GLOBALS_HPP

#include "./Engine.hpp"
#include "./Renderer.hpp"

#define g_Engine fag::Engine::get_singleton()
#define g_Renderer g_Engine->get_renderer()

#endif // FAG_CORE_GLOBALS_HPP
