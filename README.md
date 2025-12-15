# BattleForge Boards Registry

Board definitions, platform configurations, and libraries for the BattleForge embedded IDE.

## Repository Structure

```
battleforge_boards/
├── registry.json           # Main index file
├── schema/                 # JSON Schema definitions
│   ├── platform-v1.schema.json
│   ├── board-v1.schema.json
│   └── library-v1.schema.json
├── platforms/              # Platform configurations
│   ├── stm32/
│   │   └── platform.json
│   ├── esp32/
│   ├── nrf/
│   └── rp2040/
├── stm32/                  # Board definitions by platform
│   ├── index.json          # Platform board index
│   └── generic/
│       └── bluepill_stm32f103c8.json
├── esp32/
├── nrf/
├── rp2040/
├── libraries/              # Library manifests
│   ├── freertos/
│   ├── wire/
│   └── spi/
├── images/                 # Board images
└── scripts/
    └── wasm/               # WASM compiler build scripts
```

## How It Works

1. BattleForge loads `registry.json` at startup
2. User selects a platform (STM32, ESP32, etc.)
3. Platform's `index.json` lists available boards
4. Board's manifest provides build configuration

## Adding a New Board

### Step 1: Create Board Manifest

Create a JSON file in the appropriate platform/vendor directory:

```
{platform}/{vendor}/{board_id}.json
```

Example: `stm32/generic/nucleo_f446re.json`

```json
{
  "$schema": "../../schema/board-v1.schema.json",
  "id": "nucleo_f446re",
  "name": "NUCLEO-F446RE",
  "vendor": "stmicroelectronics",
  "version": "1.0.0",
  "description": "STM32 Nucleo-64 development board with STM32F446RE MCU",
  "image": "images/stmicroelectronics/nucleo_f446re.png",

  "chip": {
    "platform": "stm32",
    "family": "f4",
    "device": "stm32f446re",
    "architecture": "cortex-m4f"
  },

  "memory": {
    "flash": 524288,
    "flashAvailable": 524288,
    "ram": 131072,
    "ramAvailable": 131072
  },

  "build": {
    "frequency": 180000000,
    "defines": [
      "STM32F446xx",
      "STM32F4",
      "BOARD_NUCLEO_F446RE",
      "HSE_VALUE=8000000"
    ],
    "linkerScript": "STM32F446RETx_FLASH.ld"
  },

  "pins": {
    "ledBuiltin": { "pin": "PA5", "arduino": 13 },
    "serial": { "rx": "PA3", "tx": "PA2" },
    "i2c": { "sda": "PB9", "scl": "PB8" },
    "spi": { "mosi": "PA7", "miso": "PA6", "sck": "PA5", "ss": "PB6" }
  },

  "frameworks": {
    "arduino": {
      "core": "stm32duino",
      "variant": "NUCLEO_F446RE",
      "defines": ["ARDUINO=10819", "ARDUINO_NUCLEO_F446RE"]
    }
  },

  "upload": {
    "methods": ["stlink", "jlink"],
    "default": "stlink"
  },

  "features": ["usb", "can", "i2c", "spi", "uart", "adc", "dac"],
  "tags": ["nucleo", "stm32f4", "st-link", "arduino-compatible"]
}
```

### Step 2: Add to Platform Index

Add the board to `{platform}/index.json`:

```json
{
  "boards": [
    {
      "id": "nucleo_f446re",
      "name": "NUCLEO-F446RE",
      "vendor": "stmicroelectronics",
      "family": "f4",
      "path": "stm32/stmicroelectronics/nucleo_f446re.json",
      "features": ["usb", "can", "i2c", "spi"],
      "tags": ["nucleo", "stm32f4"],
      "exampleCount": 0
    }
  ]
}
```

### Step 3: Add Board Image (Optional)

Place a 400x300 PNG image at:
```
images/{vendor}/{board_id}.png
```

## Adding a New Platform

### Step 1: Create Platform Config

Create `platforms/{platform}/platform.json`:

```json
{
  "id": "avr",
  "name": "AVR",
  "vendor": "Microchip",
  "description": "Classic 8-bit AVR microcontrollers",
  "icon": "avr",
  "color": "#E60012",
  "website": "https://www.microchip.com/avr",
  "families": ["atmega", "attiny"]
}
```

### Step 2: Create Board Index

Create `{platform}/index.json`:

```json
{
  "platform": "avr",
  "version": "1.0.0",
  "lastUpdated": "2025-01-01",
  "vendors": [
    {
      "id": "arduino",
      "name": "Arduino",
      "boardCount": 5,
      "indexPath": "arduino/index.json"
    }
  ],
  "boards": []
}
```

### Step 3: Register in registry.json

Add to the platforms array:

