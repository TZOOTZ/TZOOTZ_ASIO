#include "RealtekDetector.h"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace {

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

}  // namespace

namespace tzootz {

std::optional<DeviceSelection> RealtekDetector::findPreferredDevice(RtAudio &audio) {
    const unsigned int deviceCount = audio.getDeviceCount();
    std::optional<DeviceSelection> fallback;

    for (unsigned int deviceId = 0; deviceId < deviceCount; ++deviceId) {
        RtAudio::DeviceInfo info;
        try {
            info = audio.getDeviceInfo(deviceId);
        } catch (const RtAudioError &error) {
            std::cerr << "Error obteniendo información del dispositivo " << deviceId << ": "
                      << error.getMessage() << '\n';
            continue;
        }

        if (!info.probed) {
            continue;
        }

        if (!fallback && info.outputChannels > 0) {
            fallback = DeviceSelection{deviceId, info};
        }

        if (isLikelyRealtek(info)) {
            return DeviceSelection{deviceId, info};
        }
    }

    return fallback;
}

bool RealtekDetector::isLikelyRealtek(const RtAudio::DeviceInfo &info) {
    const std::string lowerName = toLower(info.name);
    return lowerName.find("realtek") != std::string::npos ||
           lowerName.find("high definition audio") != std::string::npos;
}

}  // namespace tzootz
