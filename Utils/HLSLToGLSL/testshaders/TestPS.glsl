/*     Copyright 2015-2017 Egor Yusov
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF ANY PROPRIETARY RIGHTS.
 *
 *  In no event and under no legal theory, whether in tort (including negligence), 
 *  contract, or otherwise, unless required by applicable law (such as deliberate 
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental, 
 *  or consequential damages of any character arising as a result of this License or 
 *  out of the use or inability to use the software (including but not limited to damages 
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and 
 *  all other commercial damages or losses), even if such Contributor has been advised 
 *  of the possibility of such damages.
 */

#ifndef _GLSL_DEFINITIONS_
#define _GLSL_DEFINITIONS_

#define GLSL

#define float4 vec4
#define float3 vec3
#define float2 vec2

#define int4 ivec4
#define int3 ivec3
#define int2 ivec2

#define uint4 uvec4
#define uint3 uvec3
#define uint2 uvec2

#define bool4 bvec4
#define bool3 bvec3
#define bool2 bvec2

// OpenGL matrices in GLSL are always as column-major 
// (this is not related to how they are stored)
#define float2x2 mat2x2
#define float2x3 mat3x2
#define float2x4 mat4x2

#define float3x2 mat2x3
#define float3x3 mat3x3
#define float3x4 mat4x3

#define float4x2 mat2x4
#define float4x3 mat3x4
#define float4x4 mat4x4
#define matrix mat4x4

#define static

#define SamplerState int
#define SamplerComparisonState int

// https://www.opengl.org/wiki/Memory_Model#Incoherent_memory_access
// Shared variable access uses the rules for incoherent memory access. 
// This means that the user must perform certain synchronization in 
// order to ensure that shared variables are visible.
// At the same time, shared variables are all implicitly declared coherent, 
// so one don't need to (and can't) use that qualifier.
#define groupshared shared

#ifdef FRAGMENT_SHADER
#   define ddx dFdx
#   define ddy dFdy
#else
#   define ddx(x) (x) // GLSL compiler fails when it sees derivatives 
#   define ddy(x) (x) // in any shader but fragment
#endif

#define ddx_coarse ddx
#define ddy_coarse ddy
#define ddx_fine ddx
#define ddy_fine ddy

#define mul(a, b) ((a)*(b))
#define frac fract
#define atan2 atan
#define rsqrt inversesqrt
#define fmod mod
#define lerp mix
#define dst distance
#define countbits bitCount
#define firstbithigh findMSB
#define firstbitlow findLSB
#define reversebits bitfieldReverse

float rcp( float x ){ return 1.0 / x; }
vec2  rcp( vec2  x ){ return vec2(1.0,1.0) / x; }
vec3  rcp( vec3  x ){ return vec3(1.0,1.0,1.0) / x; }
vec4  rcp( vec4  x ){ return vec4(1.0,1.0,1.0,1.0) / x; }

float saturate( float x ){ return clamp( x, 0.0,                      1.0 ); }
vec2  saturate( vec2  x ){ return clamp( x, vec2(0.0, 0.0),           vec2(1.0, 1.0) ); }
vec3  saturate( vec3  x ){ return clamp( x, vec3(0.0, 0.0, 0.0),      vec3(1.0, 1.0, 1.0) ); }
vec4  saturate( vec4  x ){ return clamp( x, vec4(0.0, 0.0, 0.0, 0.0), vec4(1.0, 1.0, 1.0, 1.0) ); }

void sincos( float x, out float s, out float c ){ s = sin( x ); c = cos( x ); }
void sincos( vec2  x, out vec2  s, out vec2  c ){ s = sin( x ); c = cos( x ); }
void sincos( vec3  x, out vec3  s, out vec3  c ){ s = sin( x ); c = cos( x ); }
void sincos( vec4  x, out vec4  s, out vec4  c ){ s = sin( x ); c = cos( x ); }


// Bit conversion operations

float asfloat( float x ){ return x; }
vec2  asfloat( vec2  x ){ return x; }
vec3  asfloat( vec3  x ){ return x; }
vec4  asfloat( vec4  x ){ return x; }

float asfloat( int   x ){ return intBitsToFloat(x); }
vec2  asfloat( ivec2 x ){ return intBitsToFloat(x); }
vec3  asfloat( ivec3 x ){ return intBitsToFloat(x); }
vec4  asfloat( ivec4 x ){ return intBitsToFloat(x); }

float asfloat( uint  x ){ return uintBitsToFloat(x); }
vec2  asfloat( uvec2 x ){ return uintBitsToFloat(x); }
vec3  asfloat( uvec3 x ){ return uintBitsToFloat(x); }
vec4  asfloat( uvec4 x ){ return uintBitsToFloat(x); }


int   asint( int   x ){ return x; }
ivec2 asint( ivec2 x ){ return x; }
ivec3 asint( ivec3 x ){ return x; }
ivec4 asint( ivec4 x ){ return x; }

int   asint( uint  x ){ return int(x);   }
ivec2 asint( uvec2 x ){ return ivec2(x); }
ivec3 asint( uvec3 x ){ return ivec3(x); }
ivec4 asint( uvec4 x ){ return ivec4(x); }

int   asint( float x ){ return floatBitsToInt(x); }
ivec2 asint( vec2  x ){ return floatBitsToInt(x); }
ivec3 asint( vec3  x ){ return floatBitsToInt(x); }
ivec4 asint( vec4  x ){ return floatBitsToInt(x); }


uint  asuint( uint  x ){ return x; }
uvec2 asuint( uvec2 x ){ return x; }
uvec3 asuint( uvec3 x ){ return x; }
uvec4 asuint( uvec4 x ){ return x; }

uint  asuint( int  x  ){ return  uint(x); }
uvec2 asuint( ivec2 x ){ return uvec2(x); }
uvec3 asuint( ivec3 x ){ return uvec3(x); }
uvec4 asuint( ivec4 x ){ return uvec4(x); }

uint  asuint( float x ){ return floatBitsToUint(x); }
uvec2 asuint( vec2  x ){ return floatBitsToUint(x); }
uvec3 asuint( vec3  x ){ return floatBitsToUint(x); }
uvec4 asuint( vec4  x ){ return floatBitsToUint(x); }


float f16tof32( uint u1 )
{ 
    return unpackHalf2x16( u1 ).x; 
}
vec2 f16tof32( uvec2 u2 )
{ 
    uint u2PackedHalf = (u2.x & 0x0ffffu) | ((u2.y & 0x0ffffu) << 16u);
    return unpackHalf2x16( u2PackedHalf ); 
}
vec3 f16tof32( uvec3 u3 )
{ 
    return vec3( f16tof32( u3.xy ), f16tof32( u3.z ) );
}
vec4 f16tof32( uvec4 u4 )
{ 
    return vec4( f16tof32( u4.xy ), f16tof32( u4.zw ) );
}
float f16tof32( int   i1 ){ return f16tof32( uint ( i1 ) ); }
vec2  f16tof32( ivec2 i2 ){ return f16tof32( uvec2( i2 ) ); }
vec3  f16tof32( ivec3 i3 ){ return f16tof32( uvec3( i3 ) ); }
vec4  f16tof32( ivec4 i4 ){ return f16tof32( uvec4( i4 ) ); }

uint f32tof16( float f )
{ 
    return packHalf2x16( vec2( f, 0.0 ) ) & 0x0ffffu; 
}
uvec2 f32tof16( vec2 f2 )
{ 
    uint u2PackedHalf = packHalf2x16( f2 );
    return uvec2( u2PackedHalf & 0x0ffffu, u2PackedHalf >> 16u );
}
uvec3 f32tof16( vec3 f3 )
{
    return uvec3( f32tof16( f3.xy ), f32tof16( f3.z ) );
}
uvec4 f32tof16( vec4 f4 )
{
    return uvec4( f32tof16( f4.xy ), f32tof16( f4.zw ) );
}

#ifndef GL_ES // double is not supported on GLES
double asdouble(uint lowbits, uint highbits)
{
    return packDouble2x32( uvec2( lowbits, highbits ) );
}
#endif


// Floating point functions

bool isfinite( float x )
{
    return !isinf( x ) && !isnan( x );
}

bool2 isfinite( vec2 f2 )
{
    return bool2( isfinite( f2.x ), isfinite( f2.y ) );
}

bool3 isfinite( vec3 f3 )
{
    return bool3( isfinite( f3.xy ), isfinite( f3.z ) );
}

bool4 isfinite( vec4 f4 )
{
    return bool4( isfinite( f4.xyz ), isfinite( f4.w ) );
}

#ifndef GL_ES
    float noise( float x ){ return noise1( x ); }
    vec2  noise( vec2  x ){ return noise2( x ); }
    vec3  noise( vec3  x ){ return noise3( x ); }
    vec4  noise( vec4  x ){ return noise4( x ); }
#else
    float noise( float x ){ return 0.0; }
    vec2  noise( vec2  x ){ return vec2(0.0, 0.0); }
    vec3  noise( vec3  x ){ return vec3(0.0, 0.0, 0.0); }
    vec4  noise( vec4  x ){ return vec4(0.0, 0.0, 0.0, 0.0); }
#endif

float log10( float x )
{
    return log( x ) / log( 10.0 );
}
vec2 log10( vec2 x )
{
    float _lg10 = log( 10.0 );
    return log( x ) / vec2(_lg10, _lg10);
}
vec3 log10( vec3 x )
{
    float _lg10 = log( 10.0 );
    return log( x ) / vec3(_lg10, _lg10, _lg10);
}
vec4 log10( vec4 x )
{
    float _lg10 = log( 10.0 );
    return log( x ) / vec4(_lg10, _lg10, _lg10, _lg10);
}


#ifdef GL_ES
#   define mad(a,b,c) ((a)*(b)+(c))
#else
#   define mad fma
#endif


// Relational and logical operators
#define Less lessThan
#define LessEqual lessThanEqual
#define Greater greaterThan
#define GreaterEqual greaterThanEqual
#define Equal equal
#define NotEqual notEqual
#define Not not
bool4 And(bool4 L, bool4 R)
{
    return bool4(L.x && R.x,
                 L.y && R.y,
                 L.z && R.z,
                 L.w && R.w);
}
bool3 And(bool3 L, bool3 R)
{
    return bool3(L.x && R.x,
                 L.y && R.y,
                 L.z && R.z);
}
bool2 And(bool2 L, bool2 R)
{
    return bool2(L.x && R.x,
                 L.y && R.y);
}
bool And(bool L, bool R)
{
    return (L && R);
}


bool4 Or(bool4 L, bool4 R)
{
    return bool4(L.x || R.x,
                 L.y || R.y,
                 L.z || R.z,
                 L.w || R.w);
}
bool3 Or(bool3 L, bool3 R)
{
    return bool3(L.x || R.x,
                 L.y || R.y,
                 L.z || R.z);
}
bool2 Or(bool2 L, bool2 R)
{
    return bool2(L.x || R.x,
                 L.y || R.y);
}
bool Or(bool L, bool R)
{
    return (L || R);
}

float4 BoolToFloat( bool4 b4 )
{
    return float4(b4.x ? 1.0 : 0.0,
                  b4.y ? 1.0 : 0.0,
                  b4.z ? 1.0 : 0.0,
                  b4.w ? 1.0 : 0.0);
}
float3 BoolToFloat( bool3 b3 )
{
    return float3(b3.x ? 1.0 : 0.0,
                  b3.y ? 1.0 : 0.0,
                  b3.z ? 1.0 : 0.0);
}
float2 BoolToFloat( bool2 b2 )
{
    return float2(b2.x ? 1.0 : 0.0,
                  b2.y ? 1.0 : 0.0);
}
float BoolToFloat( bool b )
{
    return b ? 1.0 : 0.0;
}


// Synchronization functions

#ifdef COMPUTE_SHADER

// https://www.opengl.org/wiki/Memory_Model#Incoherent_memory_access

// MSDN: GroupMemoryBarrier() blocks execution of all threads 
// in a group until all group SHARED accesses have been completed.
void GroupMemoryBarrier()
{
    // OpenGL.org: groupMemoryBarrier() waits on the completion of all memory accesses 
    // performed by an invocation of a compute shader relative to the same access performed 
    // by other invocations in the same work group and then returns with no other effect.

    // groupMemoryBarrier() acts like memoryBarrier(), ordering memory writes for all kinds 
    // of variables, but it only orders read/writes for the current work group.
    groupMemoryBarrier();

    // OpenGL.org: memoryBarrierShared() waits on the completion of 
    // all memory accesses resulting from the use of SHARED variables
    // and then returns with no other effect. 
    memoryBarrierShared();
}

// MSDN: GroupMemoryBarrierWithGroupSync() blocks execution of all 
// threads in a group until all memory accesses have been completed 
// and all threads in the group have reached this call.
void GroupMemoryBarrierWithGroupSync()
{
    // Issue memory barrier first!
    GroupMemoryBarrier();
    barrier();
}

// MSDN: DeviceMemoryBarrier() blocks execution of all threads 
// in a group until all device memory accesses have been completed.
void DeviceMemoryBarrier()
{
    // Call all memory barriers except for shared memory
    
    // Do we need to call groupMemoryBarrier() ????? 

    // OpenGL.org: memoryBarrierBuffer() waits on the completion of 
    // all memory accesses resulting from the use of BUFFER variables 
    // and then returns with no other effect
    memoryBarrierBuffer();

    // OpenGL.org: memoryBarrierImage() waits on the completion of all 
    // memory accesses resulting from the use of IMAGE variables and then 
    // returns with no other effect. 
    memoryBarrierImage();

    // OpenGL.org: memoryBarrierAtomicCounter() waits on the completion of 
    // all accesses resulting from the use of ATOMIC COUNTERS and then returns 
    // with no other effect. 
    memoryBarrierAtomicCounter();
}

// MSDN: DeviceMemoryBarrierWithGroupSync() blocks execution of 
// all threads in a group until all device memory accesses have 
// been completed and all threads in the group have reached this call.
void DeviceMemoryBarrierWithGroupSync()
{
    DeviceMemoryBarrier();
    barrier();
}

// MSDN: AllMemoryBarrier() blocks execution of all threads in a 
// group until all memory accesses have been completed.
void AllMemoryBarrier()
{
    // OpenGL.org: memoryBarrier() waits on the completion of ALL 
    // memory accesses resulting from the use of IMAGE variables or 
    // ATOMIC COUNTERS and then returns with no other effect.
    memoryBarrier();
    // NOTE: nothing is said about buffer memory and shared memory,
    // so call memoryBarrierBuffer() and memoryBarrierShared() for safety

    // OpenGL.org: memoryBarrierBuffer() waits on the completion of 
    // all memory accesses resulting from the use of BUFFER variables 
    // and then returns with no other effect
    memoryBarrierBuffer();

    // OpenGL.org: memoryBarrierShared() waits on the completion of 
    // all memory accesses resulting from the use of SHARED variables
    // and then returns with no other effect. 
    memoryBarrierShared();

    // Call all memory barrier functions. They should have no effect
    // if everything is synchronized.
    
    // OpenGL.org: memoryBarrierImage() waits on the completion of all 
    // memory accesses resulting from the use of IMAGE variables and then 
    // returns with no other effect. 
    memoryBarrierImage();

    // OpenGL.org: memoryBarrierAtomicCounter() waits on the completion of 
    // all accesses resulting from the use of ATOMIC COUNTERS and then returns 
    // with no other effect. 
    memoryBarrierAtomicCounter();

    // groupMemoryBarrier waits on the completion of all memory accesses performed 
    // by an invocation of a compute shader relative to the same access performed by 
    // other invocations in the same work group and then returns with no other effect.
    groupMemoryBarrier();
}

// MSDN: AllMemoryBarrierWithGroupSync() blocks execution of all 
// threads in a group until all memory accesses have been completed 
// and all threads in the group have reached this call.
void AllMemoryBarrierWithGroupSync()
{
    AllMemoryBarrier();
    barrier();
}

#else

void AllMemoryBarrier(){}
void AllMemoryBarrierWithGroupSync(){}
void DeviceMemoryBarrier(){}
void DeviceMemoryBarrierWithGroupSync(){}
void GroupMemoryBarrier(){}
void GroupMemoryBarrierWithGroupSync(){}

#endif


// Type conversion functions

vec4 _ExpandVector( float x ){ return vec4(    x,    x,    x,    x ); }
vec4 _ExpandVector( vec2 f2 ){ return vec4( f2.x, f2.y,  0.0,  0.0 ); }
vec4 _ExpandVector( vec3 f3 ){ return vec4( f3.x, f3.y, f3.z,  0.0 ); }
vec4 _ExpandVector( vec4 f4 ){ return vec4( f4.x, f4.y, f4.z, f4.w ); }

ivec4 _ExpandVector( int    x ){ return ivec4(    x,    x,    x,    x ); }
ivec4 _ExpandVector( ivec2 i2 ){ return ivec4( i2.x, i2.y,    0,    0 ); }
ivec4 _ExpandVector( ivec3 i3 ){ return ivec4( i3.x, i3.y, i3.z,    0 ); }
ivec4 _ExpandVector( ivec4 i4 ){ return ivec4( i4.x, i4.y, i4.z, i4.w ); }

uvec4 _ExpandVector( uint   x ){ return uvec4(    x,    x,    x,    x ); }
uvec4 _ExpandVector( uvec2 u2 ){ return uvec4( u2.x, u2.y,   0u,   0u ); }
uvec4 _ExpandVector( uvec3 u3 ){ return uvec4( u3.x, u3.y, u3.z,   0u ); }
uvec4 _ExpandVector( uvec4 u4 ){ return uvec4( u4.x, u4.y, u4.z, u4.w ); }

