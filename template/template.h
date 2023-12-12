// Template, IGAD version 3, Raspberry PI 4 version
// Get the latest version from: https://github.com/jbikker/tmpl8pi
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

// default screen resolution
#define SCRWIDTH	640
#define SCRHEIGHT	480
#define  CENTER_X  SCRWIDTH/ 2.0f
#define  CENTER_Y  SCRHEIGHT/ 2.0f


// allow NEON code
#define USE_NEON_SIMD	1

// constants
#define PI			3.14159265358979323846264f
#define TO_RADIANS			PI/180.0f
#define INVPI		0.31830988618379067153777f
#define INV2PI		0.15915494309189533576888f
#define TWOPI		6.28318530717958647692528f
#define SQRT_PI_INV	0.56418958355f
#define LARGE_FLOAT	1e34f

// detect windows build
#ifdef _MSC_VER
#define WINBUILD	1
#else
#define WINBUILD	0
#endif

// basic types
using uchar = unsigned char;
using uint = unsigned int;
using ushort = unsigned short;
#if WINBUILD == 0
using LONG = long;
#endif

// generic includes
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <chrono>
#ifdef _WINDOWS

// low-level: aligned memory allocations
#ifdef _MSC_VER
#define ALIGN( x ) __declspec( align( x ) )
#define MALLOC64( x ) ( ( x ) == 0 ? 0 : _aligned_malloc( ( x ), 64 ) )
#define FREE64( x ) _aligned_free( x )
#else
#define ALIGN( x ) __attribute__( ( aligned( x ) ) )
#define MALLOC64( x ) ( ( x ) == 0 ? 0 : aligned_alloc( 64, ( x ) ) )
#define FREE64( x ) free( x )
#endif
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define CHECK_RESULT __attribute__ ((warn_unused_result))
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
#define CHECK_RESULT _Check_return_
#else
#define CHECK_RESULT
#endif

// math classes

// template headers
//#include "surface.h"
//#include "sprite.h"
//my header
#include <Audio/Sound.hpp>
#include <json.hpp>
//my old project files
//#include "Scripts/Utilities/StaticArray.h"
//#include "Scripts/Utilities/DynamicArray.h"
//#include "Scripts/Utilities/AABB.h"
//
//#include "Scripts/Utilities/SavingLoading.h"
//#include "Scripts/Utilities/Entity.h"
////observer
//#include "Scripts/Observer/Observer.h"
//#include "Scripts/Observer/Subject.h"
//
////ui
//#include "Scripts/Player/HealthBar.h"
//
////components
//#include "Scripts/Utilities/CollisionChecker.h"
//#include "Scripts/Utilities/WinCondition.h"
//#include "Scripts/Utilities/Health.h"
//#include "Scripts/Map/Tilemap.h"
//#include "Scripts/Utilities/MathLibrary.h"
//#include "Scripts/Player/Camera.h"
//#include "Scripts/Map/Tilemap.h"
//#include "Scripts/Player/Avatar.h"
//#include "Scripts/Map/Parallax.h"
//#include "Scripts/Player/Rock.h"
//#include "Scripts/Player/SpawnRocks.h"
//#include "Scripts/Utilities/Score.h"
//#include "Scripts/Utilities/PlayerScore.h"
//#include "Scripts/Utilities/CoinScore.h"
//#include "Scripts/Utilities/GameStateManager.h"
////non tile
//#include "Scripts/Map/Non-Tileables/NonTileMap.h"
//#include "Scripts/Map/Non-Tileables/Rope.h"
//#include "Scripts/Map/Non-Tileables/Zipline.h"
//#include "Scripts/Map/Non-Tileables/ElasticPlant.h"
//#include "Scripts/Map/Non-Tileables/Coin.h"
//#include "Scripts/Map/Non-Tileables/Checkpoint.h"
//
////FSM
//#include "Scripts/Player/States//PlayerState.h"
//#include "Scripts/Player/States/ClimbingState.h"
//#include "Scripts/Player/States/SwingingState.h"
//#include "Scripts/Player/States/FreemovingState.h"
//#include "Scripts/Player/States/ZipliningState.h"
//#include  "Scripts/Player/States/ElasticPlantState.h"
////enemies
//#include  "Scripts/Enemies/EnemyStates/MonkeyState.h"
//#include  "Scripts/Enemies/EnemyStates/MonkeyTurnState.h"
//#include  "Scripts/Enemies/EnemyStates/MonkeyOnLadderState.h"
//#include  "Scripts/Enemies/EnemyStates/MonkeyPatrolState.h"
//#include  "Scripts/Enemies/EnemyStates/MonkeyThrowState.h"
//#include  "Scripts/Enemies/EnemyStates/MonkeyToGroundState.h"
//
//#include "Scripts/Enemies/EnemyStates/BoarState.h"
//#include "Scripts/Enemies/EnemyStates/BoarPatrolState.h"
//#include "Scripts/Enemies/EnemyStates/BoarTurnState.h"
//#include "Scripts/Enemies/EnemyStates/BoarStopState.h"
//
//#include "Scripts/Enemies/Enemy.h"
//#include  "Scripts/Enemies/Boar.h"
//#include  "Scripts/Enemies/Monkey.h"
//#include "Scripts/Enemies/Spike.h"
//#include "Scripts/Enemies/MonkeyBall.h"
//

