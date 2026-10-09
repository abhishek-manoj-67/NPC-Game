#pragma once

inline constexpr int WINWIDTH = 800;
inline constexpr int WINHEIGHT = 600;
inline constexpr int FPS = 60;
inline constexpr uint32_t RED = 0xff0000ff;
inline constexpr uint32_t GREEN = 0x00ff00ff;
inline constexpr uint32_t BLUE = 0x0000ffff;

inline constexpr float PI = 3.141592653589793;

inline int8_t sign(long double x) {
	if (x == 0) {
		return 0;
	}

	return abs(x) / x;
} 

inline uint8_t r(uint32_t color) {

	return (color >> 24) & 0xff;

}

inline uint8_t g(uint32_t color) {

	return (color >> 16) & 0xff;

}

inline uint8_t b(uint32_t color) {

	return (color >> 8) & 0xff;

}

inline uint8_t a(uint32_t color) {

	return (color >> 0) & 0xff;

}
