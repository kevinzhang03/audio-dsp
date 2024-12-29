# basic reverb using a feedback delay network

This code is largely (like 99%) written by Geraint Luff from Signalsmithm and is from [_Let's Write A Reverb_](https://www.youtube.com/watch?v=6ZK2Goiyotk), presented at ADC 2021.

https://github.com/Signalsmith-Audio/reverb-example-code
https://github.com/signalsmith-audio/dsp

I'm mainly writing this readme (with ChatGPT) so I can look back on it if I ever forget how it works. I made some modifications to more neatly organize and name the output audio files in the project directory.

---

## how it works

At a high level, the code processes an input WAV file and generates several output WAV files using different audio processing techniques.

1. **Single-Channel Feedback**: Basic feedback delay applied to mono input.
2. **Multi-Channel Feedback**: Processes audio through multiple feedback delay lines, optionally mixing channels using Householder or Hadamard matrices.
3. **Diffusers**: Implements different delay length distributions to create more complex echo patterns.
4. **Reverb Effects**: Combines multiple processing techniques to simulate realistic reverberation in different spaces.

Each effect is applied independently, and the output files are saved in a structured format for comparison.

---

## Features
- Supports mono and multi-channel audio.
- Configurable delay feedback and mixing matrices.
- Produces multiple output WAV files for analysis.
- Automatically handles file naming and output directory management.

---

## How to Use

### **Building the Program**

1. Make sure you have a C++ compiler installed (e.g., `clang++`).
2. Use the provided `Makefile` to compile the code:

   ```bash
   make
   ```

3. If you need to clean up compiled files and outputs:

   ```bash
   make clean
   ```

---

### **Running the Program**

1. Run the program by specifying the input WAV file as the first argument:

   ```bash
   ./main.out <input-wav-file> [optional-output-dir]
   ```

   - `<input-wav-file>`: Path to the input WAV file.
   - `[optional-output-dir]`: (Optional) Path to the output directory. Defaults to `./output-wav`.

2. Example command:

   ```bash
   ./main.out example.wav
   ```

   This will process `example.wav` and save outputs in `./output-wav`.

3. Outputs are named with the format:

   ```
   output-<input-filename>-<effect-name>.wav
   ```

   Example:
   ```
   output-example-multi-channel-feedback-8.wav
   ```

---

### **Supported Effects**
The program generates several output files based on the effects applied:

1. **Single-Channel Feedback**:
   - `output-<input-filename>-single-channel-feedback.wav`

2. **Multi-Channel Feedback**:
   - Examples:
     - `output-<input-filename>-multi-channel-feedback-2.wav`
     - `output-<input-filename>-multi-channel-feedback-16.wav`

3. **Householder-Mixed Feedback**:
   - Examples:
     - `output-<input-filename>-multi-channel-feedback-householder-4.wav`

4. **Diffusers**:
   - Examples:
     - `output-<input-filename>-diffuser-equal-8-6-long.wav`

5. **Reverb Effects**:
   - Examples:
     - `output-<input-filename>-reverb-basic-8-long.wav`

---

## Directory Structure

- **Input File**: The WAV file you want to process.
- **Output Directory**: Default is `./output-wav`, but this can be customized via the second argument to the program.

---

## Requirements

- A C++11-compatible compiler. I use `clang++` here.
- An input WAV file to process. There's a couple in here already.
