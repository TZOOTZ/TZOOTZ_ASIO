#pragma once

#include <atomic>

#include <RtAudio.h>

#include "RealtekDetector.h"

namespace tzootz {

class TzootzAsioEngine {
public:
    explicit TzootzAsioEngine(RtAudio &audio);
    ~TzootzAsioEngine();

    void initialize(const DeviceSelection &selection);
    void start();
    void stop();

private:
    static int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                             double streamTime, RtAudioStreamStatus status, void *userData);

    RtAudio &audio_;
    DeviceSelection device_{};
    RtAudio::StreamParameters outputParams_{};
    RtAudio::StreamOptions options_{};
    unsigned int sampleRate_ = 48000;
    unsigned int bufferFrames_ = 128;
    std::atomic<bool> running_{false};
};

}  // namespace tzootz
