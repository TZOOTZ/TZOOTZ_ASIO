#include <iostream>

#include <RtAudio.h>

#include "RealtekDetector.h"
#include "TzootzAsioEngine.h"

int main() {
    try {
#ifdef _WIN32
        RtAudio audio(RtAudio::WINDOWS_ASIO);
#else
        RtAudio audio;
#endif

        auto selection = tzootz::RealtekDetector::findPreferredDevice(audio);
        if (!selection) {
            std::cerr << "No se encontró un dispositivo compatible (Realtek o equivalente)." << std::endl;
            return 1;
        }

        tzootz::TzootzAsioEngine engine(audio);
        engine.initialize(*selection);
        engine.start();

        std::cout << "Prototipo TZOOTZ_ASIO en ejecución. Pulsa Enter para detener." << std::endl;
        std::cin.get();

        engine.stop();

        return 0;
    } catch (const RtAudioError &error) {
        std::cerr << "Error RtAudio: " << error.getMessage() << std::endl;
        return 2;
    } catch (const std::exception &ex) {
        std::cerr << "Excepción no controlada: " << ex.what() << std::endl;
        return 3;
    }
}
