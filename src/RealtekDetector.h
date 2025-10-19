#pragma once

#include <optional>
#include <string>

#include <RtAudio.h>

namespace tzootz {

struct DeviceSelection {
    unsigned int deviceId;
    RtAudio::DeviceInfo info;
};

class RealtekDetector {
public:
    [[nodiscard]] static std::optional<DeviceSelection> findPreferredDevice(RtAudio &audio);

private:
    [[nodiscard]] static bool isLikelyRealtek(const RtAudio::DeviceInfo &info);
};

}  // namespace tzootz