bvec4 _ExpandVector( bool   x ){ return bvec4(    x,    x,     x,     x ); }
bvec4 _ExpandVector( bvec2 b2 ){ return bvec4( b2.x, b2.y, false, false ); }
bvec4 _ExpandVector( bvec3 b3 ){ return bvec4( b3.x, b3.y,  b3.z, false ); }
bvec4 _ExpandVector( bvec4 b4 ){ return bvec4( b4.x, b4.y,  b4.z,  b4.w ); }

void _ResizeVector(out vec4 outVec4, in vec4 inVec4){outVec4 = inVec4;}
void _ResizeVector(out vec3 outVec3, in vec4 inVec4){outVec3 = inVec4.xyz;}
void _ResizeVector(out vec2 outVec2, in vec4 inVec4){outVec2 = inVec4.xy;}
void _ResizeVector(out float outFlt, in vec4 inVec4){outFlt  = inVec4.x;}

void _ResizeVector(out vec4 outVec4, in vec3 inVec3){outVec4 = vec4(inVec3, 0.0);}
void _ResizeVector(out vec3 outVec3, in vec3 inVec3){outVec3 = inVec3;}
void _ResizeVector(out vec2 outVec2, in vec3 inVec3){outVec2 = inVec3.xy;}
void _ResizeVector(out float outFlt, in vec3 inVec3){outFlt  = inVec3.x;}

void _ResizeVector(out vec4 outVec4, in vec2 inVec2){outVec4 = vec4(inVec2, 0.0, 0.0);}
void _ResizeVector(out vec3 outVec3, in vec2 inVec2){outVec3 = vec3(inVec2, 0.0);}
void _ResizeVector(out vec2 outVec2, in vec2 inVec2){outVec2 = inVec2;}
void _ResizeVector(out float outFlt, in vec2 inVec2){outFlt  = inVec2.x;}

void _ResizeVector(out vec4 outVec4, in float v){outVec4 = vec4(v, 0.0, 0.0, 0.0);}
void _ResizeVector(out vec3 outVec3, in float v){outVec3 = vec3(v, 0.0, 0.0);}
void _ResizeVector(out vec2 outVec2, in float v){outVec2 = vec2(v, 0.0);}
void _ResizeVector(out float outFlt, in float v){outFlt  = v;}


void _TypeConvertStore( out float Dst, in int   Src ){ Dst = float( Src ); }
void _TypeConvertStore( out float Dst, in uint  Src ){ Dst = float( Src ); }
void _TypeConvertStore( out float Dst, in float Src ){ Dst = float( Src ); }
void _TypeConvertStore( out uint  Dst, in int   Src ){ Dst = uint( Src ); }
void _TypeConvertStore( out uint  Dst, in uint  Src ){ Dst = uint( Src ); }
void _TypeConvertStore( out uint  Dst, in float Src ){ Dst = uint( Src ); }
void _TypeConvertStore( out int   Dst, in int   Src ){ Dst = int( Src ); }
void _TypeConvertStore( out int   Dst, in uint  Src ){ Dst = int( Src ); }
void _TypeConvertStore( out int   Dst, in float Src ){ Dst = int( Src ); }

int _ToInt( int x )  { return int(x);       }
int _ToInt( uint x ) { return int(x);       }
int _ToInt( float x ){ return int(x);       }
int _ToInt( bool x ) { return x ? 1 : 0;    }

float _ToFloat( int x )  { return float(x);     }
float _ToFloat( uint x ) { return float(x);     }
float _ToFloat( float x ){ return float(x);     }
float _ToFloat( bool x ) { return x ? 1.0 : 0.0;}

uint _ToUint( int x )  { return uint(x);     }
uint _ToUint( uint x ) { return uint(x);     }
uint _ToUint( float x ){ return uint(x);     }
uint _ToUint( bool x ) { return x ? 1u : 0u; }

bool _ToBool( int x )  { return x != 0   ? true : false; }
bool _ToBool( uint x ) { return x != 0u  ? true : false; }
bool _ToBool( float x ){ return x != 0.0 ? true : false; }
bool _ToBool( bool x ) { return x; }

#define _ToVec2(x,y)     vec2(_ToFloat(x), _ToFloat(y))
#define _ToVec3(x,y,z)   vec3(_ToFloat(x), _ToFloat(y), _ToFloat(z))
#define _ToVec4(x,y,z,w) vec4(_ToFloat(x), _ToFloat(y), _ToFloat(z), _ToFloat(w))

#define _ToIvec2(x,y)     ivec2(_ToInt(x), _ToInt(y))
#define _ToIvec3(x,y,z)   ivec3(_ToInt(x), _ToInt(y), _ToInt(z))
#define _ToIvec4(x,y,z,w) ivec4(_ToInt(x), _ToInt(y), _ToInt(z), _ToInt(w))

#define _ToUvec2(x,y)     uvec2(_ToUint(x), _ToUint(y))
#define _ToUvec3(x,y,z)   uvec3(_ToUint(x), _ToUint(y), _ToUint(z))
#define _ToUvec4(x,y,z,w) uvec4(_ToUint(x), _ToUint(y), _ToUint(z), _ToUint(w))

#define _ToBvec2(x,y)     bvec2(_ToBool(x), _ToBool(y))
#define _ToBvec3(x,y,z)   bvec3(_ToBool(x), _ToBool(y), _ToBool(z))
#define _ToBvec4(x,y,z,w) bvec4(_ToBool(x), _ToBool(y), _ToBool(z), _ToBool(w))


int   _ToIvec( uint  u1 ){ return _ToInt(   u1 ); }
ivec2 _ToIvec( uvec2 u2 ){ return _ToIvec2( u2.x, u2.y ); }
ivec3 _ToIvec( uvec3 u3 ){ return _ToIvec3( u3.x, u3.y, u3.z ); }
ivec4 _ToIvec( uvec4 u4 ){ return _ToIvec4( u4.x, u4.y, u4.z, u4.w ); }

int   _ToIvec( int   i1 ){ return i1; }
ivec2 _ToIvec( ivec2 i2 ){ return i2; }
ivec3 _ToIvec( ivec3 i3 ){ return i3; }
ivec4 _ToIvec( ivec4 i4 ){ return i4; }

int   _ToIvec( float f1 ){ return _ToInt(   f1 ); }
ivec2 _ToIvec( vec2  f2 ){ return _ToIvec2( f2.x, f2.y ); }
ivec3 _ToIvec( vec3  f3 ){ return _ToIvec3( f3.x, f3.y, f3.z ); }
ivec4 _ToIvec( vec4  f4 ){ return _ToIvec4( f4.x, f4.y, f4.z, f4.w ); }


float _ToVec( uint  u1 ){ return _ToFloat(u1); }
vec2  _ToVec( uvec2 u2 ){ return _ToVec2( u2.x, u2.y ); }
vec3  _ToVec( uvec3 u3 ){ return _ToVec3( u3.x, u3.y, u3.z ); }
vec4  _ToVec( uvec4 u4 ){ return _ToVec4( u4.x, u4.y, u4.z, u4.w ); }
         
float _ToVec( int   i1 ){ return _ToFloat(i1); }
vec2  _ToVec( ivec2 i2 ){ return _ToVec2( i2.x, i2.y ); }
vec3  _ToVec( ivec3 i3 ){ return _ToVec3( i3.x, i3.y, i3.z ); }
vec4  _ToVec( ivec4 i4 ){ return _ToVec4( i4.x, i4.y, i4.z, i4.w ); }
         
float _ToVec( float f1 ){ return f1; }
vec2  _ToVec( vec2  f2 ){ return f2; }
vec3  _ToVec( vec3  f3 ){ return f3; }
vec4  _ToVec( vec4  f4 ){ return f4; }


uint   _ToUvec( uint  u1 ){ return u1; }
uvec2  _ToUvec( uvec2 u2 ){ return u2; }
uvec3  _ToUvec( uvec3 u3 ){ return u3; }
uvec4  _ToUvec( uvec4 u4 ){ return u4; }
         
uint   _ToUvec( int   i1 ){ return _ToUint(  i1 ); }
uvec2  _ToUvec( ivec2 i2 ){ return _ToUvec2( i2.x, i2.y ); }
uvec3  _ToUvec( ivec3 i3 ){ return _ToUvec3( i3.x, i3.y, i3.z ); }
uvec4  _ToUvec( ivec4 i4 ){ return _ToUvec4( i4.x, i4.y, i4.z, i4.w ); }
         
uint   _ToUvec( float f1 ){ return _ToUint(  f1 ); }
uvec2  _ToUvec( vec2  f2 ){ return _ToUvec2( f2.x, f2.y ); }
uvec3  _ToUvec( vec3  f3 ){ return _ToUvec3( f3.x, f3.y, f3.z ); }
uvec4  _ToUvec( vec4  f4 ){ return _ToUvec4( f4.x, f4.y, f4.z, f4.w ); }


// TEXTURE FUNCTION STUB MACROS
// https://www.opengl.org/wiki/Sampler_(GLSL)


// Texture size queries
// https://www.opengl.org/sdk/docs/man/html/textureSize.xhtml
// textureSize returns the dimensions of level lod (if present) of the texture bound to sampler. 
// The components in the return value are filled in, in order, with the width, height and depth 
// of the texture. For the array forms, the last component of the return value is the number of 
// layers in the texture array.

//#if !(defined(DESKTOP_GL) && __VERSION__ >= 430)
#   define textureQueryLevels(x) 0 // Only supported on 4.3+
//#endif

#define GetTex1DDimensions_1(Sampler, Width)\
{                                                       \
    _TypeConvertStore( Width, textureSize(Sampler, 0) );\
}

#define GetTex1DDimensions_3(Sampler, MipLevel, Width, NumberOfMipLevels)\
{                                                                        \
    _TypeConvertStore( Width, textureSize(Sampler, _ToInt(MipLevel)) );  \
    _TypeConvertStore( NumberOfMipLevels, textureQueryLevels(Sampler) ); \
}

#define GetTex1DArrDimensions_2(Sampler, Width, Elements)\
{                                           \
    ivec2 i2Size = textureSize(Sampler, 0); \
    _TypeConvertStore( Width,    i2Size.x );\
    _TypeConvertStore( Elements, i2Size.y );\
}

#define GetTex1DArrDimensions_4(Sampler, MipLevel, Width, Elements, NumberOfMipLevels)\
{                                                           \
    ivec2 i2Size = textureSize(Sampler, _ToInt(MipLevel));  \
    _TypeConvertStore( Width,    i2Size.x );                \
    _TypeConvertStore( Elements, i2Size.y );                \
    _TypeConvertStore( NumberOfMipLevels, textureQueryLevels(Sampler) );\
}

#define GetTex2DDimensions_2(Sampler, Width, Height)\
{                                                   \
    ivec2 i2Size = textureSize(Sampler, 0);         \
    _TypeConvertStore( Width,  i2Size.x );          \
    _TypeConvertStore( Height, i2Size.y );          \
}

#define GetTex2DDimensions_4(Sampler, MipLevel, Width, Height, NumberOfMipLevels)\
{                                                           \
    ivec2 i2Size = textureSize(Sampler, _ToInt(MipLevel) ); \
    _TypeConvertStore( Width,  i2Size.x );                              \
    _TypeConvertStore( Height, i2Size.y );                              \
    _TypeConvertStore( NumberOfMipLevels, textureQueryLevels(Sampler) );\
}

#define GetTex2DArrDimensions_3(Sampler, Width, Height, Elements)\
{                                           \
    ivec3 i3Size = textureSize(Sampler, 0); \
    _TypeConvertStore( Width,   i3Size.x ); \
    _TypeConvertStore( Height,  i3Size.y ); \
    _TypeConvertStore( Elements,i3Size.z ); \
}

#define GetTex2DArrDimensions_5(Sampler, MipLevel, Width, Height, Elements, NumberOfMipLevels)\
{                                                           \
    ivec3 i3Size = textureSize(Sampler, _ToInt(MipLevel));  \
    _TypeConvertStore( Width,    i3Size.x );                            \
    _TypeConvertStore( Height,   i3Size.y );                            \
    _TypeConvertStore( Elements, i3Size.z );                            \
    _TypeConvertStore( NumberOfMipLevels, textureQueryLevels(Sampler) );\
}

#define GetTex3DDimensions_3(Sampler, Width, Height, Depth)\
{                                           \
    ivec3 i3Size = textureSize(Sampler, 0); \
    _TypeConvertStore( Width,  i3Size.x );  \
    _TypeConvertStore( Height, i3Size.y );  \
    _TypeConvertStore( Depth,  i3Size.z );  \
}

#define GetTex3DDimensions_5(Sampler, MipLevel, Width, Height, Depth, NumberOfMipLevels)\
{                                                           \
    ivec3 i3Size = textureSize(Sampler, _ToInt(MipLevel));  \
    _TypeConvertStore( Width,  i3Size.x );                              \
    _TypeConvertStore( Height, i3Size.y );                              \
    _TypeConvertStore( Depth,  i3Size.z );                              \
    _TypeConvertStore( NumberOfMipLevels, textureQueryLevels(Sampler) );\
}

#define GetTex2DMSDimensions_3(Sampler, Width, Height, NumberOfSamples)\
{                                           \
    ivec2 i2Size = textureSize(Sampler);    \
    _TypeConvertStore( Width,  i2Size.x );  \
    _TypeConvertStore( Height, i2Size.y );  \
    _TypeConvertStore( NumberOfSamples, 0 );\
}

#define GetTex2DMSArrDimensions_4(Sampler, Width, Height, Elements, NumberOfSamples)\
{                                           \
    ivec3 i3Size = textureSize(Sampler);    \
    _TypeConvertStore( Width,    i3Size.x );\
    _TypeConvertStore( Height,   i3Size.y );\
    _TypeConvertStore( Elements, i3Size.z );\
    _TypeConvertStore( NumberOfSamples, 0 );\
}


// https://www.opengl.org/sdk/docs/man/html/imageSize.xhtml
// imageSize returns the dimensions of the image bound to image. The components in the 
// return value are filled in, in order, with the width, height and depth of the image. 
// For the array forms, the last component of the return value is the number of layers 
// in the texture array.

#define GetRWTex1DDimensions_1(Tex, Width)\
{                                               \
    _TypeConvertStore( Width, imageSize(Tex) ); \
}

#define GetRWTex1DArrDimensions_2(Tex, Width, Elements)\
{                                   \
    ivec2 i2Size = imageSize(Tex);  \
    _TypeConvertStore( Width,    i2Size.x ); \
    _TypeConvertStore( Elements, i2Size.y ); \
}

#define GetRWTex2DDimensions_2(Tex, Width, Height)\
{                                           \
    ivec2 i2Size = imageSize(Tex);          \
    _TypeConvertStore( Width,  i2Size.x );  \
    _TypeConvertStore( Height, i2Size.y );  \
}

#define GetRWTex2DArrDimensions_3(Tex, Width, Height, Elements)\
{                                           \
    ivec3 i3Size = imageSize(Tex);          \
    _TypeConvertStore( Width,    i3Size.x );\
    _TypeConvertStore( Height,   i3Size.y );\
    _TypeConvertStore( Elements, i3Size.z );\
}

#define GetRWTex3DDimensions_3(Tex, Width, Height, Depth)\
{                                           \
    ivec3 i3Size = imageSize(Tex);          \
    _TypeConvertStore( Width,  i3Size.x );  \
    _TypeConvertStore( Height, i3Size.y );  \
    _TypeConvertStore( Depth,  i3Size.z );  \
}



// Texture sampling operations


//                             IMPORTANT NOTE ABOUT OFFSET
// Offset parameter to all texture sampling functions must be a constant expression.
// If it is not, the shader will be successfully compiled, HOWEVER the value of Offset 
// will silently be zero. 
//
// A constant expression in GLSL is defined as follows:
// * A literal value.
// * A const-qualified variable with an explicit initializer (so not a function parameter).
// * The result of the length() function of an array, but only if the array has an explicit size.
// * The result of most operators, so long as all the operands are themselves constant expressions. 
//   The operators not on this list are any assignment operators (+= and so forth), and the comma operator.
// * The result of a constructor for a type, but only if all of the arguments to the constructor are 
//   themselves constant expressions.
// * The return value of any built-in function, but only if all of the arguments to the function are 
//   themselves constant expressions. Opaque Types are never constant expressions. Note that the 
//   functions dFdx, dFdy, and fwidth will return 0, when used in a context that requires a constant 
//   expression (such as a const variable initializer).
// 
// The list above does not include return value of a function, even when the value is compile-time expression.
// As a result, we cannot use type conversion functions for Offset parameter.

// In all texture sampling functions, the last component of Coords is used as Dsub and the array layer is specified 
// in the second to last component of Coords. (The second component of Coords is unused for 1D shadow lookups.)
// For cube array textures, Dsub is specified as a separate parameter
//                                                                                                                                     mip
#define SampleCmpLevel0Tex1D_3(Tex, Sampler, Coords, CompareValue)      textureLod(Tex, _ToVec3((Coords).x,        0.0, CompareValue), 0.0)
#define SampleCmpLevel0Tex1DArr_3(Tex, Sampler, Coords, CompareValue)   textureLod(Tex, _ToVec3((Coords).x, (Coords).y, CompareValue), 0.0)
#define SampleCmpLevel0Tex2D_3(Tex, Sampler, Coords, CompareValue)      textureLod(Tex, _ToVec3((Coords).x, (Coords).y, CompareValue), 0.0)
#define SampleCmpLevel0Tex2DArr_3(Tex, Sampler, Coords, CompareValue)   0.0 // No textureLod for sampler2DArrayShadow
#define SampleCmpLevel0TexCube_3(Tex, Sampler, Coords, CompareValue)    0.0 // No textureLod for samplerCubeShadow
#define SampleCmpLevel0TexCubeArr_3(Tex, Sampler, Coords, CompareValue) 0.0 // No textureLod for samplerCubeArrayShadow

