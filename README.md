# BattleForge Boards Registry

Board definitions, platform configurations, and libraries for the BattleForge embedded IDE.

## Repository Structure

```
battleforge_boards/
├── registry.json           # Main index file
├── schema/                 # JSON Schema definitions
│   ├── platform-v1.schema.json   # Legacy embedded files format
│   ├── platform-v2.schema.json   # External GitHub sources format
│   ├── board-v1.schema.json
│   └── library-v1.schema.json
├── platforms/              # Platform family manifests
│   ├── stm32/
│   │   ├── platform.json         # Platform metadata
│   │   └── f1/
│   │       ├── manifest.json     # v1: embedded headers/linkers
│   │       └── manifest-v2.json  # v2: GitHub source references
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
├── wasm/                   # WASM compiler manifests
│   └── manifest.json       # Compiler versions and hashes
└── scripts/
    └── wasm/               # WASM compiler build scripts
```

## How It Works

1. BattleForge loads `registry.json` at startup
2. User selects a platform (STM32, ESP32, etc.)
3. Platform's `index.json` lists available boards
4. Board's manifest provides build configuration
5. Platform family manifest (v1 or v2) provides headers/startup/linker files

## Manifest Versions: v1 vs v2

BattleForge supports two manifest formats for platform families:

### v1 (Legacy - Embedded Files)
- Headers, startup files, and linker scripts are stored directly in this repository
- Files bundled with the app at build time
- Suitable for small platforms or when offline support is critical

### v2 (Current - External GitHub Sources)
- References external GitHub repositories (e.g., ST's official CMSIS repos)
- Files fetched at runtime via `SourceFetcher.ts`
- Always up-to-date with vendor SDKs
- Smaller repository size
- Requires network access

BattleForge tries to load `manifest-v2.json` first, falling back to `manifest.json` (v1).

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

## Adding a Platform Family (v2 Manifest)

The v2 manifest format references external GitHub repositories for SDK files instead of embedding them.

### Create `platforms/{platform}/{family}/manifest-v2.json`

```json
{
  "$schema": "../../../../schema/platform-v2.schema.json",
  "schemaVersion": "2.0.0",
  "platform": "stm32",
  "family": "f1",
  "name": "STM32F1 Series",
  "description": "STM32F1 Cortex-M3 microcontrollers",
  "architecture": "cortex-m3",
  "version": "2.0.0",

  "sources": {
    "cmsis": {
      "repo": "github:STMicroelectronics/cmsis_device_f1",
      "ref": "master",
      "paths": {
        "headers": "Include",
        "startup": "Source/Templates/gcc",
        "linker": "Source/Templates/gcc/linker",
        "system": "Source/Templates"
      },
      "files": {
        "headers": [
          "stm32f1xx.h",
          "stm32f103xb.h",
          "system_stm32f1xx.h"
        ],
        "system": ["system_stm32f1xx.c"]
      }
    },
    "cmsis_core": {
      "repo": "github:STMicroelectronics/cmsis_core",
      "ref": "master",
      "paths": {
        "headers": "Include"
      },
      "files": {
        "headers": ["core_cm3.h", "cmsis_gcc.h", "cmsis_compiler.h"]
      }
    }
  },

  "devices": [
    {
      "id": "stm32f103c8",
      "name": "STM32F103C8",
      "flash": 65536,
      "ram": 20480,
      "frequency": 72000000,
      "defines": ["STM32F103xB", "STM32F1"],
      "fpu": "none",
      "files": {
        "startup": "startup_stm32f103xb.s",
        "linker": "STM32F103XB_FLASH.ld",
        "header": "stm32f103xb.h"
      }
    }
  ],

  "build": {
    "compilerFlags": [
      "--target=thumbv7m-none-eabi",
      "-mcpu=cortex-m3",
      "-mthumb",
      "-mfloat-abi=soft"
    ],
    "linkerFlags": ["--gc-sections"],
    "defines": ["USE_HAL_DRIVER"]
  },

  "frameworks": {
    "arduino": {
      "core": "stm32duino",
      "coreUrl": "https://github.com/stm32duino/Arduino_Core_STM32",
      "packageIndex": "https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json"
    },
    "native": {
      "sdk": "stm32cube",
      "sdkVersion": "1.8.5"
    }
  }
}
```

### v2 Sources Schema

The `sources` object defines external repositories:

| Field | Required | Description |
|-------|----------|-------------|
| `repo` | Yes | Repository in format `github:owner/repo` or full URL |
| `ref` | No | Git ref (branch, tag, commit). Default: `master` |
| `paths.headers` | No | Directory containing header files |
| `paths.startup` | No | Directory containing startup assembly files |
| `paths.linker` | No | Directory containing linker scripts |
| `paths.system` | No | Directory containing system init files |
| `files.headers` | No | Specific header files to fetch |
| `files.system` | No | Specific system files to include in project |

### v2 Device Definition

Each device in the `devices` array:

| Field | Required | Description |
|-------|----------|-------------|
| `id` | Yes | Device identifier (e.g., `stm32f103c8`) |
| `name` | Yes | Human-readable name |
| `flash` | Yes | Flash size in bytes |
| `ram` | Yes | RAM size in bytes |
| `frequency` | Yes | CPU frequency in Hz |
| `defines` | No | Device-specific preprocessor defines |
| `fpu` | No | FPU config: `none`, `soft`, `softfp`, `hard` |
| `files.startup` | No | Startup file name (from sources paths) |
| `files.linker` | No | Linker script name (from sources paths) |
| `files.header` | No | Device-specific header file |

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

| Architecture | Platforms | WASM Compiler |
|--------------|-----------|---------------|
| `cortex-m0` | STM32F0, nRF51 | clang-arm |
| `cortex-m0+` | STM32L0, RP2040 | clang-arm |
| `cortex-m3` | STM32F1, STM32F2 | clang-arm |
| `cortex-m4` | STM32F3, STM32F4, nRF52 | clang-arm |
| `cortex-m4f` | STM32F4 (with FPU) | clang-arm |
| `cortex-m7` | STM32F7, STM32H7 | clang-arm |
| `cortex-m7f` | STM32F7, STM32H7 (with FPU) | clang-arm |
| `cortex-m23` | STM32L0+, secure IoT | clang-arm |
| `cortex-m33` | STM32L5, STM32U5 | clang-arm |
| `cortex-m55` | STM32U5 (ML) | clang-arm |
| `riscv32` | ESP32-C3, ESP32-C6, ESP32-H2 | clang-riscv |
| `riscv32imc` | ESP32-C3 (compact) | clang-riscv |
| `riscv32imac` | ESP32-C6 (atomic) | clang-riscv |
| `xtensa-lx6` | ESP32 | clang-xtensa |
| `xtensa-lx7` | ESP32-S2, ESP32-S3 | clang-xtensa |

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
