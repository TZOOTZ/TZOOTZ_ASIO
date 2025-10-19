# TZOOTZ_ASIO

TZOOTZ_ASIO es un esfuerzo de código abierto para ofrecer un driver ASIO moderno y sin fricciones para Windows. El objetivo es proporcionar una experiencia cercana a Core Audio en macOS para estaciones de trabajo de audio digital (DAW) que hoy dependen de soluciones genéricas como ASIO4ALL o FlexASIO.

## Visión general

- **Zero configuration:** detección y puesta en marcha automática para chipsets Realtek, Intel HD Audio y derivados.
- **Baja latencia:** objetivo de 2-5 ms con búferes inteligentes.
- **Estabilidad:** manejo robusto de errores y reinicios del motor de audio.
- **Compatibilidad universal:** arquitectura extensible que se apoya en WDM Kernel Streaming.

## Prototipo mínimo

Este repositorio incluye un prototipo basado en [RtAudio](https://github.com/thestk/rtaudio) para validar la enumeración de dispositivos y la inicialización del flujo ASIO sobre hardware Realtek:

- Enumeración automática de dispositivos e identificación heurística de chipsets Realtek.
- Selección de parámetros de audio (sample rate, profundidad de bits y tamaño de búfer) a partir de las capacidades reportadas por el dispositivo.
- Apertura y arranque de un stream RtAudio con un callback de salida silencioso.

> ⚠️ El prototipo requiere Windows y hardware compatible para ejecutarse con ASIO. En otros sistemas operativos RtAudio cambiará automáticamente al backend nativo disponible, por lo que el comportamiento no representará al driver final.

## Requisitos

- Windows 10/11 x64
- [CMake 3.20+](https://cmake.org/download/)
- [Visual Studio 2022](https://visualstudio.microsoft.com/) con toolset MSVC v143
- SDK de RtAudio (se obtiene automáticamente mediante *FetchContent*)

## Compilación

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

## Uso

1. Ejecuta `tzootz_asio.exe`.
2. El prototipo buscará un dispositivo cuyo nombre contenga "Realtek".
3. Si lo encuentra, intentará abrir un stream ASIO con búfer de 128 samples y sample rate óptimo.
4. Pulsa **Enter** para detener el stream y cerrar la aplicación.

## Roadmap

1. **Fase de prototipado**
   - Configuración básica del SDK ASIO 2.3.3.
   - Integración con WDM Kernel Streaming.
   - Gestión de búferes e hilos en C++17.
2. **Fase de características core**
   - Ajuste fino de latencia y tamaño de búfer.
   - Telemetría y diagnóstico en tiempo real.
   - Validación con los DAWs más populares (Ableton Live, Reaper, Cubase).

## Licencia

Este proyecto está licenciado bajo los términos de la licencia MIT. Consulta el archivo `LICENSE` (por definir) para más detalles.
