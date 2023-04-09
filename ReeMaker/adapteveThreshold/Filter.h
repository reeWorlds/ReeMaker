extern "C"
{
	// Parameters:
	// 0: kernel size (int)
	// 1: sigma (float)
	_declspec(dllexport) bool apply(unsigned char* imageData, int imageWidth, int imageHeight, int channelsCount, void** parameters);
}