//                                                                                                                                                 mip
#define SampleCmpLevel0Tex1D_4(Tex, Sampler, Coords, CompareValue, Offset)    textureLodOffset(Tex, _ToVec3((Coords).x,        0.0, CompareValue), 0.0, int(Offset))
#define SampleCmpLevel0Tex1DArr_4(Tex, Sampler, Coords, CompareValue, Offset) textureLodOffset(Tex, _ToVec3((Coords).x, (Coords).y, CompareValue), 0.0, int(Offset))
#define SampleCmpLevel0Tex2D_4(Tex, Sampler, Coords, CompareValue, Offset)    textureLodOffset(Tex, _ToVec3((Coords).x, (Coords).y, CompareValue), 0.0, ivec2((Offset).xy))
#define SampleCmpLevel0Tex2DArr_4(Tex, Sampler, Coords, CompareValue, Offset) 0.0 // No textureLodOffset for sampler2DArrayShadow


// https://www.opengl.org/sdk/docs/man/html/texture.xhtml - note: there are many mistakes on the page
#ifdef FRAGMENT_SHADER

#   define Sample_2(Tex, Sampler, Coords)         texture      (Tex, _ToVec(Coords))
#   define Sample_3(Tex, Sampler, Coords, Offset) textureOffset(Tex, _ToVec(Coords), Offset)
#   define SampleBias_3(Tex, Sampler, Coords, Bias)         texture      (Tex, _ToVec(Coords), _ToFloat(Bias))
#   define SampleBias_4(Tex, Sampler, Coords, Bias, Offset) textureOffset(Tex, _ToVec(Coords), Offset, _ToFloat(Bias))

#   define SampleCmpTex1D_3(Tex, Sampler, Coords, CompareValue)      texture(Tex, _ToVec3((Coords).x,        0.0, CompareValue))
#   define SampleCmpTex1DArr_3(Tex, Sampler, Coords, CompareValue)   texture(Tex, _ToVec3((Coords).x, (Coords).y, CompareValue))
#   define SampleCmpTex2D_3(Tex, Sampler, Coords, CompareValue)      texture(Tex, _ToVec3((Coords).x, (Coords).y, CompareValue))
#   define SampleCmpTex2DArr_3(Tex, Sampler, Coords, CompareValue)   texture(Tex, _ToVec4((Coords).x, (Coords).y, (Coords).z, CompareValue))
#   define SampleCmpTexCube_3(Tex, Sampler, Coords, CompareValue)    texture(Tex, _ToVec4((Coords).x, (Coords).y, (Coords).z, CompareValue))
#   define SampleCmpTexCubeArr_3(Tex, Sampler, Coords, CompareValue) texture(Tex, _ToVec4((Coords).x, (Coords).y, (Coords).z, (Coords).w), _ToFloat(CompareValue))

#   define SampleCmpTex1D_4(Tex, Sampler, Coords, CompareValue, Offset)    textureOffset(Tex, _ToVec3((Coords).x,        0.0, CompareValue), int(Offset))
#   define SampleCmpTex1DArr_4(Tex, Sampler, Coords, CompareValue, Offset) textureOffset(Tex, _ToVec3((Coords).x, (Coords).y, CompareValue), int(Offset))
#   define SampleCmpTex2D_4(Tex, Sampler, Coords, CompareValue, Offset)    textureOffset(Tex, _ToVec3((Coords).x, (Coords).y, CompareValue), ivec2((Offset).xy))
#   define SampleCmpTex2DArr_4(Tex, Sampler, Coords, CompareValue, Offset) textureOffset(Tex, _ToVec4((Coords).x, (Coords).y, (Coords).z, CompareValue), ivec2((Offset).xy))

#else

// Derivatives are only available in fragment shader. GLSL compiler fails when it
// encounters texture() or textureOffset() instructions in other types of shaders. So
// to let the shader be compiled and to have something meaningful, replace such operations
// with textureLod() and textureLodOffset()

#   define Sample_2(Tex, Sampler, Coords)         textureLod      (Tex, _ToVec(Coords), 0.0)
#   define Sample_3(Tex, Sampler, Coords, Offset) textureLodOffset(Tex, _ToVec(Coords), 0.0, Offset)
#   define SampleBias_3(Tex, Sampler, Coords, Bias)         textureLod      (Tex, _ToVec(Coords), 0.0 + _ToFloat(Bias))
#   define SampleBias_4(Tex, Sampler, Coords, Bias, Offset) textureLodOffset(Tex, _ToVec(Coords), 0.0 + _ToFloat(Bias), Offset)

#   define SampleCmpTex1D_3        SampleCmpLevel0Tex1D_3
#   define SampleCmpTex1DArr_3     SampleCmpLevel0Tex1DArr_3
#   define SampleCmpTex2D_3        SampleCmpLevel0Tex2D_3
#   define SampleCmpTex2DArr_3     SampleCmpLevel0Tex2DArr_3
#   define SampleCmpTexCube_3      SampleCmpLevel0TexCube_3
#   define SampleCmpTexCubeArr_3   SampleCmpLevel0TexCubeArr_3
                                            
#   define SampleCmpTex1D_4        SampleCmpLevel0Tex1D_4
#   define SampleCmpTex1DArr_4     SampleCmpLevel0Tex1DArr_4
#   define SampleCmpTex2D_4        SampleCmpLevel0Tex2D_4
#   define SampleCmpTex2DArr_4     SampleCmpLevel0Tex2DArr_4

#endif

// https://www.opengl.org/sdk/docs/man/html/textureLod.xhtml
#define SampleLevel_3(Tex, Sampler, Coords, Level)         textureLod      (Tex, _ToVec(Coords), _ToFloat(Level))
#define SampleLevel_4(Tex, Sampler, Coords, Level, Offset) textureLodOffset(Tex, _ToVec(Coords), _ToFloat(Level), Offset)

// https://www.opengl.org/sdk/docs/man/html/textureGrad.xhtml
#define SampleGrad_4(Tex, Sampler, Coords, DDX, DDY)         textureGrad      (Tex, _ToVec(Coords), _ToVec(DDX), _ToVec(DDY))
#define SampleGrad_5(Tex, Sampler, Coords, DDX, DDY, Offset) textureGradOffset(Tex, _ToVec(Coords), _ToVec(DDX), _ToVec(DDY), Offset)


// texelFetch performs a lookup of a single texel from texture coordinate P in the texture 
// bound to sampler. The array layer is specified in the last component of P for array forms. 
// The lod parameter (if present) specifies the level-of-detail from which the texel will be fetched. 
// The sample specifies which sample within the texel will be returned when reading from a multi-sample texure.

#define LoadTex1D_1(Tex, Location)        texelFetch      (Tex, _ToInt((Location).x), _ToInt((Location).y))
#define LoadTex1D_2(Tex, Location, Offset)texelFetchOffset(Tex, _ToInt((Location).x), _ToInt((Location).y), int(Offset))
#define LoadTex1DArr_1(Tex, Location)        texelFetch      (Tex, _ToIvec( (Location).xy), _ToInt((Location).z) )
#define LoadTex1DArr_2(Tex, Location, Offset)texelFetchOffset(Tex, _ToIvec( (Location).xy), _ToInt((Location).z), int(Offset))
#define LoadTex2D_1(Tex, Location)        texelFetch      (Tex, _ToIvec( (Location).xy), _ToInt((Location).z))
#define LoadTex2D_2(Tex, Location, Offset)texelFetchOffset(Tex, _ToIvec( (Location).xy), _ToInt((Location).z), ivec2( (Offset).xy) )
#define LoadTex2DArr_1(Tex, Location)        texelFetch      (Tex, _ToIvec( (Location).xyz), _ToInt((Location).w) )
#define LoadTex2DArr_2(Tex, Location, Offset)texelFetchOffset(Tex, _ToIvec( (Location).xyz), _ToInt((Location).w), ivec2( (Offset).xy))
#define LoadTex3D_1(Tex, Location)        texelFetch      (Tex, _ToIvec( (Location).xyz), _ToInt((Location).w))
#define LoadTex3D_2(Tex, Location, Offset)texelFetchOffset(Tex, _ToIvec( (Location).xyz), _ToInt((Location).w), ivec3( (Offset).xyz))
#define LoadTex2DMS_2(Tex, Location, Sample)        texelFetch(Tex, _ToIvec( (Location).xy), _ToInt(Sample))
#define LoadTex2DMS_3(Tex, Location, Sample, Offset)texelFetch(Tex, _ToIvec2( (Location).x + (Offset).x, (Location).y + (Offset).y), int(Sample) ) // No texelFetchOffset for texture2DMS
#define LoadTex2DMSArr_2(Tex, Location, Sample)        texelFetch(Tex, _ToIvec( (Location).xyz), _ToInt(Sample))
#define LoadTex2DMSArr_3(Tex, Location, Sample, Offset)texelFetch(Tex, _ToIvec3( (Location).x + (Offset).x, (Location).y + (Offset).y, (Location).z), int(Sample)) // No texelFetchOffset for texture2DMSArray

//https://www.opengl.org/sdk/docs/man/html/imageLoad.xhtml
#define LoadRWTex1D_1(Tex, Location)    imageLoad(Tex, _ToIvec((Location).x)   )
#define LoadRWTex1DArr_1(Tex, Location) imageLoad(Tex, _ToIvec((Location).xy)  )
#define LoadRWTex2D_1(Tex, Location)    imageLoad(Tex, _ToIvec((Location).xy)  )
#define LoadRWTex2DArr_1(Tex, Location) imageLoad(Tex, _ToIvec((Location).xyz) )
#define LoadRWTex3D_1(Tex, Location)    imageLoad(Tex, _ToIvec((Location).xyz) )


#define Gather_2(Tex, Sampler, Location)        textureGather      (Tex, _ToVec(Location))
#define Gather_3(Tex, Sampler, Location, Offset)textureGatherOffset(Tex, _ToVec(Location), Offset)

#define GatherCmp_3(Tex, Sampler, Location, CompareVal)        textureGather      (Tex, _ToVec(Location), _ToFloat(CompareVal))
#define GatherCmp_4(Tex, Sampler, Location, CompareVal, Offset)textureGatherOffset(Tex, _ToVec(Location), _ToFloat(CompareVal), Offset)

// Atomic operations
#define InterlockedAddSharedVar_2(dest, value)                      atomicAdd(dest, value)
#define InterlockedAddSharedVar_3(dest, value, orig_val) orig_val = atomicAdd(dest, value)
#define InterlockedAddImage_2(img, coords, value)                     imageAtomicAdd(img, _ToIvec(coords), value)
#define InterlockedAddImage_3(img, coords, value, orig_val)orig_val = imageAtomicAdd(img, _ToIvec(coords), value)

#define InterlockedAndSharedVar_2(dest, value)                      atomicAnd(dest, value)
#define InterlockedAndSharedVar_3(dest, value, orig_val) orig_val = atomicAnd(dest, value)
#define InterlockedAndImage_2(img, coords, value)                     imageAtomicAnd(img, _ToIvec(coords), value)
#define InterlockedAndImage_3(img, coords, value, orig_val)orig_val = imageAtomicAnd(img, _ToIvec(coords), value)

#define InterlockedMaxSharedVar_2(dest, value)                      atomicMax(dest, value)
#define InterlockedMaxSharedVar_3(dest, value, orig_val) orig_val = atomicMax(dest, value)
#define InterlockedMaxImage_2(img, coords, value)                     imageAtomicMax(img, _ToIvec(coords), value)
#define InterlockedMaxImage_3(img, coords, value, orig_val)orig_val = imageAtomicMax(img, _ToIvec(coords), value)

#define InterlockedMinSharedVar_2(dest, value)                      atomicMin(dest, value)
#define InterlockedMinSharedVar_3(dest, value, orig_val) orig_val = atomicMin(dest, value)
#define InterlockedMinImage_2(img, coords, value)                     imageAtomicMin(img, _ToIvec(coords), value)
#define InterlockedMinImage_3(img, coords, value, orig_val)orig_val = imageAtomicMin(img, _ToIvec(coords), value)

#define InterlockedOrSharedVar_2(dest, value)                      atomicOr(dest, value)
#define InterlockedOrSharedVar_3(dest, value, orig_val) orig_val = atomicOr(dest, value)
#define InterlockedOrImage_2(img, coords, value)                     imageAtomicOr(img, _ToIvec(coords), value)
#define InterlockedOrImage_3(img, coords, value, orig_val)orig_val = imageAtomicOr(img, _ToIvec(coords), value)

#define InterlockedXorSharedVar_2(dest, value)                      atomicXor(dest, value)
#define InterlockedXorSharedVar_3(dest, value, orig_val) orig_val = atomicXor(dest, value)
#define InterlockedXorImage_2(img, coords, value)                     imageAtomicXor(img, _ToIvec(coords), value)
#define InterlockedXorImage_3(img, coords, value, orig_val)orig_val = imageAtomicXor(img, _ToIvec(coords), value)

// There is actually no InterlockedExchange() with 2 arguments
#define InterlockedExchangeSharedVar_2(dest, value)                      atomicExchange(dest, value)
#define InterlockedExchangeSharedVar_3(dest, value, orig_val) orig_val = atomicExchange(dest, value)
#define InterlockedExchangeImage_2(img, coords, value)                     imageAtomicExchange(img, _ToIvec(coords), value)
#define InterlockedExchangeImage_3(img, coords, value, orig_val)orig_val = imageAtomicExchange(img, _ToIvec(coords), value)

//uint imageAtomicCompSwap(	image img,    IVec P,       nint compare,   nint data);
//void InterlockedCompareExchange(     in R dest, in T compare_value, in  T value, out T original_value);
#define InterlockedCompareExchangeSharedVar_4(dest, cmp_val, value, orig_val)  orig_val = atomicCompSwap(dest, cmp_val, value)
#define InterlockedCompareExchangeImage_4(img, coords, cmp_val, value, orig_val) orig_val = imageAtomicCompSwap(img, _ToIvec(coords), cmp_val, value)

#define InterlockedCompareStoreSharedVar_3(dest, cmp_val, value) atomicCompSwap(dest, cmp_val, value)
#define InterlockedCompareStoreImage_3(img, coords, cmp_val, value)imageAtomicCompSwap(img, _ToIvec(coords), cmp_val, value)


// Swizzling macros
#define _SWIZZLE0
#define _SWIZZLE1 .x
#define _SWIZZLE2 .xy
#define _SWIZZLE3 .xyz
#define _SWIZZLE4 .xyzw

// Helper functions

float2 NormalizedDeviceXYToTexUV( float2 f2ProjSpaceXY )
{
    return float2(0.5,0.5) + float2(0.5,0.5) * f2ProjSpaceXY.xy;
}

float NormalizedDeviceZToDepth(float fNDC_Z)
{
    return fNDC_Z * 0.5 + 0.5; // [-1, +1] -> [0, 1]
}

float DepthToNormalizedDeviceZ(float fDepth)
{
    return fDepth * 2.0 - 1.0; // [0, 1] -> [-1, +1]
}

#define F3NDC_XYZ_TO_UVD_SCALE float3(0.5, 0.5, 0.5)

#define NDC_MIN_Z -1.0 // Minimal z in the normalized device space

#define MATRIX_ELEMENT(mat, row, col) mat[col][row]

// ---------------------------------- Vertex shader ----------------------------------
#ifdef VERTEX_SHADER

#ifndef GL_ES
out gl_PerVertex
{
    vec4 gl_Position;
};
#endif

#define _GET_GL_VERTEX_ID(VertexId)_TypeConvertStore(VertexId, gl_VertexID)
#define _GET_GL_INSTANCE_ID(InstId)_TypeConvertStore(InstId, gl_InstanceID)
#define _SET_GL_POSITION(Pos)gl_Position=_ExpandVector(Pos)

#endif


// --------------------------------- Fragment shader ---------------------------------
#ifdef FRAGMENT_SHADER

#define _GET_GL_FRAG_COORD(FragCoord)_ResizeVector(FragCoord, gl_FragCoord)
#define _SET_GL_FRAG_DEPTH(Depth)_TypeConvertStore(gl_FragDepth, Depth)

#endif


// --------------------------------- Geometry shader ---------------------------------
#ifdef GEOMETRY_SHADER

// ARB_separate_shader_objects requires built-in block gl_PerVertex to be redeclared before accessing its members
// declaring gl_PointSize and gl_ClipDistance causes compilation error on Android
in gl_PerVertex
{
    vec4 gl_Position;
    //float gl_PointSize;
    //float gl_ClipDistance[];
} gl_in[];

out gl_PerVertex
{
    vec4 gl_Position;
    //float gl_PointSize;
    //float gl_ClipDistance[];
};

#define _GET_GL_POSITION(Pos)_ResizeVector(Pos, gl_in[i].gl_Position)
#define _GET_GL_PRIMITIVE_ID(PrimId)_TypeConvertStore(PrimId, gl_in[i].gl_PrimitiveIDIn)

#define _SET_GL_POSITION(Pos)gl_Position=_ExpandVector(Pos)
#define _SET_GL_LAYER(Layer)_TypeConvertStore(gl_Layer,Layer)

#endif


// --------------------------- Tessellation control shader ---------------------------
#ifdef TESS_CONTROL_SHADER

in gl_PerVertex
{
    vec4 gl_Position;
    //float gl_PointSize;
    //float gl_ClipDistance[];
} gl_in[gl_MaxPatchVertices];

