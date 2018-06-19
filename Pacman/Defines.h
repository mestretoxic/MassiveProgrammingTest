#pragma once

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#endif

#ifdef DEBUG
#include <cassert>
#define ASSERT(exp, msg) assert(exp && msg)
#define ASSERT_NULL(exp, msg) assert(exp && msg)
#define ASSERT_BOOL(exp, msg) assert(exp && msg)
#else
#define ASSERT(exp, msg) if (!exp) return
#define ASSERT_NULL(exp, msg) if (!exp) return nullptr
#define ASSERT_BOOL(exp, msg) if (!exp) return false
#endif