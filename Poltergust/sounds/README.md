# Archivos de Sonido para SD Card 🔊👻

## Instrucciones

1. Formatear microSD como **FAT32**
2. Crear carpeta `mp3` en la raíz (opcional, DFPlayer también lee de raíz)
3. Nombrar archivos con prefijo numérico: `0001_nombre.mp3`

## Lista de Archivos Necesarios

| # | Archivo | Descripción | Duración |
|---|---------|-------------|----------|
| 1 | `0001_vacuum.mp3` | Sonido aspiradora (loop) | 3-5 seg |
| 2 | `0002_strobulb.mp3` | Flash de cámara/luz | < 1 seg |
| 3 | `0003_theme.mp3` | Tema Luigi's Mansion | 30+ seg |
| 4 | `0004_capture.mp3` | Victoria al capturar | 2 seg |
| 5 | `0005_ghost_alert.mp3` | Alerta fantasma apareció | 2 seg |
| 6 | `0006_ghost_escape.mp3` | Fantasma escapó | 2 seg |
| 7 | `0007_power_on.mp3` | Sonido encendido | 1-2 seg |
| 8 | `0008_power_off.mp3` | Sonido apagado | 1-2 seg |

## Dónde Conseguir Sonidos

- **The Sounds Resource**: https://www.sounds-resource.com/nintendo_switch/luigismansion3/
- **YouTube** → convertir con sitios como y2mate
- **Freesound.org** para efectos genéricos

## Formato Recomendado

- **MP3** o **WAV**
- **Bitrate**: 128kbps es suficiente
- **Sample rate**: 44100 Hz
- **Mono** (reduce tamaño)

## Estructura en la SD

```
microSD/
├── 0001_vacuum.mp3
├── 0002_strobulb.mp3
├── 0003_theme.mp3
├── 0004_capture.mp3
├── 0005_ghost_alert.mp3
├── 0006_ghost_escape.mp3
├── 0007_power_on.mp3
└── 0008_power_off.mp3
```