out gl_PerVertex
{
    vec4 gl_Position;
    //float gl_PointSize;
    //float gl_ClipDistance[];
} gl_out[];

#define _GET_GL_INVOCATION_ID(InvocId)_TypeConvertStore(InvocId, gl_InvocationID)
#define _GET_GL_PRIMITIVE_ID(PrimId)_TypeConvertStore(PrimId, gl_PrimitiveID)
#define _GET_GL_POSITION(Pos)_ResizeVector(Pos, gl_in[i].gl_Position)

#define _SET_GL_POSITION(Pos)gl_out[gl_InvocationID].gl_Position=_ExpandVector(Pos)
#define _SET_GL_TESS_LEVEL_OUTER(OuterLevel)for(int i=0; i < OuterLevel.length(); ++i)gl_TessLevelOuter[i] = OuterLevel[i]

void _SetGLTessLevelInner(float InnerLevel[2])
{
    gl_TessLevelInner[0] = InnerLevel[0];
    gl_TessLevelInner[1] = InnerLevel[1];
}
void _SetGLTessLevelInner(float InnerLevel)
{
    gl_TessLevelInner[0] = InnerLevel;
}

#endif


// --------------------------- Tessellation evaluation shader ---------------------------
#ifdef TESS_EVALUATION_SHADER

in gl_PerVertex
{
    vec4 gl_Position;
    //float gl_PointSize;
    //float gl_ClipDistance[];
} gl_in[gl_MaxPatchVertices];

out gl_PerVertex
{
    vec4 gl_Position;
    //float gl_PointSize;
    //float gl_ClipDistance[];
};

#define _GET_GL_POSITION(Pos)_ResizeVector(Pos, gl_in[i].gl_Position)
#define _GET_GL_TESS_LEVEL_OUTER(OuterLevel)for(int i=0; i < OuterLevel.length(); ++i)OuterLevel[i] = gl_TessLevelOuter[i]
void _GetGLTessLevelInner(out float InnerLevel[2])
{
    InnerLevel[0] = gl_TessLevelInner[0];
    InnerLevel[1] = gl_TessLevelInner[1];
}
void _GetGLTessLevelInner(out float InnerLevel)
{
    InnerLevel = gl_TessLevelInner[0];
}

#define _GET_GL_TESS_COORD(TessCoord)_ResizeVector(TessCoord, gl_TessCoord)
#define _GET_GL_PRIMITIVE_ID(PrimId)_TypeConvertStore(PrimId, gl_PrimitiveID)

#define _SET_GL_POSITION(Pos)gl_Position=_ExpandVector(Pos)

#endif


// ---------------------------------- Compute shader ----------------------------------
#ifdef COMPUTE_SHADER

#define _GET_GL_GLOBAL_INVOCATION_ID(Type, InvocId)InvocId=Type(gl_GlobalInvocationID)
#define _GET_GL_WORK_GROUP_ID(Type, GroupId)GroupId=Type(gl_WorkGroupID)
#define _GET_GL_LOCAL_INVOCATION_ID(Type, InvocId)InvocId=Type(gl_LocalInvocationID)
#define _GET_GL_LOCAL_INVOCATION_INDEX(Type, InvocInd)InvocInd=Type(gl_LocalInvocationIndex)

#endif

#endif // _GLSL_DEFINITIONS_
/***//* Some comment * ** * * * / ** //// */ //Another comment
//
// Comment

/* More *//*com*///ments/**/
//
//


//--------------------------------------------------------------------------------------
// Copyright 2013 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
//--------------------------------------------------------------------------------------

#   ifndef _STRCUTURES_FXH_
#   define _STRCUTURES_FXH_

#define PI (3.1415928)

#ifdef __cplusplus

#ifndef BOOL
#   define BOOL int32_t // Do not use bool, because sizeof(bool)==1 !
#endif

#ifndef TRUE
#   define TRUE 1
#endif

#ifndef FALSE
#   define FALSE 0
#endif

#else

#   define BOOL bool

#endif

#ifdef __cplusplus
#   define CHECK_STRUCT_ALIGNMENT(s) static_assert( sizeof(s) % 16 == 0, sizeof(#s) is not multiple of 16 );
#else
#   define CHECK_STRUCT_ALIGNMENT(s)
#endif



#define MAX_CASCADES 8
struct CascadeAttribs
{
	float4 f4LightSpaceScale;
	float4 f4LightSpaceScaledBias;
    float4 f4StartEndZ;
};
#ifdef __cplusplus
static_assert( (sizeof(CascadeAttribs) % 16) == 0, sizeof(CascadeAttribs) is not multiple of 16 );
#endif

struct ShadowMapAttribs
{
    // 0
#ifdef __cplusplus
    float4x4 mWorldToLightView; // Matrices in HLSL are COLUMN-major while float4x4 is ROW major
#else
    matrix mWorldToLightView;  // Transform from view space to light projection space
#endif
    // 16
    CascadeAttribs Cascades[MAX_CASCADES];

#ifdef __cplusplus
    float fCascadeCamSpaceZEnd[MAX_CASCADES];
    float4x4 mWorldToShadowMapUVDepth[MAX_CASCADES];
#else
	float4 f4CascadeCamSpaceZEnd[MAX_CASCADES/4];
    matrix mWorldToShadowMapUVDepth[MAX_CASCADES];
#endif

    // Do not use bool, because sizeof(bool)==1 !
	BOOL bVisualizeCascades;

    float3 f3Padding;
};
#ifdef __cplusplus
static_assert( (sizeof(ShadowMapAttribs) % 16) == 0, sizeof(ShadowMapAttribs) is not multiple of 16 );
#endif


struct LightAttribs
{
    float4 f4DirOnLight;
    float4 f4AmbientLight;
    float4 f4LightScreenPos;
    float4 f4ExtraterrestrialSunColor;

    BOOL bIsLightOnScreen;
    float3 f3Dummy;

    ShadowMapAttribs ShadowAttribs;
};
CHECK_STRUCT_ALIGNMENT(LightAttribs);

struct CameraAttribs
{
    float4 f4CameraPos;            ///< Camera world position
    float fNearPlaneZ; 
    float fFarPlaneZ; // fNearPlaneZ < fFarPlaneZ
    float2 f2Dummy;

#ifdef __cplusplus
    float4x4 WorldViewProj;
    //float4x4 mView;
    float4x4 mProj;
    float4x4 mViewProjInv;
#else
    matrix WorldViewProj;
    //matrix mView;
    matrix mProj;
    matrix mViewProjInv;
#endif
};
CHECK_STRUCT_ALIGNMENT(CameraAttribs);

#define LIGHT_SCTR_TECHNIQUE_EPIPOLAR_SAMPLING 0
#define LIGHT_SCTR_TECHNIQUE_BRUTE_FORCE 1

#define CASCADE_PROCESSING_MODE_SINGLE_PASS 0
#define CASCADE_PROCESSING_MODE_MULTI_PASS 1
#define CASCADE_PROCESSING_MODE_MULTI_PASS_INST 2

#define REFINEMENT_CRITERION_DEPTH_DIFF 0
#define REFINEMENT_CRITERION_INSCTR_DIFF 1

// Extinction evaluation mode used when attenuating background
#define EXTINCTION_EVAL_MODE_PER_PIXEL 0// Evaluate extinction for each pixel using analytic formula 
                                        // by Eric Bruneton
#define EXTINCTION_EVAL_MODE_EPIPOLAR 1 // Render extinction in epipolar space and perform
                                        // bilateral filtering in the same manner as for
                                        // inscattering

#define SINGLE_SCTR_MODE_NONE 0
#define SINGLE_SCTR_MODE_INTEGRATION 1
#define SINGLE_SCTR_MODE_LUT 2

#define MULTIPLE_SCTR_MODE_NONE 0
#define MULTIPLE_SCTR_MODE_UNOCCLUDED 1
#define MULTIPLE_SCTR_MODE_OCCLUDED 2

#define TONE_MAPPING_MODE_EXP 0
#define TONE_MAPPING_MODE_REINHARD 1
#define TONE_MAPPING_MODE_REINHARD_MOD 2
#define TONE_MAPPING_MODE_UNCHARTED2 3
#define TONE_MAPPING_FILMIC_ALU 4
#define TONE_MAPPING_LOGARITHMIC 5
#define TONE_MAPPING_ADAPTIVE_LOG 6


struct PostProcessingAttribs
{
    uint m_uiNumEpipolarSlices;
    uint m_uiMaxSamplesInSlice;
    uint m_uiInitialSampleStepInSlice;
    uint m_uiEpipoleSamplingDensityFactor;

    float m_fRefinementThreshold;
    // do not use bool, because sizeof(bool)==1 and as a result bool variables
    // will be incorrectly mapped on GPU constant buffer
    BOOL m_bShowSampling; 
    BOOL m_bCorrectScatteringAtDepthBreaks; 
    BOOL m_bShowDepthBreaks; 

    BOOL m_bShowLightingOnly;
    BOOL m_bOptimizeSampleLocations;
    BOOL m_bEnableLightShafts;
    uint m_uiInstrIntegralSteps;
    
    float2 m_f2ShadowMapTexelSize;
    uint m_uiShadowMapResolution;
    uint m_uiMinMaxShadowMapResolution;

    BOOL m_bUse1DMinMaxTree;
    float m_fMaxShadowMapStep;
    float m_fMiddleGray;
    uint m_uiLightSctrTechnique;

    int m_iNumCascades;
    int m_iFirstCascade;
    float m_fNumCascades;
    float m_fFirstCascade;

    uint m_uiCascadeProcessingMode;
    uint m_uiRefinementCriterion;
    BOOL m_bIs32BitMinMaxMipMap;
    uint m_uiMultipleScatteringMode;

    uint m_uiSingleScatteringMode;
    BOOL m_bAutoExposure;
    uint m_uiToneMappingMode;
    BOOL m_bLightAdaptation;
    
    float m_fWhitePoint;
    float m_fLuminanceSaturation;
    float2 f2Dummy;
    
    uint m_uiExtinctionEvalMode;
    BOOL m_bUseCustomSctrCoeffs;
    float m_fAerosolDensityScale;
    float m_fAerosolAbsorbtionScale;

    float4 m_f4CustomRlghBeta;
    float4 m_f4CustomMieBeta;

#ifdef __cplusplus
    PostProcessingAttribs() : 
        m_uiNumEpipolarSlices(512),
        m_uiMaxSamplesInSlice(256),
        m_uiInitialSampleStepInSlice(16),
        // Note that sampling near the epipole is very cheap since only a few steps
        // required to perform ray marching
        m_uiEpipoleSamplingDensityFactor(2),
        m_fRefinementThreshold(0.03f),
        m_bShowSampling(FALSE),
        m_bCorrectScatteringAtDepthBreaks(FALSE),
        m_bShowDepthBreaks(FALSE),
        m_bShowLightingOnly(FALSE),
        m_bOptimizeSampleLocations(TRUE),
        m_bEnableLightShafts(TRUE),
        m_uiInstrIntegralSteps(30),
        m_bUse1DMinMaxTree(TRUE),
        m_fMaxShadowMapStep(16.f),
        m_f2ShadowMapTexelSize(0,0),
        m_uiMinMaxShadowMapResolution(0),
        m_fMiddleGray(.18f),
        m_uiLightSctrTechnique(LIGHT_SCTR_TECHNIQUE_EPIPOLAR_SAMPLING),
        m_iNumCascades(0),
        m_iFirstCascade(1),
        m_fNumCascades(0),
        m_fFirstCascade(1),
        m_uiCascadeProcessingMode(CASCADE_PROCESSING_MODE_SINGLE_PASS),
        m_uiRefinementCriterion(REFINEMENT_CRITERION_INSCTR_DIFF),
        m_bIs32BitMinMaxMipMap(FALSE),
        m_uiMultipleScatteringMode(MULTIPLE_SCTR_MODE_UNOCCLUDED),
        m_uiSingleScatteringMode(SINGLE_SCTR_MODE_LUT),
        m_bAutoExposure(TRUE),
        m_uiToneMappingMode(TONE_MAPPING_MODE_UNCHARTED2),
        m_bLightAdaptation(TRUE),
        m_fWhitePoint(3e+0f),
        m_fLuminanceSaturation(1e+0),
        m_uiExtinctionEvalMode(EXTINCTION_EVAL_MODE_EPIPOLAR),
        m_bUseCustomSctrCoeffs(FALSE),
        m_fAerosolDensityScale(1.f),
        m_fAerosolAbsorbtionScale(0.1f),
        m_f4CustomRlghBeta( 5.8e-6f, 13.5e-6f, 33.1e-6f, 0.f ),
        m_f4CustomMieBeta(2.e-5f, 2.e-5f, 2.e-5f, 0.f)
        {}
#endif
};
CHECK_STRUCT_ALIGNMENT(PostProcessingAttribs);

struct AirScatteringAttribs
{
    // Angular Rayleigh scattering coefficient contains all the terms exepting 1 + cos^2(Theta):
    // Pi^2 * (n^2-1)^2 / (2*N) * (6+3*Pn)/(6-7*Pn)
    float4 f4AngularRayleighSctrCoeff;
    // Total Rayleigh scattering coefficient is the integral of angular scattering coefficient in all directions
    // and is the following:
    // 8 * Pi^3 * (n^2-1)^2 / (3*N) * (6+3*Pn)/(6-7*Pn)
    float4 f4TotalRayleighSctrCoeff;
    float4 f4RayleighExtinctionCoeff;

    // Note that angular scattering coefficient is essentially a phase function multiplied by the
    // total scattering coefficient
    float4 f4AngularMieSctrCoeff;
    float4 f4TotalMieSctrCoeff;
    float4 f4MieExtinctionCoeff;

    float4 f4TotalExtinctionCoeff;
    // Cornette-Shanks phase function (see Nishita et al. 93) normalized to unity has the following form:
    // F(theta) = 1/(4*PI) * 3*(1-g^2) / (2*(2+g^2)) * (1+cos^2(theta)) / (1 + g^2 - 2g*cos(theta))^(3/2)
    float4 f4CS_g; // x == 3*(1-g^2) / (2*(2+g^2))
                   // y == 1 + g^2
                   // z == -2*g

    float fEarthRadius;
    float fAtmTopHeight;
    float2 f2ParticleScaleHeight;
    
    float fTurbidity;
    float fAtmTopRadius;
    float m_fAerosolPhaseFuncG;
    float m_fDummy;


#ifdef __cplusplus
    AirScatteringAttribs():        
        f2ParticleScaleHeight(7994.f, 1200.f),
        // Air molecules and aerosols are assumed to be distributed
        // between 6360 km and 6420 km
        fEarthRadius(6360000.f),
        fAtmTopHeight(80000.f),
        fTurbidity(1.02f),
        m_fAerosolPhaseFuncG(0.76f)
    {
        fAtmTopRadius = fEarthRadius + fAtmTopHeight;
    }
#endif
};

CHECK_STRUCT_ALIGNMENT(AirScatteringAttribs);

struct MiscDynamicParams
{
    float fMaxStepsAlongRay;   // Maximum number of steps during ray tracing
    float fCascadeInd;
    float2 f2WQ; // Used when pre-computing inscattering look-up table

    uint uiDepthSlice;
    float fElapsedTime;
    float2 f2Dummy;

#ifdef __cplusplus
    uint ui4SrcMinMaxLevelXOffset;
    uint ui4SrcMinMaxLevelYOffset;
    uint ui4DstMinMaxLevelXOffset;
    uint ui4DstMinMaxLevelYOffset;
#else
    uint4 ui4SrcDstMinMaxLevelOffset;
#endif
};
CHECK_STRUCT_ALIGNMENT(MiscDynamicParams);

#endif //_STRCUTURES_FXH_

// #include "NonExistingFile.h"


//#define TEXTURE2D Texture2D <- Macros do not work currently
//TEXTURE2D MacroTex2D;

/******//* /* /**** / */
void EmptyFunc(){}uniform cbTest1{int a;};uniform cbTest2{int b;};/*comment
test

**/uniform cbTest3{int c;};//Single line comment
uniform cbTest4{int d;};

uniform cbTest5
{
    float4 e;
};

uniform cbTest6
{
    float4 f;
};

int cbuffer_fake;
int fakecbuffer;

int GlobalIntVar;uniform sampler2D Tex2D_Test1;uniform sampler2D Tex2D_Test2;/*Comment* / *//* /** Comment2**/uniform sampler2D Tex2D_Test3;

uniform sampler2D Tex2D_M1;
uniform sampler2D Tex2D_M2;

// Test texture declaration

#ifndef GL_ES

uniform sampler1D Tex1D_F1;
uniform sampler1D Tex1D_F2;
uniform isampler1D Tex1D_I;
uniform usampler1D Tex1D_U;

SamplerState Tex1D_F1_sampler;

uniform sampler1DArray          Tex1D_F_A1;
uniform sampler1DArray  Tex1D_F_A2;
uniform isampler1DArray   Tex1D_I_A;
uniform usampler1DArray  Tex1D_U_A;

SamplerState Tex1D_F_A1_sampler;

uniform sampler1DShadow Tex1DS1;
uniform sampler1DShadow Tex1DS2;
uniform sampler1DShadow Tex1DS3;
SamplerComparisonState Tex1DS1_sampler, Tex1DS2_sampler, TestCmpSamplerArr[2], Tex1DS3_sampler;

uniform sampler1DArrayShadow Tex1DAS1;
SamplerComparisonState Tex1DAS1_sampler, Tex1DAS2_sampler;
uniform sampler1DArrayShadow Tex1DAS2;

#endif