// namespaces
//using namespace Tmpl8;

// clang-format off
// basic types
using uchar = unsigned char;
using uint = unsigned int;
using ushort = unsigned short;
// windows.h: disable a few things to speed up compilation.
#define NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#endif
#define NOGDICAPMASKS
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
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
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NOIME
#include "windows.h"

// OpenCL headers
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS // safe; see https://stackoverflow.com/a/28500846
#include "cl/cl.h"
#include <cl/cl_gl_ext.h>

// GLFW
#define GLFW_USE_CHDIR 0
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// zlib
#include "zlib.h"
// low-level: instruction set detection
#ifdef _WIN32
#define cpuid(info, x) __cpuidex(info, x, 0)
#else
#include <cpuid.h>
void cpuid(int info[4], int InfoType) { __cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]); }
#endif


//
//class CPUCaps // from https://github.com/Mysticial/FeatureDetector
//{
//public:
//	static inline bool HW_MMX = false, HW_x64 = false, HW_ABM = false, HW_RDRAND = false;
//	static inline bool HW_BMI1 = false, HW_BMI2 = false, HW_ADX = false, HW_PREFETCHWT1 = false;
//	// SIMD: 128-bit
//	static inline bool HW_SSE = false, HW_SSE2 = false, HW_SSE3 = false, HW_SSSE3 = false;
//	static inline bool HW_SSE41 = false, HW_SSE42 = false, HW_SSE4a = false;
//	static inline bool HW_AES = false, HW_SHA = false;
//	// SIMD: 256-bit
//	static inline bool HW_AVX = false, HW_XOP = false, HW_FMA3 = false, HW_FMA4 = false;
//	static inline bool HW_AVX2 = false;
//	// SIMD: 512-bit
//	static inline bool HW_AVX512F = false; //  AVX512 Foundation
//	static inline bool HW_AVX512CD = false; //  AVX512 Conflict Detection
//	static inline bool HW_AVX512PF = false; //  AVX512 Prefetch
//	static inline bool HW_AVX512ER = false; //  AVX512 Exponential + Reciprocal
//	static inline bool HW_AVX512VL = false; //  AVX512 Vector Length Extensions
//	static inline bool HW_AVX512BW = false; //  AVX512 Byte + Word
//	static inline bool HW_AVX512DQ = false; //  AVX512 Doubleword + Quadword
//	static inline bool HW_AVX512IFMA = false; //  AVX512 Integer 52-bit Fused Multiply-Add
//	static inline bool HW_AVX512VBMI = false; //  AVX512 Vector Byte Manipulation Instructions
//	// constructor
//	CPUCaps()
//	{
//		int info[4];
//		cpuid(info, 0);
//		int nIds = info[0];
//		cpuid(info, 0x80000000);
//		unsigned nExIds = info[0];
//		// detect cpu features
//		if (nIds >= 0x00000001)
//		{
//			cpuid(info, 0x00000001);
//			HW_MMX = (info[3] & ((int)1 << 23)) != 0;
//			HW_SSE = (info[3] & ((int)1 << 25)) != 0;
//			HW_SSE2 = (info[3] & ((int)1 << 26)) != 0;
//			HW_SSE3 = (info[2] & ((int)1 << 0)) != 0;
//			HW_SSSE3 = (info[2] & ((int)1 << 9)) != 0;
//			HW_SSE41 = (info[2] & ((int)1 << 19)) != 0;
//			HW_SSE42 = (info[2] & ((int)1 << 20)) != 0;
//			HW_AES = (info[2] & ((int)1 << 25)) != 0;
//			HW_AVX = (info[2] & ((int)1 << 28)) != 0;
//			HW_FMA3 = (info[2] & ((int)1 << 12)) != 0;
//			HW_RDRAND = (info[2] & ((int)1 << 30)) != 0;
//		}
//		if (nIds >= 0x00000007)
//		{
//			cpuid(info, 0x00000007);
//			HW_AVX2 = (info[1] & ((int)1 << 5)) != 0;
//			HW_BMI1 = (info[1] & ((int)1 << 3)) != 0;
//			HW_BMI2 = (info[1] & ((int)1 << 8)) != 0;
//			HW_ADX = (info[1] & ((int)1 << 19)) != 0;
//			HW_SHA = (info[1] & ((int)1 << 29)) != 0;
//			HW_PREFETCHWT1 = (info[2] & ((int)1 << 0)) != 0;
//			HW_AVX512F = (info[1] & ((int)1 << 16)) != 0;
//			HW_AVX512CD = (info[1] & ((int)1 << 28)) != 0;
//			HW_AVX512PF = (info[1] & ((int)1 << 26)) != 0;
//			HW_AVX512ER = (info[1] & ((int)1 << 27)) != 0;
//			HW_AVX512VL = (info[1] & ((int)1 << 31)) != 0;
//			HW_AVX512BW = (info[1] & ((int)1 << 30)) != 0;
//			HW_AVX512DQ = (info[1] & ((int)1 << 17)) != 0;
//			HW_AVX512IFMA = (info[1] & ((int)1 << 21)) != 0;
//			HW_AVX512VBMI = (info[2] & ((int)1 << 1)) != 0;
//		}
//		if (nExIds >= 0x80000001)
//		{
//			cpuid(info, 0x80000001);
//			HW_x64 = (info[3] & ((int)1 << 29)) != 0;
//			HW_ABM = (info[2] & ((int)1 << 5)) != 0;
//			HW_SSE4a = (info[2] & ((int)1 << 6)) != 0;
//			HW_FMA4 = (info[2] & ((int)1 << 16)) != 0;
//			HW_XOP = (info[2] & ((int)1 << 11)) != 0;
//		}
//	}
//	};
#else
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES3/gl31.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <EGL/eglext.h> // EGL_OPENGL_ES3_BIT_KHR etc.
#include <X11/Xlib.h> // Display object etc.
#include <unistd.h>
#include <pthread.h>
#include <X11/Xutil.h>
#endif
#include <vector>
#include <fcntl.h>
#include <string.h>
#include <fstream>


