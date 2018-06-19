#pragma once

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#endif

#ifdef DEBUG
#include <cassert>
#define ASSERT(exp, msg) assert(exp && msg)
#define ASSERT_R(exp, msg) assert(exp && msg)
#define ASSERT_B(exp, msg) assert(exp && msg)
#else
#define ASSERT(exp, msg) if (!exp) return
#define ASSERT_R(exp, msg) if (!exp) return nullptr
#define ASSERT_B(exp, msg) if (!exp) return false
#endif