uniform sampler2D Tex2D_F1;
uniform sampler2D Tex2D_F3[2];
uniform sampler2D Tex2D_F2;
uniform sampler2DShadow Tex2DS_F4;
uniform sampler2DShadow Tex2DS_F5;
uniform sampler2D Tex2D_F6;
uniform isampler2D Tex2D_I;
uniform usampler2D Tex2D_U;

SamplerState Tex2D_F1_sampler,Tex2D_F6_sampler;
SamplerComparisonState DummySampler, Tex2DS_F4_sampler,Tex2DS_F5_sampler;

uniform sampler2DArray          Tex2D_F_A1;
uniform sampler2DArray  Tex2D_F_A2;
uniform sampler2DArray Tex2D_F_A3;
uniform isampler2DArray   Tex2D_I_A;
uniform usampler2DArray  Tex2D_U_A;

SamplerState Tex2D_F_A1_sampler,Tex2D_F_A3_sampler;

uniform sampler2DMS Tex2DMS_F1;
uniform sampler2DMS Tex2DMS_F2;
uniform isampler2DMS   Tex2DMS_I;
uniform usampler2DMS Tex2DMS_U;

#ifndef GL_ES
uniform sampler2DMSArray Tex2DMS_F_A1;
uniform sampler2DMSArray  Tex2DMS_F_A2;
uniform isampler2DMSArray  Tex2DMS_I_A;
uniform usampler2DMSArray  Tex2DMS_U_A;
#endif

uniform sampler3D           Tex3D_F1;
uniform sampler3D Tex3D_F2;
uniform sampler3D Tex3D_F3;
uniform isampler3D    Tex3D_I;
uniform usampler3D  Tex3D_U;

SamplerState Tex3D_F1_sampler;

uniform samplerCube          TexC_F1;
uniform samplerCube TexC_F2;
uniform isamplerCube   TexC_I;
uniform usamplerCube   TexC_U;

SamplerState TexC_F1_sampler;

#ifndef GL_ES
uniform samplerCubeArray            TexC_F_A1;
uniform samplerCubeArray   TexC_F_A2;
uniform isamplerCubeArray   TexC_I_A;
uniform usamplerCubeArray   TexC_U_A;

SamplerState TexC_F_A1_sampler;
#endif

uniform sampler2DShadow Tex2DS1;
SamplerComparisonState Tex2DS1_sampler;
uniform sampler2DShadow Tex2DS2;
SamplerComparisonState Tex2DS2_sampler;

uniform sampler2DArrayShadow Tex2DAS1;
SamplerComparisonState Tex2DAS1_sampler;
uniform sampler2DArrayShadow Tex2DAS2;
SamplerComparisonState Tex2DAS2_sampler;

uniform samplerCubeShadow TexCS1;
SamplerComparisonState TexCS1_sampler;
uniform samplerCubeShadow TexCS2;
SamplerComparisonState TexCS2_sampler;

#ifndef GL_ES
uniform samplerCubeArrayShadow TexCAS1;
SamplerComparisonState TexCAS1_sampler;
uniform samplerCubeArrayShadow TexCAS2;
SamplerComparisonState TexCAS2_sampler;
#endif

int intvar1;SamplerState Dummy;int intvar2;

int Texture2D_fake, Texture2DArray_fake, fakeTexture2D, fakeTexture2DArray;
int Texture2DMS_fake;
int fakeTexture2DMS;
int Texture2DMSArray_fake;
int fakeTexture2DMSArray;
int Texture3D_fake;
int fakeTexture3D;
int TextureCube_fake, TextureCubeArray_fake;
int fakeTextureCube, fakeTextureCubeArray;
int SamplerState_fake;
int SamplerComparisonState_fake;
int fakeSamplerState;
int fakeSamplerComparisonState;
int Texture4D;
int Texture2d;
int TextureCub;
int Texture2DArr;
int Texture2DM;
int Texture2DMSArr;

void TestGetDimensions()
{
#ifndef GL_ES
    // Texture1D 
    {
        uint uWidth, uMipLevels;
        int iWidth, iMipLevels;
        float fWidth, fMipLevels;
        GetTex1DDimensions_1( Tex1D_F1,uWidth);
        GetTex1DDimensions_3( Tex1D_F1,0, uWidth, uMipLevels);
        GetTex1DDimensions_1( Tex1D_I,uWidth);
        GetTex1DDimensions_3( Tex1D_I,0, uWidth, uMipLevels);
        GetTex1DDimensions_1( Tex1D_U,uWidth);
        GetTex1DDimensions_3( Tex1D_U,0, uWidth, uMipLevels);
        GetTex1DDimensions_1( Tex1DS1,uWidth);
        GetTex1DDimensions_3( Tex1DS1,0, uWidth, uMipLevels);

        GetTex1DDimensions_1( Tex1D_F1,fWidth);
        GetTex1DDimensions_3( Tex1D_F1,0, fWidth, fMipLevels);
        GetTex1DDimensions_1( Tex1D_I,fWidth);
        GetTex1DDimensions_3( Tex1D_I,0, fWidth, fMipLevels);
        GetTex1DDimensions_1( Tex1D_U,fWidth);
        GetTex1DDimensions_3( Tex1D_U,0, fWidth, fMipLevels);
        GetTex1DDimensions_1( Tex1DS1,fWidth);
        GetTex1DDimensions_3( Tex1DS1,0, fWidth, fMipLevels);

        GetTex1DDimensions_1( Tex1D_F1,iWidth);
        GetTex1DDimensions_3( Tex1D_F1,0, iWidth, iMipLevels);
        GetTex1DDimensions_1( Tex1D_I,iWidth);
        GetTex1DDimensions_3( Tex1D_I,0, iWidth, iMipLevels);
        GetTex1DDimensions_1( Tex1D_U,iWidth);
        GetTex1DDimensions_3( Tex1D_U,0, iWidth, iMipLevels);
        GetTex1DDimensions_1( Tex1DS1,iWidth);
        GetTex1DDimensions_3( Tex1DS1,0, iWidth, iMipLevels);
    }

    // Texture1DArray
    {
        uint uWidth, uMipLevels, uElems;
        float fWidth, fMipLevels, fElems;
        int iWidth, iMipLevels, iElems;

        GetTex1DArrDimensions_4( Tex1D_F_A1,0, uWidth, uElems, uMipLevels);
        GetTex1DArrDimensions_2( Tex1D_F_A1,uWidth, uElems);
        GetTex1DArrDimensions_4( Tex1D_U_A,0, uWidth, uElems, uMipLevels);
        GetTex1DArrDimensions_2( Tex1D_U_A,uWidth, uElems);
        GetTex1DArrDimensions_4( Tex1D_I_A,0, uWidth, uElems, uMipLevels);
        GetTex1DArrDimensions_2( Tex1D_I_A,uWidth, uElems);
        GetTex1DArrDimensions_4( Tex1DAS1,0, uWidth, uElems, uMipLevels);
        GetTex1DArrDimensions_2( Tex1DAS1,uWidth, uElems);

        GetTex1DArrDimensions_4( Tex1D_F_A1,0, iWidth, iElems, iMipLevels);
        GetTex1DArrDimensions_2( Tex1D_F_A1,iWidth, iElems);
        GetTex1DArrDimensions_4( Tex1D_U_A,0, iWidth, iElems, iMipLevels);
        GetTex1DArrDimensions_2( Tex1D_U_A,iWidth, iElems);
        GetTex1DArrDimensions_4( Tex1D_I_A,0, iWidth, iElems, iMipLevels);
        GetTex1DArrDimensions_2( Tex1D_I_A,iWidth, iElems);
        GetTex1DArrDimensions_4( Tex1DAS1,0, iWidth, iElems, iMipLevels);
        GetTex1DArrDimensions_2( Tex1DAS1,iWidth, iElems);

        GetTex1DArrDimensions_4( Tex1D_F_A1,0, fWidth, fElems, fMipLevels);
        GetTex1DArrDimensions_2( Tex1D_F_A1,fWidth, fElems);
        GetTex1DArrDimensions_4( Tex1D_U_A,0, fWidth, fElems, fMipLevels);
        GetTex1DArrDimensions_2( Tex1D_U_A,fWidth, fElems);
        GetTex1DArrDimensions_4( Tex1D_I_A,0, fWidth, fElems, fMipLevels);
        GetTex1DArrDimensions_2( Tex1D_I_A,fWidth, fElems);
        GetTex1DArrDimensions_4( Tex1DAS1,0, fWidth, fElems, fMipLevels);
        GetTex1DArrDimensions_2( Tex1DAS1,fWidth, fElems);
    }
#endif

    //Texture2D
    {
        uint uWidth, uHeight, uMipLevels;
        int iWidth, iHeight, iMipLevels;
        float fWidth, fHeight, fMipLevels;

        GetTex2DDimensions_2( Tex2D_F1,uWidth, uHeight);
        GetTex2DDimensions_4( Tex2D_F1,0, uWidth, uHeight, uMipLevels);
        GetTex2DDimensions_4( Tex2D_F3[0], 0, uWidth, uHeight, uMipLevels );
        GetTex2DDimensions_2( Tex2D_I,uWidth, uHeight);
        GetTex2DDimensions_4( Tex2D_I,0, uWidth, uHeight, uMipLevels);
        GetTex2DDimensions_2( Tex2D_U,uWidth, uHeight);
        GetTex2DDimensions_4( Tex2D_U,0, uWidth, uHeight, uMipLevels);
        GetTex2DDimensions_2( Tex2DS1,uWidth, uHeight);
        GetTex2DDimensions_4( Tex2DS1,0, uWidth, uHeight, uMipLevels);

        GetTex2DDimensions_2( Tex2D_F1,iWidth, iHeight);
        GetTex2DDimensions_4( Tex2D_F1,0, iWidth, iHeight, iMipLevels);
        GetTex2DDimensions_4( Tex2D_F3[0], 0, iWidth, iHeight, iMipLevels );
        GetTex2DDimensions_2( Tex2D_I,iWidth, iHeight);
        GetTex2DDimensions_4( Tex2D_I,0, iWidth, iHeight, iMipLevels);
        GetTex2DDimensions_2( Tex2D_U,iWidth, iHeight);
        GetTex2DDimensions_4( Tex2D_U,0, iWidth, iHeight, iMipLevels);
        GetTex2DDimensions_2( Tex2DS1,iWidth, iHeight);
        GetTex2DDimensions_4( Tex2DS1,0, iWidth, iHeight, iMipLevels);


        GetTex2DDimensions_2( Tex2D_F1,fWidth, fHeight);
        GetTex2DDimensions_4( Tex2D_F1,0, fWidth, fHeight, fMipLevels);
        GetTex2DDimensions_4( Tex2D_F3[0], 0, fWidth, fHeight, fMipLevels );
        GetTex2DDimensions_2( Tex2D_I,fWidth, fHeight);
        GetTex2DDimensions_4( Tex2D_I,0, fWidth, fHeight, fMipLevels);
        GetTex2DDimensions_2( Tex2D_U,fWidth, fHeight);
        GetTex2DDimensions_4( Tex2D_U,0, fWidth, fHeight, fMipLevels);
        GetTex2DDimensions_2( Tex2DS1,fWidth, fHeight);
        GetTex2DDimensions_4( Tex2DS1,0, fWidth, fHeight, fMipLevels);
    }

    //Texture2DArray
    {
        uint uWidth, uHeight, uMipLevels, uElems;
        int iWidth, iHeight, iMipLevels, iElems;
        float fWidth, fHeight, fMipLevels, fElems;

        GetTex2DArrDimensions_5( Tex2D_F_A1,0, uWidth, uHeight, uElems, uMipLevels);
        GetTex2DArrDimensions_3( Tex2D_F_A1,uWidth, uHeight, uElems);
        GetTex2DArrDimensions_5( Tex2D_U_A,0, uWidth, uHeight, uElems, uMipLevels);
        GetTex2DArrDimensions_3( Tex2D_U_A,uWidth, uHeight, uElems);
        GetTex2DArrDimensions_5( Tex2D_I_A,0, uWidth, uHeight, uElems, uMipLevels);
        GetTex2DArrDimensions_3( Tex2D_I_A,uWidth, uHeight, uElems);
        GetTex2DArrDimensions_5( Tex2DAS1,0, uWidth, uHeight, uElems, uMipLevels);
        GetTex2DArrDimensions_3( Tex2DAS1,uWidth, uHeight, uElems);

        GetTex2DArrDimensions_5( Tex2D_F_A1,0, iWidth, iHeight, iElems, iMipLevels);
        GetTex2DArrDimensions_3( Tex2D_F_A1,iWidth, iHeight, iElems);
        GetTex2DArrDimensions_5( Tex2D_U_A,0, iWidth, iHeight, iElems, iMipLevels);
        GetTex2DArrDimensions_3( Tex2D_U_A,iWidth, iHeight, iElems);
        GetTex2DArrDimensions_5( Tex2D_I_A,0, iWidth, iHeight, iElems, iMipLevels);
        GetTex2DArrDimensions_3( Tex2D_I_A,iWidth, iHeight, iElems);
        GetTex2DArrDimensions_5( Tex2DAS1,0, iWidth, iHeight, iElems, iMipLevels);
        GetTex2DArrDimensions_3( Tex2DAS1,iWidth, iHeight, iElems);

        GetTex2DArrDimensions_5( Tex2D_F_A1,0, fWidth, fHeight, fElems, fMipLevels);
        GetTex2DArrDimensions_3( Tex2D_F_A1,fWidth, fHeight, fElems);
        GetTex2DArrDimensions_5( Tex2D_U_A,0, fWidth, fHeight, fElems, fMipLevels);
        GetTex2DArrDimensions_3( Tex2D_U_A,fWidth, fHeight, fElems);
        GetTex2DArrDimensions_5( Tex2D_I_A,0, fWidth, fHeight, fElems, fMipLevels);
        GetTex2DArrDimensions_3( Tex2D_I_A,fWidth, fHeight, fElems);
        GetTex2DArrDimensions_5( Tex2DAS1,0, fWidth, fHeight, fElems, fMipLevels);
        GetTex2DArrDimensions_3( Tex2DAS1,fWidth, fHeight, fElems);
    }

    //Texture3D
    {
        uint uWidth, uHeight, uDepth, uMipLevels;
        int iWidth, iHeight, iDepth, iMipLevels;
        float fWidth, fHeight, fDepth, fMipLevels;
        GetTex3DDimensions_5( Tex3D_F1,0, uWidth, uHeight, uDepth, uMipLevels);
        GetTex3DDimensions_3( Tex3D_F1,uWidth, uHeight, uDepth);
        GetTex3DDimensions_5( Tex3D_U,0, uWidth, uHeight, uDepth, uMipLevels);
        GetTex3DDimensions_3( Tex3D_U,uWidth, uHeight, uDepth);
        GetTex3DDimensions_5( Tex3D_I,0, uWidth, uHeight, uDepth, uMipLevels);
        GetTex3DDimensions_3( Tex3D_I,uWidth, uHeight, uDepth);

        GetTex3DDimensions_5( Tex3D_F1,0, iWidth, iHeight, iDepth, iMipLevels);
        GetTex3DDimensions_3( Tex3D_F1,iWidth, iHeight, iDepth);
        GetTex3DDimensions_5( Tex3D_U,0, iWidth, iHeight, iDepth, iMipLevels);
        GetTex3DDimensions_3( Tex3D_U,iWidth, iHeight, iDepth);
        GetTex3DDimensions_5( Tex3D_I,0, iWidth, iHeight, iDepth, iMipLevels);
        GetTex3DDimensions_3( Tex3D_I,iWidth, iHeight, iDepth);

        GetTex3DDimensions_5( Tex3D_F1,0, fWidth, fHeight, fDepth, fMipLevels);
        GetTex3DDimensions_3( Tex3D_F1,fWidth, fHeight, fDepth);
        GetTex3DDimensions_5( Tex3D_U,0, fWidth, fHeight, fDepth, fMipLevels);
        GetTex3DDimensions_3( Tex3D_U,fWidth, fHeight, fDepth);
        GetTex3DDimensions_5( Tex3D_I,0, fWidth, fHeight, fDepth, fMipLevels);
        GetTex3DDimensions_3( Tex3D_I,fWidth, fHeight, fDepth);
    }

    //TextureCube ~ Texture2D
    {
        uint uWidth, uHeight, uMipLevels;
        int iWidth, iHeight, iMipLevels;
        float fWidth, fHeight, fMipLevels;

        GetTex2DDimensions_4( TexC_F1,0, uWidth, uHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_F1,uWidth, uHeight);
        GetTex2DDimensions_4( TexC_I,0, uWidth, uHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_I,uWidth, uHeight);
        GetTex2DDimensions_4( TexC_U,0, uWidth, uHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_U,uWidth, uHeight);
        GetTex2DDimensions_4( TexCS1,0, uWidth, uHeight, uMipLevels);
        GetTex2DDimensions_2( TexCS1,uWidth, uHeight);

        GetTex2DDimensions_4( TexC_F1,0, iWidth, iHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_F1,iWidth, iHeight);
        GetTex2DDimensions_4( TexC_I,0, iWidth, iHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_I,iWidth, iHeight);
        GetTex2DDimensions_4( TexC_U,0, iWidth, iHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_U,iWidth, iHeight);
        GetTex2DDimensions_4( TexCS1,0, iWidth, iHeight, uMipLevels);
        GetTex2DDimensions_2( TexCS1,iWidth, iHeight);

        GetTex2DDimensions_4( TexC_F1,0, fWidth, fHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_F1,fWidth, fHeight);
        GetTex2DDimensions_4( TexC_I,0, fWidth, fHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_I,fWidth, fHeight);
        GetTex2DDimensions_4( TexC_U,0, fWidth, fHeight, uMipLevels);
        GetTex2DDimensions_2( TexC_U,fWidth, fHeight);
        GetTex2DDimensions_4( TexCS1,0, fWidth, fHeight, uMipLevels);
        GetTex2DDimensions_2( TexCS1,fWidth, fHeight);
    }

#ifndef GL_ES
    //TextureCubeArray ~ Texture2DArray
    {
        uint uWidth, uHeight, uMipLevels, uElems;
        float fWidth, fHeight, fMipLevels, fElems;
        int iWidth, iHeight, iMipLevels, iElems;

        GetTex2DArrDimensions_5( TexC_F_A1,0, uWidth, uHeight, uElems, uMipLevels);
        GetTex2DArrDimensions_3( TexC_F_A1,uWidth, uHeight, uElems);
        GetTex2DArrDimensions_5( TexC_I_A,0, uWidth, uHeight, uElems, uMipLevels);
        GetTex2DArrDimensions_3( TexC_I_A,uWidth, uHeight, uElems);
        GetTex2DArrDimensions_5( TexC_U_A,0, uWidth, uHeight, uElems, uMipLevels);
        GetTex2DArrDimensions_3( TexC_U_A,uWidth, uHeight, uElems);
        GetTex2DArrDimensions_5( TexCAS1,0, uWidth, uHeight, uElems, uMipLevels);
        GetTex2DArrDimensions_3( TexCAS1,uWidth, uHeight, uElems);

        GetTex2DArrDimensions_5( TexC_F_A1,0, iWidth, iHeight, iElems, iMipLevels);
        GetTex2DArrDimensions_3( TexC_F_A1,iWidth, iHeight, iElems);
        GetTex2DArrDimensions_5( TexC_I_A,0, iWidth, iHeight, iElems, iMipLevels);
        GetTex2DArrDimensions_3( TexC_I_A,iWidth, iHeight, iElems);
        GetTex2DArrDimensions_5( TexC_U_A,0, iWidth, iHeight, iElems, iMipLevels);
        GetTex2DArrDimensions_3( TexC_U_A,iWidth, iHeight, iElems);
        GetTex2DArrDimensions_5( TexCAS1,0, iWidth, iHeight, iElems, iMipLevels);
        GetTex2DArrDimensions_3( TexCAS1,iWidth, iHeight, iElems);

        GetTex2DArrDimensions_5( TexC_F_A1,0, fWidth, fHeight, fElems, fMipLevels);
        GetTex2DArrDimensions_3( TexC_F_A1,fWidth, fHeight, fElems);
        GetTex2DArrDimensions_5( TexC_I_A,0, fWidth, fHeight, fElems, fMipLevels);
        GetTex2DArrDimensions_3( TexC_I_A,fWidth, fHeight, fElems);
        GetTex2DArrDimensions_5( TexC_U_A,0, fWidth, fHeight, fElems, fMipLevels);
        GetTex2DArrDimensions_3( TexC_U_A,fWidth, fHeight, fElems);
        GetTex2DArrDimensions_5( TexCAS1,0, fWidth, fHeight, fElems, fMipLevels);
        GetTex2DArrDimensions_3( TexCAS1,fWidth, fHeight, fElems);
    }
#endif


#ifndef GL_ES // This should work on ES3.1, but compiler fails for no reason
    // Texture2DMS
    {
        uint uWidth, uHeight, uNumSamples;
        float fWidth, fHeight, fNumSamples;
        int iWidth, iHeight, iNumSamples;
        GetTex2DMSDimensions_3( Tex2DMS_F1,uWidth, uHeight, uNumSamples);
        GetTex2DMSDimensions_3( Tex2DMS_I,uWidth, uHeight, uNumSamples);
        GetTex2DMSDimensions_3( Tex2DMS_U,uWidth, uHeight, uNumSamples);

        GetTex2DMSDimensions_3( Tex2DMS_F1,fWidth, fHeight, fNumSamples);
        GetTex2DMSDimensions_3( Tex2DMS_I,fWidth, fHeight, fNumSamples);
        GetTex2DMSDimensions_3( Tex2DMS_U,fWidth, fHeight, fNumSamples);

        GetTex2DMSDimensions_3( Tex2DMS_F1,iWidth, iHeight, iNumSamples);
        GetTex2DMSDimensions_3( Tex2DMS_I,iWidth, iHeight, iNumSamples);
        GetTex2DMSDimensions_3( Tex2DMS_U,iWidth, iHeight, iNumSamples);
    }
#endif

#ifndef GL_ES
    // Texture2DMSArray
    {
        uint uWidth, uHeight, uElems, uNumSamples;
        int iWidth, iHeight, iElems, iNumSamples;
        float fWidth, fHeight, fElems, fNumSamples;
        GetTex2DMSArrDimensions_4( Tex2DMS_F_A1,uWidth, uHeight, uElems, uNumSamples);
        GetTex2DMSArrDimensions_4( Tex2DMS_I_A,uWidth, uHeight, uElems, uNumSamples);
        // OpenGL4.2 only supports 32 texture units and this one is 33rd:
        // Tex2DMS_U_A.GetDimensions(Width, Height, Elems, NumSamples);

        GetTex2DMSArrDimensions_4( Tex2DMS_F_A1,fWidth, fHeight, fElems, fNumSamples);
        GetTex2DMSArrDimensions_4( Tex2DMS_I_A,fWidth, fHeight, fElems, fNumSamples);

        GetTex2DMSArrDimensions_4( Tex2DMS_F_A1,iWidth, iHeight, iElems, iNumSamples);
        GetTex2DMSArrDimensions_4( Tex2DMS_I_A,iWidth, iHeight, iElems, iNumSamples);
    }
#endif
}



