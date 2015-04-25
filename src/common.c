#include <pebble.h>
#include "common.h"
#include "game_ddr.h"


const int kPebbleScreenWidth	= 144;
const int kPebbleScreenHeight	= 152;

static void coords_from_index(const int rowSizeBits, const int index, int *x, int *y) {
	
}
static int index_from_coords(int rowSizeBits, int x, int y) {
	return (rowSizeBits * y) + x;
}

BitmapLayer *bitmap_layer_rotate_right(BitmapLayer *bitmapLayer) {
	CGFrame frame = layer_get_frame(bitmap_layer_get_layer(bitmapLayer));
	BitmapLayer *newBitmapLayer = bitmap_layer_create(frame);
	if (!newBitmapLayer)
		return NULL;
	
	GBitmap orig = bitmap_layer_get_bitmap(bitmapLayer);
	GSize origSize = orig.bounds.size;
	uint16_t rowSizeBytes = orig.row_size_bytes;
	void *origData = orig.addr;
	uint16_t bufferSize = origSize.h * rowSizeBytes;
	// copy bitmap data
	void *newBuffer = malloc(bufferSize);
	if (!newBuffer)
		return NULL;
	memcpy(newBuffer, origData, bufferSize);
	
	GBitmap newBitmap = gbitmap_create_with_data(newBuffer);
}

void game_init() {
	
}

void game_deinit() {
	
}
