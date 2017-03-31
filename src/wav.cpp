#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <cstring>

#include "wav.h"

wav::wav()
{
	is_valid = false;
}

wav::wav(const char * path)
{
	// Read the contents of the file into memory.
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer.resize(size);
	file.read((char *)buffer.data(), size);
	uint8_t *data = buffer.data();

	uint32_t offset = 0;

	// Read the RIFF chunk.
	riff_chunk = (WavRIFFChunk &)data[offset];
	offset += sizeof(WavRIFFChunk);
	
	// Read the format chunk.
	format_chunk = (WavFormatChunk &)data[offset];
	offset += format_chunk.Size + 8;

	// Read the data chunk.
	data_chunk = (WavDataChunk &)data[offset];
	offset += sizeof(WavDataChunk);

	// Set the data pointer which points to the start of the PCM data.
	waveform_offset = offset;

	// Perform the validation on the WAV file.
	// Check to see if the file is a RIFF container file.
	if(memcmp("RIFF", riff_chunk.ChunkID, 4) != 0)
	{
		std::cout << "Error: Expecting 'RIFF'.  Unknown container type." << std::endl;
		is_valid = false;
		return;
	}

	// Check to see if the file contains a WAVE file.
	if (memcmp("WAVE", riff_chunk.RIFFType, 4) != 0) 
	{
		std::cout << "Error: Expecting 'WAVE'. Unsupported RIFF type." << std::endl;
		is_valid = false;
		return;
	}

	// Check to see if the wave is in PCM format.
	if (format_chunk.CompressionType != 1)
	{
		std::cout << "Error: Expecting PCM data.  Unsupported compression type." << std::endl;
		is_valid = false;
		return;
	}

	// Check to see if 16 bit depth.
	if (format_chunk.SampleDepth != 16)
	{
		std::cout << "Error: Expecting 16 bit PCM data.  Unsupported bit depth." << std::endl;
		is_valid = false;
		return;
	}

	// Check to see if the data chunk is in the correct position.
	if (memcmp("data", data_chunk.ChunkID, 4) != 0)
	{
		std::cout << "Error: Data chunk not found in the correct location." << std::endl;
		is_valid = false;
		return;
	}

	is_valid = true;
}

wav::~wav()
{
}

uint32_t wav::TimeToSample(double time)
{

	return uint32_t(time * SampleRate());
}

uint32_t wav::NumSamples()
{
	if (is_valid)
	{
		return (data_chunk.Size * 8) / (format_chunk.SampleDepth * format_chunk.Channels);
	}
	else
	{
		return 0;
	}
}

uint16_t wav::Channels()
{
	return is_valid ? format_chunk.Channels : 0;
}

uint32_t wav::SampleRate()
{
	return is_valid ? format_chunk.SamplesRate : 0;
}

uint16_t wav::SampleDepth()
{
	return is_valid ? format_chunk.SampleDepth : 0;
}


double wav::LengthInSec()
{
	return NumSamples() / SampleRate();
}

void wav::GetSamples(float *buffer, uint32_t buffer_size, uint32_t start_index, uint32_t end_index)
{
	if (end_index - start_index == 0)
	{
		return;
	}

	if (buffer_size == 0)
	{
		return;
	}

	if (!is_valid)
	{
		return;
	}

	int16_t *data = (int16_t *)&this->buffer.data()[waveform_offset];

	uint16_t samplesPerPoint = (end_index - start_index) / buffer_size;
	samplesPerPoint = std::max(uint16_t(1), samplesPerPoint);

	uint32_t start = start_index - start_index % samplesPerPoint;

	for (uint32_t i = 0; i < buffer_size; i++)
	{
		buffer[i] = float(data[start + samplesPerPoint * i]) / INT16_MAX;
	}
}