void TestSample()
{
    float2 f2UV = float2(0.2, 0.3);
    float3 f3UVW = float3(0.2, 0.3, 0.5);
    int3 Offset = int3(3, 6, 2);
    float4 f4UVWQ = float4(0.2, 0.3, 0.5, 10.0);

#ifndef GL_ES
    // Texture1D 
    Sample_2( Tex1D_F1,Tex1D_F1_sampler, f3UVW.x)_SWIZZLE0;
    Sample_3( Tex1D_F1,Tex1D_F1_sampler, f3UVW.x, Offset.x)_SWIZZLE0;

    // Texture1DArray
    Sample_2( Tex1D_F_A1,Tex1D_F_A1_sampler, f3UVW.xy)_SWIZZLE0;
    Sample_3( Tex1D_F_A1,Tex1D_F_A1_sampler, f3UVW.xy, Offset.x)_SWIZZLE0;
#endif

    //Texture2D
    Sample_2( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy )_SWIZZLE0.xyzw.xyzw;
    Sample_3( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, Offset.xy )_SWIZZLE0;
    Sample_2( Tex2D_F1, Tex2D_F1_sampler, float2(0.1, 0.3) )_SWIZZLE0;
    Sample_3( Tex2D_F1, Tex2D_F1_sampler, float2(0.1, (0.3+0.1) ), int2( (3-1) ,5) )_SWIZZLE0;

    //Texture2DArray
    Sample_2( Tex2D_F_A1, Tex2D_F_A1_sampler, f3UVW.xyz )_SWIZZLE0;
    Sample_3( Tex2D_F_A1, Tex2D_F_A1_sampler, f3UVW.xyz, Offset.xy )_SWIZZLE0;

    //Texture3D
    Sample_2( Tex3D_F1,Tex3D_F1_sampler, f3UVW.xyz )_SWIZZLE0;
    Sample_3( Tex3D_F1,Tex3D_F1_sampler, f3UVW.xyz, Offset.xyz )_SWIZZLE0;
    
    //TextureCube
    Sample_2( TexC_F1,TexC_F1_sampler, f3UVW.xyz )_SWIZZLE0;
    // Offset not supported

#ifndef GL_ES
    // TextureCubeArray
    Sample_2( TexC_F_A1, TexC_F_A1_sampler, f4UVWQ.xyzw )_SWIZZLE0;
    Sample_2( TexC_F_A1, TexC_F_A1_sampler, float4(0.5, (0.2+(0.01+0.02)), 0.4, 7.0) )_SWIZZLE0;
    // Offset not supported
#endif
}



void TestSampleBias()
{
    float2 f2UV = float2(0.2, 0.3);
    float3 f3UVW = float3(0.2, 0.3, 0.5);
    int3 Offset = int3(3, 6, 2);
    float4 f4UVWQ = float4(0.2, 0.3, 0.5, 10.0);

#ifndef GL_ES
    // Texture1D 
    SampleBias_3( Tex1D_F1,Tex1D_F1_sampler, f3UVW.x, 1.5)_SWIZZLE0;
    SampleBias_4( Tex1D_F1,Tex1D_F1_sampler, f3UVW.x, 1.5, Offset.x)_SWIZZLE0;

    // Texture1DArray
    SampleBias_3( Tex1D_F_A1,Tex1D_F_A1_sampler, f3UVW.xy, 1.5)_SWIZZLE0;
    SampleBias_4( Tex1D_F_A1,Tex1D_F_A1_sampler, f3UVW.xy, 1.5, Offset.x)_SWIZZLE0;
#endif

    //Texture2D
    SampleBias_3( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, 1.5 )_SWIZZLE0;
    SampleBias_4( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, 1.5, Offset.xy )_SWIZZLE0;
    SampleBias_3( Tex2D_F1, Tex2D_F1_sampler, float2(0.1, 0.3), 1.5 )_SWIZZLE0;
    SampleBias_4( Tex2D_F1, Tex2D_F1_sampler, float2(0.1, (0.3+0.1) ), 1.5, int2( (3-1) ,5) )_SWIZZLE0;

    //Texture2DArray
    SampleBias_3( Tex2D_F_A1, Tex2D_F_A1_sampler, f3UVW.xyz, 1.5 )_SWIZZLE0;
    SampleBias_4( Tex2D_F_A1, Tex2D_F_A1_sampler, f3UVW.xyz, 1.5, Offset.xy )_SWIZZLE0;

    //Texture3D
    SampleBias_3( Tex3D_F1,Tex3D_F1_sampler, f3UVW.xyz, 1.5 )_SWIZZLE0;
    SampleBias_4( Tex3D_F1,Tex3D_F1_sampler, f3UVW.xyz, 1.5, Offset.xyz )_SWIZZLE0;
    
    //TextureCube
    SampleBias_3( TexC_F1,TexC_F1_sampler, f3UVW.xyz, 1.5 )_SWIZZLE0;
    // Offset not supported

#ifndef GL_ES
    // TextureCubeArray
    SampleBias_3( TexC_F_A1, TexC_F_A1_sampler, f4UVWQ.xyzw, 1.5 )_SWIZZLE0;
    SampleBias_3( TexC_F_A1, TexC_F_A1_sampler, float4(0.5, (0.2+(0.01+0.02)), 0.4, 7.0), 1.6 )_SWIZZLE0;
    // Offset not supported
#endif
}

void TestSampleLevel()
{
    float2 f2UV = float2(0.2, 0.3);
    float3 f3UVW = float3(0.2, 0.3, 0.5);
    float Level = 1.8;
    int3 Offset = int3(3, 6, 2);
    float4 f4UVWQ = float4(0.2, 0.3, 0.5, 10.0);

#ifndef GL_ES
    // Texture1D 
    SampleLevel_3( Tex1D_F1,Tex1D_F1_sampler, f3UVW.x, Level)_SWIZZLE0;
    SampleLevel_4( Tex1D_F1,Tex1D_F1_sampler, f3UVW.x, Level, Offset.x)_SWIZZLE0;

    // Texture1DArray
    SampleLevel_3( Tex1D_F_A1,Tex1D_F_A1_sampler, f3UVW.xy, Level)_SWIZZLE0;
    SampleLevel_4( Tex1D_F_A1,Tex1D_F_A1_sampler, f3UVW.xy, Level, Offset.x)_SWIZZLE0;
#endif

    //Texture2D
    SampleLevel_3( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, Level )_SWIZZLE0;
    SampleLevel_3( Tex2D_F1, Tex2D_F1_sampler, SampleLevel_3( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, Level )_SWIZZLE0.xy + SampleLevel_3( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, Level )_SWIZZLE0.xy, Level )_SWIZZLE0;
    SampleLevel_4( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, Level, Offset.xy )_SWIZZLE0;
    SampleLevel_3( Tex2D_F1, Tex2D_F1_sampler, float2(0.1, 0.3), 4.0 )_SWIZZLE0;
    SampleLevel_4( Tex2D_F1, Tex2D_F1_sampler, float2(0.1, (0.3+0.1) ), 4.0, int2( (3-1) ,5) )_SWIZZLE0;

    //Texture2DArray
    SampleLevel_3( Tex2D_F_A1, Tex2D_F_A1_sampler, f3UVW.xyz, Level )_SWIZZLE0;
    SampleLevel_4( Tex2D_F_A1, Tex2D_F_A1_sampler, f3UVW.xyz, Level, Offset.xy )_SWIZZLE0;

    //Texture3D
    SampleLevel_3( Tex3D_F1,Tex3D_F1_sampler, f3UVW.xyz, Level )_SWIZZLE0;
    SampleLevel_4( Tex3D_F1,Tex3D_F1_sampler, f3UVW.xyz, Level, Offset.xyz )_SWIZZLE0;
    
    //TextureCube
    SampleLevel_3( TexC_F1,TexC_F1_sampler, f3UVW.xyz, Level )_SWIZZLE0;
    // Offset not supported

#ifndef GL_ES
    // TextureCubeArray
    SampleLevel_3( TexC_F_A1, TexC_F_A1_sampler, f4UVWQ.xyzw, Level )_SWIZZLE0;
    SampleLevel_3( TexC_F_A1, TexC_F_A1_sampler, float4(0.5, (0.2+(0.01+0.02)), 0.4, 7.0), 5 )_SWIZZLE0;
    // Offset not supported
#endif
}


void TestSampleGrad()
{
    float2 f2UV = float2(0.2, 0.3);
    float2 f2ddxUV = float2(0.01, -0.02);
    float2 f2ddyUV = float2(-0.01, 0.01);
    float3 f3UVW = float3(0.2, 0.3, 0.5);
    float3 f3ddxUVW = float3(-0.02, 0.03, 0.05);
    float3 f3ddyUVW = float3( 0.01, -0.02, 0.02);
    int3 Offset = int3(3, 6, 2);
    float4 f4UVWQ = float4(0.2, 0.3, 0.5, 10.0);

#ifndef GL_ES
    // Texture1D 
    SampleGrad_4( Tex1D_F1,Tex1D_F1_sampler, f3UVW.x, f2ddxUV.x, f2ddyUV.x)_SWIZZLE0;
    SampleGrad_5( Tex1D_F1,Tex1D_F1_sampler, f3UVW.x, f2ddxUV.x, f2ddyUV.x, Offset.x)_SWIZZLE0;

    // Texture1DArray
    SampleGrad_4( Tex1D_F_A1,Tex1D_F_A1_sampler, f3UVW.xy, f2ddxUV.x, f2ddyUV.x)_SWIZZLE0;
    SampleGrad_5( Tex1D_F_A1,Tex1D_F_A1_sampler, f3UVW.xy, f2ddxUV.x, f2ddyUV.x, Offset.x)_SWIZZLE0;
#endif

    //Texture2D
    SampleGrad_4( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, f2ddxUV.xy, f2ddyUV.xy )_SWIZZLE0;
    SampleGrad_5( Tex2D_F1, Tex2D_F1_sampler, f3UVW.xy, f2ddxUV.xy, f2ddyUV.xy, Offset.xy )_SWIZZLE0;
    SampleGrad_4( Tex2D_F1, Tex2D_F1_sampler, float2(0.1, 0.3), float2(0.01, 0.02), float2(-0.02, 0.01) )_SWIZZLE0;
    SampleGrad_5( Tex2D_F1, Tex2D_F1_sampler, float2(0.1, (0.3+0.1) ), float2(0.01, 0.02), float2(-0.02, 0.01), int2( (3-1) ,5) )_SWIZZLE0;

    //Texture2DArray
    SampleGrad_4( Tex2D_F_A1, Tex2D_F_A1_sampler, f3UVW.xyz, f2ddxUV.xy, f2ddyUV.xy )_SWIZZLE0;
    SampleGrad_5( Tex2D_F_A1, Tex2D_F_A1_sampler, f3UVW.xyz, f2ddxUV.xy, f2ddyUV.xy, Offset.xy )_SWIZZLE0;

    //Texture3D
    SampleGrad_4( Tex3D_F1,Tex3D_F1_sampler, f3UVW.xyz, f3ddxUVW.xyz, f3ddyUVW.xyz )_SWIZZLE0;
    SampleGrad_5( Tex3D_F1,Tex3D_F1_sampler, f3UVW.xyz, f3ddxUVW.xyz, f3ddyUVW.xyz, Offset.xyz )_SWIZZLE0;
    
    //TextureCube
    SampleGrad_4( TexC_F1,TexC_F1_sampler, f3UVW.xyz, f3ddxUVW.xyz, f3ddyUVW.xyz )_SWIZZLE0;
    // Offset not supported

#ifndef GL_ES
    // TextureCubeArray
    SampleGrad_4( TexC_F_A1, TexC_F_A1_sampler, f4UVWQ.xyzw, f3ddxUVW.xyz, f3ddyUVW.xyz )_SWIZZLE0;
    SampleGrad_4( TexC_F_A1, TexC_F_A1_sampler, float4(0.5, (0.2+(0.01+0.02)), 0.4, 7.0), float3(0.01,0.02,0.03), float3(-0.01,-0.02,-0.03) )_SWIZZLE0;
    // Offset not supported
#endif
}



