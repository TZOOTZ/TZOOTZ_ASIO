#include "TzootzAsioEngine.h"

#include <algorithm>
#include <cstring>
#include <iostream>

namespace tzootz {

namespace {

unsigned int chooseSampleRate(const RtAudio::DeviceInfo &info) {
    static constexpr unsigned int preferredRates[] = {48000, 44100, 96000};

    for (const auto rate : preferredRates) {
        if (std::find(info.sampleRates.begin(), info.sampleRates.end(), rate) !=
            info.sampleRates.end()) {
            return rate;
        }
    }

    return !info.sampleRates.empty() ? info.sampleRates.front() : 48000;
}

}  // namespace

TzootzAsioEngine::TzootzAsioEngine(RtAudio &audio) : audio_(audio) {
    options_.flags = RTAUDIO_MINIMIZE_LATENCY;
    options_.streamName = "TZOOTZ_ASIO Prototype";
}

TzootzAsioEngine::~TzootzAsioEngine() {
    try {
        stop();
    } catch (const RtAudioError &error) {
        std::cerr << "Error deteniendo el motor ASIO: " << error.getMessage() << '\n';
    }
}

void TzootzAsioEngine::initialize(const DeviceSelection &selection) {
    device_ = selection;
    sampleRate_ = chooseSampleRate(device_.info);
    bufferFrames_ = 128;

    outputParams_.deviceId = device_.deviceId;
    outputParams_.firstChannel = 0;
    outputParams_.nChannels = std::min<unsigned int>(device_.info.outputChannels, 2U);

    if (audio_.isStreamOpen()) {
        audio_.closeStream();
    }

    try {
        audio_.openStream(&outputParams_, nullptr, RTAUDIO_FLOAT32, sampleRate_, &bufferFrames_,
                          &TzootzAsioEngine::audioCallback, this, &options_);
    } catch (const RtAudioError &error) {
        std::cerr << "No se pudo abrir el stream ASIO: " << error.getMessage() << '\n';
        throw;
    }
}

void TzootzAsioEngine::start() {
    if (running_.exchange(true)) {
        return;
    }

    try {
        audio_.startStream();
        std::cout << "Motor ASIO iniciado en dispositivo: " << device_.info.name << '\n';
        std::cout << "Sample rate: " << sampleRate_ << " Hz, buffer: " << bufferFrames_
                  << " frames" << '\n';
    } catch (const RtAudioError &error) {
        running_ = false;
        std::cerr << "Error iniciando el stream ASIO: " << error.getMessage() << '\n';
        throw;
    }
}

void TzootzAsioEngine::stop() {
    if (!running_.exchange(false) && !audio_.isStreamRunning()) {
        return;
    }

    try {
        if (audio_.isStreamRunning()) {
            audio_.stopStream();
        }
    } catch (const RtAudioError &error) {
        std::cerr << "Error deteniendo el stream ASIO: " << error.getMessage() << '\n';
    }

    if (audio_.isStreamOpen()) {
        audio_.closeStream();
    }
}

int TzootzAsioEngine::audioCallback(void *outputBuffer, void *, unsigned int nBufferFrames,
                                    double, RtAudioStreamStatus status, void *userData) {
    auto *engine = static_cast<TzootzAsioEngine *>(userData);

    if (status != 0U) {
        std::cerr << "Advertencia: estado de stream RtAudio = " << status << '\n';
    }

    if (outputBuffer) {
        std::memset(outputBuffer, 0, nBufferFrames * engine->outputParams_.nChannels * sizeof(float));
    }

    return 0;
}

}  // namespace tzootz
