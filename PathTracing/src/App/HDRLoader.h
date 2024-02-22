#pragma once

struct HDRLoaderResult {
	int width, height;
	// each pixel takes 3 float32, each component can be of any value...
	float* cols;
	HDRLoaderResult() = default;
};

class HDRLoader {
public:
	static bool load(const char* fileName, HDRLoaderResult& res);
};