void TestSampleCmp()
{
    float3 f3UVW = float3(0.2, 0.3, 0.5);
    int3 Offset = int3(3, 6, 2);
    float4 f4UVWQ = float4(0.2, 0.3, 0.5, 10.0);
    float CompareVal = 0.7;

#ifndef GL_ES
    // Texture1D 
    SampleCmpTex1D_3( Tex1DS1,Tex1DS1_sampler, f3UVW.x, CompareVal);
    SampleCmpTex1D_4( Tex1DS1,Tex1DS1_sampler, f3UVW.x, CompareVal, Offset.x);

    // Texture1DArray
    SampleCmpTex1DArr_3( Tex1DAS1,Tex1DAS1_sampler, f3UVW.xy, CompareVal);
    SampleCmpTex1DArr_4( Tex1DAS1,Tex1DAS1_sampler, f3UVW.xy, CompareVal, Offset.x);
#endif

    //Texture2D
    SampleCmpTex2D_3( Tex2DS1, Tex2DS1_sampler, f3UVW.xy, CompareVal );
    SampleCmpTex2D_4( Tex2DS1, Tex2DS1_sampler, f3UVW.xy, CompareVal, Offset.xy );
    SampleCmpTex2D_3( Tex2DS1, Tex2DS1_sampler, float2(0.1, 0.3), 4.0 );
    SampleCmpTex2D_4( Tex2DS1, Tex2DS1_sampler, float2(0.1, (0.3+0.1) ), 4.0, int2( (3-1) ,5) );
    SampleCmpTex2D_3( Tex2DS_F5, Tex2DS_F5_sampler, f3UVW.xy, CompareVal );

    //Texture2DArray
    SampleCmpTex2DArr_3( Tex2DAS1, Tex2DAS1_sampler, f3UVW.xyz, CompareVal );
    // This seems to be another bug on Intel driver: the following line does not compile:
    // Tex2DAS1.SampleCmp( Tex2DAS1_sampler, f3UVW.xyz, CompareVal, Offset.xy );

    //TextureCube
    SampleCmpTexCube_3( TexCS1,TexCS1_sampler, f3UVW.xyz, CompareVal );
    // Offset not supported

#ifndef GL_ES
    // TextureCubeArray
    SampleCmpTexCubeArr_3( TexCAS1, TexCAS1_sampler, f4UVWQ.xyzw, CompareVal );
    SampleCmpTexCubeArr_3( TexCAS1, TexCAS1_sampler, float4(0.5, (0.2+(0.01+0.02)), 0.4, 7.0), 0.5 );
    // Offset not supported
#endif
}



void TestSampleCmpLevelZero()
{
    float3 f3UVW = float3(0.2, 0.3, 0.5);
    int3 Offset = int3(3, 6, 2);
    float4 f4UVWQ = float4(0.2, 0.3, 0.5, 10.0);
    float CompareVal = 0.7;

#ifndef GL_ES
    // Texture1D 
    SampleCmpLevel0Tex1D_3( Tex1DS1,Tex1DS1_sampler, f3UVW.x, CompareVal);
    SampleCmpLevel0Tex1D_4( Tex1DS1,Tex1DS1_sampler, f3UVW.x, CompareVal, Offset.x);

    // Texture1DArray
    SampleCmpLevel0Tex1DArr_3( Tex1DAS1,Tex1DAS1_sampler, f3UVW.xy, CompareVal);
    SampleCmpLevel0Tex1DArr_4( Tex1DAS1,Tex1DAS1_sampler, f3UVW.xy, CompareVal, Offset.x);
#endif

    //Texture2D
    SampleCmpLevel0Tex2D_3( Tex2DS1, Tex2DS1_sampler, f3UVW.xy, CompareVal );
    SampleCmpLevel0Tex2D_4( Tex2DS1, Tex2DS1_sampler, f3UVW.xy, CompareVal, Offset.xy );
    SampleCmpLevel0Tex2D_3( Tex2DS1, Tex2DS1_sampler, float2(0.1, 0.3), 4.0 );
    SampleCmpLevel0Tex2D_4( Tex2DS1, Tex2DS1_sampler, float2(0.1, (0.3+0.1) ), 4.0, int2( (3-1) ,5) );

    //Texture2DArray
    SampleCmpLevel0Tex2DArr_3( Tex2DAS1, Tex2DAS1_sampler, f3UVW.xyz, CompareVal );
    // This seems to be another bug on Intel driver: the following line does not compile:
    // Tex2DAS1.SampleCmpLevelZero( Tex2DAS1_sampler, f3UVW.xyz, CompareVal, Offset.xy );

    //TextureCube
    SampleCmpLevel0TexCube_3( TexCS1,TexCS1_sampler, f3UVW.xyz, CompareVal );
    // Offset not supported

#ifndef GL_ES
    // TextureCubeArray
    SampleCmpLevel0TexCubeArr_3( TexCAS1, TexCAS1_sampler, f4UVWQ.xyzw, CompareVal );
    SampleCmpLevel0TexCubeArr_3( TexCAS1, TexCAS1_sampler, float4(0.5, (0.2+(0.01+0.02)), 0.4, 7.0), 0.5 );
    // Offset not supported
#endif
}



void TestLoad()
{
    int4 Location = int4(2, 5, 1, 10);
    int3 Offset = int3(5, 10, 20);

#ifndef GL_ES
    // Texture1D 
    {
        LoadTex1D_1( Tex1D_F1,Location.xy)_SWIZZLE0;
        LoadTex1D_2( Tex1D_F1,Location.xy, Offset.x)_SWIZZLE0;
        LoadTex1D_1( Tex1D_I,Location.xy)_SWIZZLE2;
        LoadTex1D_2( Tex1D_I,Location.xy, Offset.x)_SWIZZLE2;
        LoadTex1D_1( Tex1D_U,Location.xy)_SWIZZLE4;
        LoadTex1D_2( Tex1D_U,Location.xy, Offset.x)_SWIZZLE4;
    }

    // Texture1DArray
    {
        LoadTex1DArr_1( Tex1D_F_A1,Location.xyz)_SWIZZLE0;
        LoadTex1DArr_2( Tex1D_F_A1,Location.xyz, Offset.x)_SWIZZLE0;
        LoadTex1DArr_1( Tex1D_U_A,Location.xyz)_SWIZZLE4;
        LoadTex1DArr_2( Tex1D_U_A,Location.xyz, Offset.x)_SWIZZLE4;
        LoadTex1DArr_1( Tex1D_I_A,Location.xyz)_SWIZZLE2;
        LoadTex1DArr_2( Tex1D_I_A,Location.xyz, Offset.x)_SWIZZLE2;
    }
#endif

    //Texture2D
    {
        LoadTex2D_1( Tex2D_F1,Location.xyz)_SWIZZLE0;
        LoadTex2D_2( Tex2D_F1,Location.xyz, Offset.xy)_SWIZZLE0;
        LoadTex2D_1( Tex2D_F1,LoadTex2D_1( Tex2D_I,Location.xyz)_SWIZZLE3.xyz + LoadTex2D_1( Tex2D_I,Location.xyz)_SWIZZLE3.xyz)_SWIZZLE0;
        LoadTex2D_1( Tex2D_I,Location.xyz)_SWIZZLE3;
        LoadTex2D_2( Tex2D_I,Location.xyz, Offset.xy)_SWIZZLE3;
        LoadTex2D_1( Tex2D_U,Location.xyz)_SWIZZLE4;
        LoadTex2D_2( Tex2D_U,Location.xyz, Offset.xy)_SWIZZLE4;
    }

    //Texture2DArray
    {
        LoadTex2DArr_1( Tex2D_F_A1,Location.xyzw)_SWIZZLE0;
        LoadTex2DArr_2( Tex2D_F_A1,Location.xyzw, Offset.xy)_SWIZZLE0;
        LoadTex2DArr_1( Tex2D_U_A,Location.xyzw)_SWIZZLE4;
        LoadTex2DArr_2( Tex2D_U_A,Location.xyzw, Offset.xy)_SWIZZLE4;
        LoadTex2DArr_1( Tex2D_I_A,Location.xyzw)_SWIZZLE2;
        LoadTex2DArr_2( Tex2D_I_A,Location.xyzw, Offset.xy)_SWIZZLE2;
    }

    //Texture3D
    {
        LoadTex3D_1( Tex3D_F1,Location.xyzw)_SWIZZLE0;
        LoadTex3D_2( Tex3D_F1,Location.xyzw, Offset.xyz)_SWIZZLE0;
        LoadTex3D_1( Tex3D_U,Location.xyzw)_SWIZZLE2;
        LoadTex3D_2( Tex3D_U,Location.xyzw, Offset.xyz)_SWIZZLE2;
        LoadTex3D_1( Tex3D_I,Location.xyzw)_SWIZZLE1;
        LoadTex3D_2( Tex3D_I,Location.xyzw, Offset.xyz)_SWIZZLE1;
    }

#ifndef GL_ES // This should work on ES3.1, but compiler fails for no reason
    // Texture2DMS
    {
        LoadTex2DMS_2( Tex2DMS_F1,Location.xy, 1)_SWIZZLE2;
        LoadTex2DMS_3( Tex2DMS_F1,Location.xy, 1, Offset.xy)_SWIZZLE2;
        LoadTex2DMS_2( Tex2DMS_I,Location.xy, 1)_SWIZZLE1;
        LoadTex2DMS_3( Tex2DMS_I,Location.xy, 1, Offset.xy)_SWIZZLE1;
        LoadTex2DMS_2( Tex2DMS_U,Location.xy, 1)_SWIZZLE1;
        LoadTex2DMS_3( Tex2DMS_U,Location.xy, 1, Offset.xy)_SWIZZLE1;
    }
#endif

#ifndef GL_ES
    // Texture2DMSArray
    {
        LoadTex2DMSArr_2( Tex2DMS_F_A1,Location.xyz, 1)_SWIZZLE3;
        LoadTex2DMSArr_3( Tex2DMS_F_A1,Location.xyz, 1, Offset.xy)_SWIZZLE3;
        LoadTex2DMSArr_2( Tex2DMS_I_A,Location.xyz, 1)_SWIZZLE2;
        LoadTex2DMSArr_3( Tex2DMS_I_A,Location.xyz, 1, Offset.xy)_SWIZZLE2;
        LoadTex2DMSArr_2( Tex2DMS_U_A,Location.xyz, 1)_SWIZZLE4;
        LoadTex2DMSArr_3( Tex2DMS_U_A,Location.xyz, 1, Offset.xy)_SWIZZLE4;
    }
#endif
}




void TestGather()
{
    float4 Location = float4(0.2, 0.5, 0.1, 0.7);
    int3 Offset = int3(5, 10, 20);
    
    //Texture2D
    {
        Gather_2( Tex2D_F1,Tex2D_F1_sampler, Location.xy);
        Gather_3( Tex2D_F1,Tex2D_F1_sampler, Location.xy, Offset.xy);
        //Tex2D_I.Gather(Location.xyz);
        //Tex2D_I.Gather(Location.xyz, Offset.xy);
        //Tex2D_U.Gather(Location.xyz);
        //Tex2D_U.Gather(Location.xyz, Offset.xy);
    }

    //Texture2DArray
    {
        Gather_2( Tex2D_F_A1,Tex2D_F_A1_sampler, Location.xyz);
        Gather_3( Tex2D_F_A1,Tex2D_F_A1_sampler, Location.xyz, Offset.xy);
        //Tex2D_U_A.Gather(Location.xyzw);
        //Tex2D_U_A.Gather(Location.xyzw, Offset.xy);
        //Tex2D_I_A.Gather(Location.xyzw);
        //Tex2D_I_A.Gather(Location.xyzw, Offset.xy);
    }

    // TextureCube
    {
        Gather_2( TexC_F1,TexC_F1_sampler, Location.xyz);
        //TexC_I.Gather(Location.xyz);
        //TexC_U.Gather(Location.xyz);
    }
#ifndef GL_ES 
    // TextureCubeArray
    {
        Gather_2( TexC_F_A1,TexC_F_A1_sampler, Location.xyzw);
        //TexC_I_A.Gather(Location.xyzw);
        //TexC_U_A.Gather(Location.xyzw);
    }
#endif
}



void TestGatherCmp()
{
    float4 Location = float4(0.2, 0.5, 0.1, 0.7);
    int3 Offset = int3(5, 10, 20);
    float CompareVal = 0.01;

    //Texture2D
    {
        GatherCmp_3( Tex2DS1,Tex2DS1_sampler, Location.xy, CompareVal);
        GatherCmp_4( Tex2DS1,Tex2DS1_sampler, Location.xy, CompareVal, Offset.xy);
    }

    //Texture2DArray
    {
        GatherCmp_3( Tex2DAS1,Tex2DAS1_sampler, Location.xyz, CompareVal);
        GatherCmp_4( Tex2DAS1,Tex2DAS1_sampler, Location.xyz, CompareVal, Offset.xy);
    }

    // TextureCube
    {
        GatherCmp_3( TexCS1,TexCS1_sampler, Location.xyz, CompareVal);
    }
#ifndef GL_ES 
    // TextureCubeArray
    {
        GatherCmp_3( TexCAS1,TexCAS1_sampler, Location.xyzw, CompareVal);
    }
#endif
}

struct InnerStruct
{
    float f[4];
    int i;
    uint u;
    bool b;
};

struct OuterStruct
{
    InnerStruct inner;
    float f;
    int i;
    uint u;
    bool b;
};

struct VSInputSubStruct
{
    float3 f3Normal;
    int VertexId;
};

struct VSInput
{
    float3 f3PosWS;
    float2 f2UV;
    VSInputSubStruct SubStruct;
};

struct VSOutputSubStruct
{
    float4 f4Attrib;
    int iAttrib;
};
struct VSOutput
{
    float2 f2Attrib;
    float3 f3Attrib;
    float4 f4Attrib;
    uint uiAttrib;
    uint3 ui3Attrib;
    float4 f4PosPS;
    VSOutputSubStruct SubStruct;
};

float4 TestVS  ( VSInput In,
               in float3 f3UV,
               in uint InstID,
               out VSOutput Out, 
               out float  fAttrib,
               out int4 i4Attrib)
{
    Out.f4PosPS = float4(1.0, 2.0, 3.0, 4.0);
    fAttrib = 1.0;
    Out.f2Attrib = float2(5.0, 6.0) + In.f2UV;
    Out.f3Attrib = float3(7.0, 8.0, 9.0) + In.f3PosWS + f3UV + In.SubStruct.f3Normal;
    Out.f4Attrib = float4(10.0, 11.0, 12.0, 13.0);
    if( In.f2UV.x < 0.5 )
        return float4(0.2,0.3,0.4,0.5);

    Out.f4Attrib.z = 0.1;
    Out.SubStruct.f4Attrib = float4(10.0, 11.0, 12.0, 13.0);
    //Out.uiAttrib = 1u;
    //Out.ui3Attrib = uint3(2u, 3u, 4u);
    //Out.iAttrib   = 5;
    //Out.i4Attrib = int4(5, 6, 7, 8);
    return float4(0.5,0.6,0.7,0.8);
}



void TestFuncArgs1( sampler2D Arg1,
                    SamplerState Arg1_sampler,
                    sampler2DShadow Arg2, 
                    SamplerComparisonState Arg2_sampler,
                    sampler3D Arg3)
{
    uint uWidth, uHeight, uMipLevels;
    GetTex2DDimensions_2( Arg1,uWidth, uHeight);

    SampleCmpTex2D_3( Arg2, Arg2_sampler, float2(0.5,0.5), 0.1 );
    
    LoadTex3D_1( Arg3, int4(1, 2, 3, 0) )_SWIZZLE1;
}

void TestFuncArgs2( isampler3D Arg1,
                    sampler2DArray Arg2, 
                    SamplerState Arg2_sampler,
                    sampler2D Arg3)
{
    LoadTex3D_1( Arg1, int4(1, 2, 3, 0) )_SWIZZLE1;

    uint uWidth, uHeight, uElems;
    GetTex2DArrDimensions_3( Arg2,uWidth, uHeight, uElems);

    LoadTex2D_1( Arg3,int3(10,15,3) )_SWIZZLE1;
}

struct PSOutputSubStruct
{
    float4 Color4;
};
struct PSOutput
{
    float4 Color3;
    PSOutputSubStruct substr;
};

layout(location = 0) in float2 _vsin_In_f2Attrib;
layout(location = 1) in float3 _vsin_In_f3Attrib;
layout(location = 2) in float4 _vsin_In_f4Attrib;
layout(location = 3) in uint _vsin_In_uiAttrib;
layout(location = 4) in uint3 _vsin_In_ui3Attrib;
layout(location = 5) in float4 _vsin_In_f4PosPS;
layout(location = 6) in float4 _vsin_In_SubStruct_f4Attrib;
layout(location = 7) in int _vsin_In_SubStruct_iAttrib;
layout(location = 0) out float _vsout_TestPS;
layout(location = 1) out float4 _vsout_Color;
layout(location = 2) out float3 _vsout_Color2;
layout(location = 3) out float4 _vsout_Out_Color3;
layout(location = 4) out float4 _vsout_Out_substr_Color4;

#define _RETURN_(_RET_VAL_){\
_vsout_TestPS = _RET_VAL_;\
_vsout_Color = Color;\
_vsout_Color2 = Color2;\
_vsout_Out_Color3 = Out.Color3;\
_vsout_Out_substr_Color4 = Out.substr.Color4;\
return;}

