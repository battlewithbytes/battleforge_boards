# WASM Build Scripts

Build scripts for compiling Clang/LLVM and LLD to WebAssembly for in-browser compilation.

## Pre-built Binaries

Pre-built WASM binaries are available in [GitHub Releases](https://github.com/battlewithbytes/battleforge_boards/releases).

| Binary | Version | Size | Targets |
|--------|---------|------|---------|
| clang-arm.wasm.gz | 20.0.0 | 21 MB | STM32, nRF52, RP2040 (Cortex-M) |
| clang-riscv.wasm.gz | 19.0.0 | 16 MB | ESP32-C3, ESP32-C6, ESP32-H2 |
| clang-xtensa.wasm.gz | 18.1.2-esp | 14 MB | ESP32, ESP32-S2, ESP32-S3 |
| lld.wasm.gz | 18.1.2-esp | 10 MB | ARM, RISC-V, Xtensa |

## Building From Source

### Requirements

- Linux build machine (Ubuntu 22.04+ recommended)
- 32GB+ RAM (64GB recommended for parallel builds)
- 50GB+ disk space
- CMake 3.20+
- Ninja build system
- WASI SDK 27+ (downloaded automatically by ARM script)
- Emscripten SDK (for LLD build)

### Build Scripts

Each script is self-contained and will download/setup dependencies:

```bash
# ARM compiler (STM32, nRF52, RP2040)
./build-clang-arm.sh

# RISC-V compiler (ESP32-C3, C6, H2)
./build-clang-riscv.sh

# Xtensa compiler (ESP32, S2, S3) - uses Espressif LLVM fork
./build-clang-xtensa.sh

# Universal LLD linker (all architectures)
./build-lld-universal.sh
```

### Build Time

On a 128-thread build server:
- ARM: ~15 minutes
- RISC-V: ~20 minutes
- Xtensa: ~25 minutes
- LLD: ~8 minutes

### Output

Each script outputs to `~/clang-wasm-builds/output/`:
- `clang-arm.wasm.gz` - Compressed ARM compiler
- `clang-riscv.wasm.gz` - Compressed RISC-V compiler
- `clang-xtensa.wasm.gz` - Compressed Xtensa compiler
- `lld.wasm.gz` - Compressed universal linker

## Architecture Notes

### ARM Compiler
- Built from upstream LLVM 20.0
- Uses WASI SDK for compilation
- Supports all Cortex-M variants (M0, M0+, M3, M4, M4F, M7, M33)

### RISC-V Compiler
- Built from upstream LLVM 19.0
- Uses Emscripten for WASM compilation
- Targets rv32imac (ESP32-C series)

### Xtensa Compiler
- Built from [Espressif LLVM fork](https://github.com/espressif/llvm-project)
- Xtensa is an experimental target requiring special build flags
- Supports LX6 (ESP32) and LX7 (ESP32-S2, S3) cores

### LLD Universal Linker
- Single binary supporting all three architectures
- Built with ARM, RISC-V, and Xtensa backends
- Reduces total download size vs separate linkers

## Manifest

The `manifest.json` in releases contains metadata for each binary:
- Version information
- File sizes
- SHA256 hashes for integrity verification
- Supported architectures

## License

Build scripts are MIT licensed. LLVM/Clang is Apache 2.0 with LLVM Exception.
