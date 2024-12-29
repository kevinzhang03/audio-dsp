# Diffusion Step in Audio Processing

my original prompt:
*so let's say the incoming samples would be the first iteration of the processing. we are writing to the delay line and reading from it right away. because the delay line is initially empty and this is the first iteration, there is nothing in the delay line. delayed is [0,0,0,0]. but since the delay amounts for each channel are randomized, then that first sample written to the delay line will not be read in order so further down the line we will get some more diffuse samples. and while all this is happening, the delayed array is getting maximally mixed from the hadamard matrix to further diffuse the data. then, each channel's sample's polarity is flipped randomly?*

## Overview

The `DiffusionStep` struct is designed to diffuse audio signals across multiple channels using delay lines, Hadamard mixing, and polarity flipping. This document explains how the diffusion process works, using a 4-channel audio input as an example.

## How It Works

### 1. **Initialization**
- **Channels**: The number of channels is defined by the template parameter, defaulting to 8.
- **Delay Lines**: Each channel has its own delay line, initialized to be empty.
- **Delay Samples**: The delay amount for each channel is randomized within a specified range.
- **Polarity Flipping**: Each channel has a boolean flag to determine if its polarity should be flipped.

### 2. **Processing Flow**

#### First Iteration

1. **Input Samples**:
   - Assume the input array is `[in0, in1, in2, in3]`.

2. **Writing to Delay Lines**:
   - Each input sample is written to its respective delay line.
   - Since the delay lines are initially empty, reading from them will return zero.
   - Therefore, the `delayed` array will be `[0, 0, 0, 0]` on the first iteration.

3. **Hadamard Mixing**:
   - The `delayed` array `[0, 0, 0, 0]` is mixed using a Hadamard matrix.
   - The Hadamard transform spreads the energy across all channels.
   - On the first iteration, since the `delayed` array is all zeros, the mixed array will also be `[0, 0, 0, 0]`.

4. **Polarity Flipping**:
   - Each channel's sample in the `mixed` array is potentially flipped in polarity based on the `flipPolarity` array.
   - On the first iteration, this will still result in `[0, 0, 0, 0]` since the input was all zeros.

5. **Output**:
   - The `process` function returns the `mixed` array, which is `[0, 0, 0, 0]` on the first iteration.

#### Subsequent Iterations

- On subsequent iterations, the delay lines will contain the previously written samples.
- The `delayed` array will contain non-zero values, and the Hadamard transform will mix these values.
- The polarity flipping will further modify the mixed samples, resulting in a more diffuse output.

## Example: 4 Channels

### Input:
- **Channels**: 4.
- **Input Samples**: `[in0, in1, in2, in3]`.

### First Iteration:
1. **Writing to Delay Lines**:
   - `delays[0].write(in0)`, `delays[1].write(in1)`, `delays[2].write(in2)`, `delays[3].write(in3)`.
   - `delayed` array: `[0, 0, 0, 0]`.

2. **Hadamard Mixing**:
   - `mixed` array: `[0, 0, 0, 0]`.

3. **Polarity Flipping**:
   - `mixed` array after flipping: `[0, 0, 0, 0]`.

4. **Output**:
   - `[0, 0, 0, 0]`.

### Subsequent Iterations:
- Delay lines contain previous samples.
- `delayed` array contains non-zero values.
- Hadamard mixing and polarity flipping result in a more diffuse output.

## Why This Matters

This approach:
- **Provides Immediate Feedback**: Writing and reading in the same iteration allows the current input to affect the current output.
- **Maximizes Diffusion**: The Hadamard transform and polarity flipping ensure that the audio signal is spread across all channels.
- **Supports Complex Audio Processing**: This method can be used in various audio effects to create rich, diffuse soundscapes.

This logic is essential for creating versatile, reusable audio processors in modern DSP workflows.