void main  ()
{
    VSOutput In;
    In.f2Attrib = _vsin_In_f2Attrib;
    In.f3Attrib = _vsin_In_f3Attrib;
    In.f4Attrib = _vsin_In_f4Attrib;
    In.uiAttrib = _vsin_In_uiAttrib;
    In.ui3Attrib = _vsin_In_ui3Attrib;
    In.f4PosPS = _vsin_In_f4PosPS;
    In.SubStruct.f4Attrib = _vsin_In_SubStruct_f4Attrib;
    In.SubStruct.iAttrib = _vsin_In_SubStruct_iAttrib;
    float4 Color;
    float3 Color2;
    PSOutput Out;

    float4 Pos = In.f4PosPS;

    Out.Color3 = float4(0.0, 1.0, 2.0, 3.0);
    Out.substr.Color4 = float4(0.0, 1.0, 2.0, 3.0);

    Tex2D_F1.Sample(Tex2D_F1_sampler, f3UVW.xy).xyzw.xyzw;


    TestFuncArgs1( Tex2D_F6,
                   Tex2D_F6_sampler,
                   Tex2DS_F5, 
                   Tex2DS_F5_sampler,
                   Tex3D_F3);
    TestFuncArgs2( Tex3D_I,
                   Tex2D_F_A3, 
                   Tex2D_F_A3_sampler,
                   Tex2D_F2);
    {
        float2 a = float2(0.0, 0.0);
        float2 b = float2(1.0, 1.0);
        float2 c = float2(2.0, 2.0);
        a += b;
        a-=b;
        a *=c;
        a/= c;
        float2 d = a;

        int2 x = int2(1, 2);
        int2 y = int2(2, 1);
        x %= y;
        x &= y;
        x |= y;
        x ^= y;
        x >>= y;
        x <<= y;
        x = x | y;
        x = x & y;
        x = x % y;
        x = x ^ y;
        x = ~y;
        x = x >> y;
        x = x << y;
        x++;
        ++x;
        y--;
        --y;
        if( x.x>= y.x || y.y >=x.y && x.y == y.x && y.y==x.y && !(x.x==y.y) )
            x += y;

        if(x.x==y.x)
            x.x=y.x;

        if(x.x==y.x)
            x.x+=y.x;
        {
            for(int i=0;i<10;++i)
                x.x+=1;
        }

        {
            for(int i=0;i<10;++i)
                y.x+=1;
        }

        Color =  In.f4Attrib;
        Color2 = In.f3Attrib;
        if( Pos.x < 0.2 ) _RETURN_( 0.1)
    }

    {
        int  i1 = 1;
        int2 i2 = int2(1, 2);
        int3 i3 = int3(1, 2, 3);
        int4 i4 = int4(1,2,3,4);

        float  f1 = 1.0;
        float2 f2 = float2(1.0, 2.0);
        float3 f3 = float3(1.0, 2.0, 3.0);
        float4 f4 = float4(1.0,2.0,3.0,4.0);
        float  f1_ = 2.0;
        float2 f2_ = float2(11.0, 12.0);
        float3 f3_ = float3(11.0, 12.0, 13.0);
        float4 f4_ = float4(11.0,12.0,13.0,14.0);

        uint  u1  = 1u;
        uint2 u2 = uint2(1u, 2u);
        uint3 u3 = uint3(1u, 2u, 3u);
        uint4 u4 = uint4(1u, 2u,3u,4u);

        bool  b1 = true;
        bool2 b2 = bool2(true, false);
        bool3 b3 = bool3(true, false, true);
        bool4 b4 = bool4(true, false, true, false);

        i1 = abs ( i1 ); i2 = abs ( i2 ); i3 = abs ( i3 ); i4 = abs ( i4 );
        f1 = abs ( f1 ); f2 = abs ( f2 ); f3 = abs ( f3 ); f4 = abs ( f4 );

                         b1 = all ( b2 ); b1 = all ( b3 ); b1 = all ( b4 );
                         b1 = any ( b2 ); b1 = any ( b3 ); b1 = any ( b4 );

        f1 = ceil( f1 ); f2 = ceil( f2 ); f3 = ceil( f3 ); f4 = ceil( f4 );

        f1 = clamp(f1,f1,f1); f2 = clamp(f2,f2,f2); f3 = clamp(f3,f3,f3); f4 = clamp(f4,f4,f4);
        i1 = clamp(i1,i1,i1); i2 = clamp(i2,i2,i2); i3 = clamp(i3,i3,i3); i4 = clamp(i4,i4,i4);
        u1 = clamp(u1,u1,u1); u2 = clamp(u2,u2,u2); u3 = clamp(u3,u3,u3); u4 = clamp(u4,u4,u4);

        // Trigonometric functions
        f1 = cos( f1 ); f2 = cos( f2 ); f3 = cos( f3 ); f4 = cos( f4 );
        f1 = sin( f1 ); f2 = sin( f2 ); f3 = sin( f3 ); f4 = sin( f4 );
        f1 = tan( f1 ); f2 = tan( f2 ); f3 = tan( f3 ); f4 = tan( f4 );
        f1 = cosh( f1 ); f2 = cosh( f2 ); f3 = cosh( f3 ); f4 = cosh( f4 );
        f1 = sinh( f1 ); f2 = sinh( f2 ); f3 = sinh( f3 ); f4 = sinh( f4 );
        f1 = tanh( f1 ); f2 = tanh( f2 ); f3 = tanh( f3 ); f4 = tanh( f4 );
        f1 = acos( f1 ); f2 = acos( f2 ); f3 = acos( f3 ); f4 = acos( f4 );
        f1 = asin( f1 ); f2 = asin( f2 ); f3 = asin( f3 ); f4 = asin( f4 );
        f1 = atan( f1 ); f2 = atan( f2 ); f3 = atan( f3 ); f4 = atan( f4 );
        f1 = atan2(f1,f1); f2 = atan2(f2,f2); f3 = atan2(f3,f3); f4 = atan2(f4,f4);
        f1 = degrees( f1 ); f2 = degrees( f2 ); f3 = degrees( f3 ); f4 = degrees( f4 );
        f1 = radians( f1 ); f2 = radians( f2 ); f3 = radians( f3 ); f4 = radians( f4 );

        // Exponential functions
        f1 = pow( f1, f1 ); f2 = pow( f2,f2 ); f3 = pow( f3,f3 ); f4 = pow( f4,f4 );
        f1 = exp( f1 ); f2 = exp( f2 ); f3 = exp( f3 ); f4 = exp( f4 );
        f1 = log( f1 ); f2 = log( f2 ); f3 = log( f3 ); f4 = log( f4 );
        f1 = exp2( f1 ); f2 = exp2( f2 ); f3 = exp2( f3 ); f4 = exp2( f4 );
        f1 = log2( f1 ); f2 = log2( f2 ); f3 = log2( f3 ); f4 = log2( f4 );
        f1 = sqrt( f1 ); f2 = sqrt( f2 ); f3 = sqrt( f3 ); f4 = sqrt( f4 );
        f1 = rsqrt( f1 ); f2 = rsqrt( f2 ); f3 = rsqrt( f3 ); f4 = rsqrt( f4 );
        f1 = log10( f1 ); f2 = log10( f2 ); f3 = log10( f3 ); f4 = log10( f4 );

        i1 = sign ( i1 ); i2 = sign ( i2 ); i3 = sign ( i3 ); i4 = sign ( i4 );
        f1 = sign ( f1 ); f2 = sign ( f2 ); f3 = sign ( f3 ); f4 = sign ( f4 );
        f1 = floor ( f1 ); f2 = floor( f2 ); f3 = floor( f3 ); f4 = floor( f4 );
        f1 = trunc ( f1 ); f2 = trunc( f2 ); f3 = trunc( f3 ); f4 = trunc( f4 );
        f1 = round ( f1 ); f2 = round( f2 ); f3 = round( f3 ); f4 = round( f4 );
        f1 = frac  ( f1 ); f2 = frac ( f2 ); f3 = frac ( f3 ); f4 = frac ( f4 );
        
        f1 = 1.0;
        f2 = float2(1.0, 2.0);
        f3 = float3(1.0, 2.0, 3.0);
        f4 = float4(1.0,2.0,3.0,4.0);
        f1 = fmod  ( f1, f1 ); f2 = fmod ( f2, f2 ); f3 = fmod ( f3, f3 ); f4 = fmod ( f4, f4 );
        f1 = modf  ( f1, f1 ); f2 = modf ( f2, f2 ); f3 = modf ( f3, f3 ); f4 = modf ( f4, f4 );

        f1 = min(f1,f1); f2 = min(f2,f2); f3 = min(f3,f3); f4 = min(f4,f4);
        i1 = min(i1,i1); i2 = min(i2,i2); i3 = min(i3,i3); i4 = min(i4,i4);
        u1 = min(u1,u1); u2 = min(u2,u2); u3 = min(u3,u3); u4 = min(u4,u4);
        
        f1 = max(f1,f1); f2 = max(f2,f2); f3 = max(f3,f3); f4 = max(f4,f4);
        i1 = max(i1,i1); i2 = max(i2,i2); i3 = max(i3,i3); i4 = max(i4,i4);
        u1 = max(u1,u1); u2 = max(u2,u2); u3 = max(u3,u3); u4 = max(u4,u4);

        f1 = lerp(f1,f1,f1); f2 = lerp(f2,f2,f2); f3 = lerp(f3,f3,f3); f4 = lerp(f4,f4,f4);
        f1 = step(f1,f1); f2 = step(f2,f2); f3 = step(f3,f3); f4 = step(f4,f4);
        f1 = smoothstep(f1,f1_,f1); f2 = smoothstep(f2,f2_,f2); f3 = smoothstep(f3,f3_,f3); f4 = smoothstep(f4,f4_,f4);

        b1 = isnan ( f1/Pos.x ); b2 = isnan( f2/Pos.x ); b3 = isnan( f3/Pos.x ); b4 = isnan( f4/Pos.x );
        b1 = isinf ( f1/Pos.x ); b2 = isinf( f2/Pos.x ); b3 = isinf( f3/Pos.x ); b4 = isinf( f4/Pos.x );
        b1 = isfinite ( f1/Pos.x ); b2 = isfinite( f2/Pos.x ); b3 = isfinite( f3/Pos.x ); b4 = isfinite( f4/Pos.x );
        f1 = mad(f1,f1,f1); f2 = mad(f2,f2,f2); f3 = mad(f3,f3,f3); f4 = mad(f4,f4,f4);

        f1 = distance(f1,f1); f1 = distance(f2,f2); f1 = distance(f3,f3); f1 = distance(f4,f4);
        f1 = length(f1); f1 = length(f2); f1 = length(f3); f1 = length(f4);
        f1 = dot(f1,f1); f1 = dot(f2,f2); f1 = dot(f3,f3); f1 = dot(f4,f4);
        f3 = cross( f3, f3 );

        f1 = 1.0;
        f2 = float2(1.0, 2.0);
        f3 = float3(1.0, 2.0, 3.0);
        f4 = float4(1.0,2.0,3.0,4.0);
        f1 = normalize(f1); f2 = normalize(f2); f3 = normalize(f3); f4 = normalize(f4);
        f1 = reflect(f1,f1); f2 = reflect(f2,f2); f3 = reflect(f3,f3); f4 = reflect(f4,f4);
        f1 = refract(f1,f1,1.0); f2 = refract(f2,f2,1.0); f3 = refract(f3,f3,1.0); f4 = refract(f4,f4,1.0);
        f1 = faceforward(f1,f1,f1); f2 = faceforward(f2,f2,f2); f3 = faceforward(f3,f3,f3); f4 = faceforward(f4,f4,f4);

        //f1 = dst(f1,f1); f1 = dst(f2,f2); f1 = dst(f3,f3); f1 = dst(f4,f4);
        f1 = rcp(f1); f2 = rcp(f2); f3 = rcp(f3); f4 = rcp(f4);

        i1 = countbits(u1); i2 = countbits(u2); i3 = countbits(u3); i4 = countbits(u4);
        i1 = countbits(i1); i2 = countbits(i2); i3 = countbits(i3); i4 = countbits(i4);
        i1 = firstbithigh(u1); i2 = firstbithigh(u2); i3 = firstbithigh(u3); i4 = firstbithigh(u4);
        i1 = firstbithigh(i1); i2 = firstbithigh(i2); i3 = firstbithigh(i3); i4 = firstbithigh(i4);
        i1 = firstbitlow(u1); i2 = firstbitlow(u2); i3 = firstbitlow(u3); i4 = firstbitlow(u4);
        i1 = firstbitlow(i1); i2 = firstbitlow(i2); i3 = firstbitlow(i3); i4 = firstbitlow(i4);
        u1 = reversebits(u1); u2 = reversebits(u2); u3 = reversebits(u3); u4 = reversebits(u4);
        i1 = reversebits(i1); i2 = reversebits(i2); i3 = reversebits(i3); i4 = reversebits(i4);

        f1 = saturate(f1); f2 = saturate(f2); f3 = saturate(f3); f4 = saturate(f4);
        sincos(f1,f1,f1_); sincos(f2,f2,f2_); sincos(f3,f3,f3_); sincos(f4,f4,f4_);

        f1 = frexp(f1_, i1); f2 = frexp(f2_, i2); f3 = frexp(f3_, i3); f4 = frexp(f4_, i4);
        f1 = ldexp(f1, i1); f2 = ldexp(f2, i2); f3 = ldexp(f3, i3); f4 = ldexp(f4, i4);

        float4x4 f4x4;
        f4x4[0] = float4(0.0, 1.0/Pos.x, 2.0/Pos.y, 3.0);
        f4x4[1] = float4(0.0, 1.0/Pos.x, 2.0/Pos.y, 3.0);
        f4x4[2] = float4(0.0, 1.0/Pos.x, 2.0/Pos.y, 3.0);
        f4x4[3] = float4(0.0, 1.0/Pos.x, 2.0/Pos.y, 3.0);
        f4x4 = transpose(f4x4);
        f1 = determinant( f4x4 );

        float3x3 f3x3;
        f3x3[0] = float3(0.0, 1.0/Pos.x, 2.0/Pos.y);
        f3x3[1] = float3(0.0, 1.0/Pos.x, 2.0/Pos.y);
        f3x3[2] = float3(0.0, 1.0/Pos.x, 2.0/Pos.y);
        f3x3 = transpose(f3x3);
        f1 = determinant( f3x3 );

        float2x2 f2x2;
        f2x2[0] = float2(0.0, 1.0/Pos.x);
        f2x2[1] = float2(0.0, 1.0/Pos.x);
        f2x2 = transpose(f2x2);
        f1 = determinant( f2x2 );

        f1 = ddx( Pos.x ); f2 = ddx( Pos.xy ); f3 = ddx( Pos.xyz ); f4 = ddx( Pos.xyzw );
        f1 = ddy( Pos.x ); f2 = ddy( Pos.xy ); f3 = ddy( Pos.xyz ); f4 = ddy( Pos.xyzw );
        f1 = ddx_coarse( Pos.x ); f2 = ddx_coarse( Pos.xy ); f3 = ddx_coarse( Pos.xyz ); f4 = ddx_coarse( Pos.xyzw );
        f1 = ddy_coarse( Pos.x ); f2 = ddy_coarse( Pos.xy ); f3 = ddy_coarse( Pos.xyz ); f4 = ddy_coarse( Pos.xyzw );
        f1 = ddx_fine( Pos.x ); f2 = ddx_fine( Pos.xy ); f3 = ddx_fine( Pos.xyz ); f4 = ddx_fine( Pos.xyzw );
        f1 = ddy_fine( Pos.x ); f2 = ddy_fine( Pos.xy ); f3 = ddy_fine( Pos.xyz ); f4 = ddy_fine( Pos.xyzw );
#ifdef FRAGMENT_SHADER
        f1 = fwidth( Pos.x ); f2 = fwidth( Pos.xy ); f3 = fwidth( Pos.xyz ); f4 = fwidth( Pos.xyzw );
#endif

        f1 = asfloat( f1 ); f2 = asfloat( f2 ); f3 = asfloat( f3 ); f4 = asfloat( f4 );
        f1 = asfloat( i1 ); f2 = asfloat( i2 ); f3 = asfloat( i3 ); f4 = asfloat( i4 );
        f1 = asfloat( u1 ); f2 = asfloat( u2 ); f3 = asfloat( u3 ); f4 = asfloat( u4 );

        i1 = asint( f1 ); i2 = asint( f2 ); i3 = asint( f3 ); i4 = asint( f4 );
        i1 = asint( i1 ); i2 = asint( i2 ); i3 = asint( i3 ); i4 = asint( i4 );
        i1 = asint( u1 ); i2 = asint( u2 ); i3 = asint( u3 ); i4 = asint( u4 );

        u1 = asuint( f1 ); u2 = asuint( f2 ); u3 = asuint( f3 ); u4 = asuint( f4 );
        u1 = asuint( i1 ); u2 = asuint( i2 ); u3 = asuint( i3 ); u4 = asuint( i4 );
        u1 = asuint( u1 ); u2 = asuint( u2 ); u3 = asuint( u3 ); u4 = asuint( u4 );

        f1 = f16tof32( u1 ); f2 = f16tof32( u2 ); f3 = f16tof32( u3 ); f4 = f16tof32( u4 );
        f1 = f16tof32( i1 ); f2 = f16tof32( i2 ); f3 = f16tof32( i3 ); f4 = f16tof32( i4 );
        u1 = f32tof16( f1 ); u2 = f32tof16( f2 ); u3 = f32tof16( f3 ); u4 = f32tof16( f4 );
#ifndef GL_ES
        double d = asdouble( u1, u1 );
#endif

        f1 = noise( f1 ); f2 = noise( f2 ); f3 = noise( f3 ); f4 = noise( f4 );
    }
    _RETURN_( 1.0)
}