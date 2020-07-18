/*!
 *	@file debug.hpp
 *	@brief Библиотека для упрощения отладки программ и поиска SEGFAULT
 */


#pragma once


#ifndef release
	#define DEBUG printf("\x1b[35m>> debug from <%s::%d>\n\x1b[0m", __FILE__, __LINE__);
#else
	#define DEBUG
#endif
