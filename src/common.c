#include <pebble.h>
#include "common.h"
#include "game_ddr.h"
#include "fapp_game.h"
#include "game_math.h"

const int kPebbleScreenWidth	= 144;
const int kPebbleScreenHeight	= 152;

static void coords_from_index(const int rowSizeBits, const int index,
		int *restrict x, int *restrict y) __attribute__((unused));
static void coords_from_index(const int rowSizeBits, const int index,
		int *restrict x, int *restrict y)
{
	*x = index % rowSizeBits;
	*y = index / rowSizeBits;
}
static int index_from_coords(int rowSizeBits, int x, int y) {
	return (rowSizeBits * y) + x;
}

BitmapLayer *bitmap_layer_rotate_right(BitmapLayer *bitmapLayer) {
	const GRect frame = layer_get_frame(bitmap_layer_get_layer(bitmapLayer));
	BitmapLayer *const newBitmapLayer = bitmap_layer_create(frame);
	if (!newBitmapLayer)
		return NULL;
	
	const GBitmap *const orig		= bitmap_layer_get_bitmap(bitmapLayer);
	const GSize origSize			= orig->bounds.size;
	const uint16_t rowSizeBytes		= orig->row_size_bytes;
	const uint8_t *const origData	= (uint8_t *)orig->addr;
	const uint16_t bufferSize		= origSize.h * rowSizeBytes;
	
	// malloc bitmap data
	uint8_t *const restrict newBuffer = (uint8_t *)calloc(1, bufferSize);
	if (!newBuffer) {
		bitmap_layer_destroy(newBitmapLayer);
		return NULL;
	}
	
	// rotate bits
	const int rowSizeBits = rowSizeBytes * 8;
	for (int x = 0; x < origSize.w; x++) {
		for (int y = 0; y < origSize.h; y++) {
			const uint8_t srcMask	= 0x01 << (7 - (x % 8));
			const uint8_t destMask	= ~(0x01 << (x % 8));
			// >> 3 is to divide by 8
			const int srcIndex	= index_from_coords(rowSizeBits, x, y) >> 3;
			const int destIndex	= index_from_coords(rowSizeBits, origSize.h - y, x) >> 3;
			newBuffer[destIndex] = (newBuffer[destIndex] & destMask) | (origData[srcIndex] & srcMask);
		}
	}
	
	// create new bitmap from our buffer
	const GBitmap *const newBitmap = gbitmap_create_with_data(newBuffer);
	bitmap_layer_set_bitmap(newBitmapLayer, newBitmap);
	return newBitmapLayer;
}

int rand_in_range(int min, int max) {
	return min + (int)((max - min) * (1.0 * rand() / RAND_MAX));
}

void game_init() {
	
}

void game_deinit() {
	
}
