#include "reverb-example-code.h"

#include "./wav.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <array>
#include <filesystem>

class ExampleRunner {
    Wav inputWav;
    std::string outputLocation;
    std::string inputFilenamePrefix;

    template<class Processor>
    void monoProcessor(std::string name, Processor &&processor) {
        Wav outputWav;
        outputWav.sampleRate = inputWav.sampleRate;
        outputWav.channels = 1;
        
        srand(1);
        processor.configure(outputWav.sampleRate);
        
        int durationSamples = inputWav.samples.size()/inputWav.channels;
        outputWav.samples.resize(durationSamples);
        for (int i = 0; i < durationSamples; ++i) {
            outputWav.samples[i] = processor.process(inputWav.samples[i*inputWav.channels]);
        }
        std::string outputFilename = outputLocation + "/output-" + inputFilenamePrefix + "-" + name + ".wav";
        outputWav.write(outputFilename);
        if (!outputWav.result) {
            std::cerr << outputWav.result.reason << "\n";
            std::exit(1);
        }
    }

	/*
	 creates a template for multiProcessor
	 template takes in arguments for number of channels, whether it should avg output, and what kind of processor
	 multiProcessor takes in fixed type of string, but also an arbitary processor

	 usage:
													 vvv-- this can be intferred by the compiler
	 multiProcessor<channels, whether to avg output, and processor type/class/struct> ( other args... )
	*/
    template<int channels, bool averageOutputs=true, class Processor>
    void multiProcessor(std::string name, Processor &&processor) {
        static_assert(channels%2 == 0, "there must be an even number of channels");

        Wav outputWav;
        outputWav.sampleRate = inputWav.sampleRate;
        outputWav.channels = 2;
        
        srand(1);
        processor.configure(outputWav.sampleRate);
        
        int durationSamples = inputWav.samples.size()/inputWav.channels;
        outputWav.samples.resize(durationSamples*2);
		for (int i = 0; i < durationSamples; ++i) {

			// Duplicate input channels as many times as needed
			// This array is of type double and length channels
			std::array<double, channels> array;
			for (int c = 0; c < channels; ++c) {
				int inputChannel = c%inputWav.channels;
				int index = i*inputWav.channels + inputChannel; // if 4 logical and 2 physical input channels, then inputchannel will be 0 1 0 1
				array[c] = inputWav.samples[index];
			}
			
			array = processor.process(array);
			
			if (averageOutputs) {
				// Mix down into stereo for example output
				double left = 0, right = 0;
				for (int c = 0; c < channels; c += 2) {
					left += array[c];
					right += array[c + 1];
				}
				
				constexpr double scaling = 2.0/channels;
				outputWav.samples[2*i] = left*scaling;
				outputWav.samples[2*i + 1] = right*scaling;
			} else {
				outputWav.samples[2*i] = array[0];
				outputWav.samples[2*i + 1] = array[1];
			}
		}
        std::string outputFilename = outputLocation + "/output-" + inputFilenamePrefix + "-" + name + ".wav";
        outputWav.write(outputFilename);
        if (!outputWav.result) {
            std::cerr << outputWav.result.reason << "\n";
            std::exit(1);
        }
    }

    void extendDuration(double seconds) {
        int extraSamples = seconds*inputWav.sampleRate;
        inputWav.samples.resize(inputWav.samples.size() + extraSamples, 0);
    }
public:
    ExampleRunner(std::string inputFile, std::string outputLocationArg) : inputWav(inputFile) {
        if (!inputWav.result) {
            std::cerr << inputWav.result.reason << "\n";
            std::exit(1);
        }

        // Extract filename without extension for prefix
        size_t lastSlash = inputFile.find_last_of("/\\");
        std::string filename = (lastSlash == std::string::npos) ? inputFile : inputFile.substr(lastSlash + 1);
        size_t dotPos = filename.find_last_of('.');
        inputFilenamePrefix = (dotPos == std::string::npos) ? filename : filename.substr(0, dotPos);

        // Set output location and ensure directory exists
        outputLocation = outputLocationArg.empty() ? "./output-wav" : outputLocationArg;
        if (!std::__fs::filesystem::exists(outputLocation)) {
            std::__fs::filesystem::create_directories(outputLocation);
        }
    }

    void runExamples() {
        extendDuration(20);
        monoProcessor("single-channel-feedback", SingleChannelFeedback());
        multiProcessor<2>("multi-channel-feedback-2", MultiChannelFeedback<2>());
        multiProcessor<4>("multi-channel-feedback-4", MultiChannelFeedback<4>());
        multiProcessor<8>("multi-channel-feedback-8", MultiChannelFeedback<8>());
        multiProcessor<16>("multi-channel-feedback-16", MultiChannelFeedback<16>());
        multiProcessor<4>("multi-channel-feedback-householder-4", MultiChannelMixedFeedback<4>());
        multiProcessor<8>("multi-channel-feedback-householder-8", MultiChannelMixedFeedback<8>());
        multiProcessor<16>("multi-channel-feedback-householder-16", MultiChannelMixedFeedback<16>());
        multiProcessor<4, false>("diffuser-equal-4-3", DiffuserHalfLengths<4, 3>(200));
        multiProcessor<8, false>("diffuser-equal-8-4", DiffuserHalfLengths<8, 4>(200));
        multiProcessor<8, false>("diffuser-equal-8-6-long", DiffuserEqualLengths<8, 6>(3000));
        multiProcessor<8, false>("diffuser-halves-8-6-long", DiffuserHalfLengths<8, 6>(3000));
		// extendDuration(1);
        multiProcessor<8, true>("reverb-basic-8-short", BasicReverb<8, 4>(50, 2.5));
		// extendDuration(2);
        multiProcessor<8, true>("reverb-basic-8-mix", BasicReverb<8, 4>(100, 3, 1, 0.25));
		// extendDuration(2);
        multiProcessor<8, true>("reverb-basic-8-long", BasicReverb<8, 4>(100, 10));
    }
};

int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cerr << "Missing: input.wav [output-location]\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputLocation = (argc > 2) ? argv[2] : "";
    ExampleRunner runner(inputFile, outputLocation);
    runner.runExamples();
}