```json
{
  "platforms": [
    {
      "id": "avr",
      "configPath": "platforms/avr/platform.json",
      "boardsPath": "avr/index.json"
    }
  ]
}
```

## Adding a Library

### Step 1: Create Library Manifest

Create `libraries/{library_id}/manifest.json`:

```json
{
  "$schema": "../../schema/library-v1.schema.json",
  "id": "adafruit_ssd1306",
  "name": "Adafruit SSD1306",
  "version": "2.5.7",
  "description": "SSD1306 OLED display driver",
  "author": "Adafruit",
  "license": "BSD",
  "repository": "https://github.com/adafruit/Adafruit_SSD1306",

  "url": "/libs/adafruit_ssd1306-2.5.7.tar.gz",
  "hash": "sha256:abc123...",

  "platforms": ["stm32", "nrf", "esp32", "rp2040"],
  "architectures": ["cortex-m0", "cortex-m3", "cortex-m4"],

  "includes": ["src"],
  "sources": ["src/*.cpp"],

  "dependencies": ["adafruit_gfx", "wire"],

  "categories": ["display", "oled"],
  "keywords": ["ssd1306", "oled", "display", "i2c"]
}
```

### Step 2: Register in registry.json

Add to the libraries array:

```json
{
  "libraries": [
    {
      "id": "adafruit_ssd1306",
      "name": "Adafruit SSD1306",
      "version": "2.5.7",
      "category": "display",
      "path": "libraries/adafruit_ssd1306/manifest.json"
    }
  ]
}
```

## Adding Code Templates/Examples

Examples are stored within board definitions. Add to the board manifest:

```json
{
  "examples": [
    {
      "id": "blink",
      "name": "Blink LED",
      "description": "Classic LED blink example",
      "files": {
        "main.cpp": "examples/blink/main.cpp"
      },
      "category": "basics"
    },
    {
      "id": "uart_echo",
      "name": "UART Echo",
      "description": "Echo characters over serial",
      "files": {
        "main.cpp": "examples/uart/main.cpp"
      },
      "category": "communication"
    }
  ]
}
```

Example files go in `{platform}/{vendor}/examples/`:
```
stm32/generic/examples/
├── blink/
│   └── main.cpp
└── uart/
    └── main.cpp
```

## Schema Reference

### Board Manifest Fields

| Field | Required | Description |
|-------|----------|-------------|
| `id` | Yes | Unique board identifier (lowercase, underscores) |
| `name` | Yes | Display name |
| `vendor` | Yes | Manufacturer/vendor ID |
| `chip.platform` | Yes | Platform ID (stm32, esp32, etc.) |
| `chip.family` | Yes | Chip family (f1, f4, c3, etc.) |
| `chip.device` | Yes | Specific device (stm32f103c8) |
| `chip.architecture` | Yes | CPU architecture (cortex-m3) |
| `memory.flash` | Yes | Total flash in bytes |
| `memory.ram` | Yes | Total RAM in bytes |
| `build.frequency` | Yes | Default clock speed in Hz |
| `build.defines` | Yes | Compiler defines |
| `pins` | No | Pin mappings |
| `frameworks` | No | Framework-specific config |
| `features` | No | Feature tags for filtering |
| `tags` | No | Search tags |

### Supported Architectures

| Architecture | Platforms |
|--------------|-----------|
| `cortex-m0` | STM32F0, nRF51 |
| `cortex-m0+` | STM32L0, RP2040 |
| `cortex-m3` | STM32F1, STM32F2 |
| `cortex-m4` | STM32F3, STM32F4, nRF52 |
| `cortex-m4f` | STM32F4 (with FPU) |
| `cortex-m7` | STM32F7, STM32H7 |
| `cortex-m33` | STM32L5, STM32U5 |
| `riscv32` | ESP32-C3, ESP32-C6 |
| `xtensa-lx6` | ESP32 |
| `xtensa-lx7` | ESP32-S2, ESP32-S3 |

## WASM Compiler Builds

Pre-built WASM compilers are available in [GitHub Releases](https://github.com/battlewithbytes/battleforge_boards/releases).

Build scripts are in `scripts/wasm/`. See [scripts/wasm/README.md](scripts/wasm/README.md) for build instructions.

## Validation

Validate your JSON files against the schemas:

```bash
# Using ajv-cli
npx ajv validate -s schema/board-v1.schema.json -d stm32/generic/myboard.json
```

## Contributing

1. Fork the repository
2. Create a branch for your changes
3. Add/modify board definitions
4. Validate against schemas
5. Submit a pull request

### Guidelines

- Use lowercase IDs with underscores: `bluepill_f103c8`
- Include accurate memory sizes (check datasheet)
- Provide complete pin mappings where possible
- Add meaningful tags for searchability
- Test with BattleForge before submitting

## License

MIT License - Board definitions and build scripts are freely usable.
