#ifndef COLOR_H
#define COLOR_H
struct Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};
struct Color get_random_color();
uint32_t color_to_int(struct Color color);
#endif