////define only if we run on the ARM platform
//#ifdef __arm__
//#ifdef USE_NEON_SIMD
//
//#include <arm_neon.h>
//
//inline float hormin(const float32x4_t v4)
//{
//	const float32x2_t minOfHalfs = vpmin_f32(vget_low_f32(v4), vget_high_f32(v4));
//	const float32x2_t minOfMinOfHalfs = vpmin_f32(minOfHalfs, minOfHalfs);
//	return vget_lane_f32(minOfMinOfHalfs, 0);
//}
//
//inline float hormax(const float32x4_t v4)
//{
//	const float32x2_t maxOfHalfs = vpmax_f32(vget_low_f32(v4), vget_high_f32(v4));
//	const float32x2_t maxOfMaxOfHalfs = vpmax_f32(maxOfHalfs, maxOfHalfs);
//	return vget_lane_f32(maxOfMaxOfHalfs, 0);
//}
//
//inline float32x4_t reciproc4(const float32x4_t v4)
//{
//	float32x4_t reci4 = vrecpeq_f32(v4); // just an approximation
//	reci4 = vmulq_f32(vrecpsq_f32(v4, reci4), reci4); // Newton-Raphson #1
//	return vmulq_f32(vrecpsq_f32(v4, reci4), reci4); // Newton-Raphson #2
//}
//
//#endif
//#endif

using namespace std;

// include the surface and sprite classes and related functionality
#include "surface.h"
#include "sprite.h"

using namespace Tmpl8;

// aligned memory allocations
#if WINBUILD
#define ALIGN( x ) __declspec( align( x ) )
#define MALLOC64( x ) ( ( x ) == 0 ? 0 : _aligned_malloc( ( x ), 64 ) )
#define FREE64( x ) _aligned_free( x )
#else
#define ALIGN( x ) __attribute__( ( aligned( x ) ) )
#define MALLOC64( x ) ( ( x ) == 0 ? 0 : aligned_alloc( 64, ( x ) ) )
#define FREE64( x ) free( x )
#endif
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define CHECK_RESULT __attribute__ ((warn_unused_result))
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
#define CHECK_RESULT _Check_return_
#else
#define CHECK_RESULT
#endif

//// include math classes
//#include "tmpl8math.h"

// fatal error reporting (with a pretty window)
void FatalError(const char* fmt, ...);
#define FATALERROR( fmt, ... ) FatalError( "Error on line %d of %s: " fmt "\n", __LINE__, __FILE__, ##__VA_ARGS__ )
#define FATALERROR_IF( condition, fmt, ... ) do { if ( ( condition ) ) FATALERROR( fmt, ##__VA_ARGS__ ); } while ( 0 )
#define FATALERROR_IN( prefix, errstr, fmt, ... ) FatalError( prefix " returned error '%s' at %s:%d" fmt "\n", errstr, __FILE__, __LINE__, ##__VA_ARGS__ );
#define FATALERROR_IN_CALL( stmt, error_parser, fmt, ... ) do { auto ret = ( stmt ); if ( ret ) FATALERROR_IN( #stmt, error_parser( ret ), fmt, ##__VA_ARGS__ ) } while ( 0 )

// include opengl template functionality
#include "opengl.h"

// generic error checking for OpenGL code
#define CheckGL() { _CheckGL( __FILE__, __LINE__ ); }

// forward declarations of helper functions
void _CheckGL(const char* f, int l);
GLuint CreateVBO(const GLfloat* data, const uint size);
void BindVBO(const uint idx, const uint N, const GLuint id);
void CheckShader(GLuint shader, const char* vshader, const char* fshader);
void CheckProgram(GLuint id, const char* vshader, const char* fshader);
void DrawQuad();
void FixWorkingFolder();
string TextFileRead(const char* _File);


// EOF
