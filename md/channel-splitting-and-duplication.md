# Channel Splitting and Duplication in Audio Processing

REFERENCING `multiProcessor` in `main.cpp`

We should get familiar with channels, frames, and samples. A **channel** corresponds to a specific audio track of a file, like for left and right in stereo audio. Most input audio will have two channels. A **sample** is the atomic unit of an audio file, representing the amplitude value of the sound wave being represented at any moment. Separate channels do not share a sample for a moment in time. A **frame** groups together the distinct samples in each channel that all represent the same moment in time.

```py
# For stereo audio (2 channels):
# each a, b, ..., h are the individual samples and are of type float
wav = [a, b, c, d, e, f, g, h]
frames = [a, b], [c, d], [e, f], [g, h]
# The first element in each frame is L, the second is R
# e.g. a is left, b is right
# Samples are share a frame happen at the same time
```

There's also the sample rate, which tells us how many **frames per second** are in our audio file, represented has Hz. This is not important to us for now.

---

## How It Works

### 1. **Logical vs. Physical Channels**
- **Logical Channels (`channels`)**: The number of channels expected by the processing logic. Must be an even number so we can map these back to stereo for the output.
- **Physical Channels (`inputWav.channels`)**: The actual channels present in the input audio. Will typically be 2 because input wav file is stereo.

If the logical channels exceed physical channels, **duplication occurs** to map logical channels to valid physical ones. This ensures all logical channels have corresponding input data.

### 2. **Channel Mapping**
The code maps logical channels to physical channels using modular arithmetic:
```cpp
int inputChannel = c % inputWav.channels;
int index = i * inputWav.channels + inputChannel;
array[c] = inputWav.samples[index];
```
- **`c % inputWav.channels`**: Ensures logical channels wrap back to physical channels if necessary.
- **`i * inputWav.channels + inputChannel`**: Computes the index of the correct sample in the flattened array of audio data.

---

## Example: 4 Logical Channels and 2 Physical Channels (Stereo)

### Input:
- **Physical Channels**: 2 (stereo: left and right).
- **Logical Channels**: 4.
- **Input Samples**: `[a, b, c, d, e, f, g, h]` (interleaved stereo, 4 frames).

### Frames:
1. **Frame 1 (`i = 0`)**:
   - Logical to Physical Mapping: `0 → 0, 1 → 1, 2 → 0, 3 → 1`.
   - Array: `[a, b, a, b]`.

2. **Frame 2 (`i = 1`)**:
   - Array: `[c, d, c, d]`.

3. **Frame 3 (`i = 2`)**:
   - Array: `[e, f, e, f]`.

4. **Frame 4 (`i = 3`)**:
   - Array: `[g, h, g, h]`.

---

## Why This Matters
This approach:
- **Handles channel mismatches**: Logical channels can exceed physical channels without breaking the processing.
- **Supports flexible processing**: Audio effects can be applied to configurations like quadraphonic or multi-channel setups, even with stereo input.

This logic is the backbone for versatile, reusable audio processors in modern DSP workflows.