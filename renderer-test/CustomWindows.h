#ifndef WINDOWS_H
#define WINDOWS_H

#pragma once

// Window 7 or later Targeted
#define	_WIN32_WINNT 0x0601
#include <sdkddkver.h>

/*
** Disable some unused windows stuff.
** if you get errors try removing some
** (or all) of these defines
*/
#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

#define NOMINMAX

#define STRICT

#ifdef DEBUG
#define IS_DEBUG 1
#else
#define IS_DEBUG 0
#endif

#define WIDTH 1320.0f
#define HEIGHT 965.0f

#include <Windows.h>

#endif