// myodd.h : main header file for the myodd dll
//

#pragma once

#ifndef __AFXWIN_H__


#ifdef _DEBUG
#define UNUSED(x)
#else
#define UNUSED(x) x
#endif
#define UNUSED_ALWAYS(x) x

#include <assert.h>
#ifdef _DEBUG
#define ASSERT(f) assert(f)
#else
#define ASSERT(f) __noop
#endif

#endif  //  __AFXWIN_H__