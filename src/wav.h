#pragma once

#include <cstdint>
#include <vector>

struct WavRIFFChunk
{
	uint8_t  ChunkID[4];
	uint32_t Size;
	uint8_t  RIFFType[4];
};

struct WavFormatChunk
{
	uint8_t  ChunkID[4];
	uint32_t Size;
	uint16_t CompressionType;
	uint16_t Channels;
	uint32_t SamplesRate;
	uint32_t BytesPerSec;
	uint16_t BlockAlignment;
	uint16_t SampleDepth;
	uint16_t ExtraFormatSize;
};

struct WavDataChunk
{
	uint8_t  ChunkID[4];
	uint32_t Size;
};

class wav
{
private:
	WavRIFFChunk riff_chunk;
	WavFormatChunk format_chunk;
	WavDataChunk  data_chunk;
	std::vector<uint8_t> buffer;
	bool is_valid;
	uint32_t waveform_offset;

public:
	wav();
	wav(const char *path);
	~wav();

	uint32_t TimeToSample(double time);
	uint32_t NumSamples();
	uint16_t Channels();
	uint32_t SampleRate();
	uint16_t SampleDepth();
	double   LengthInSec();

	void GetSamples(float *buffer, uint32_t buffer_size, uint32_t start_index, uint32_t end_index);
};