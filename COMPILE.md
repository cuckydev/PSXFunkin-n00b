# Compiling PSXFunkin

## Setting up the Development Environment
First off, you'll need a terminal one way or another.

### Windows
On Windows, you basically have two choices:
- MSYS2 (follow the instructions at https://www.msys2.org/ to set it up)
- WSL (Windows 10 Linux terminal from the Windows Store, maintained by Microsoft, follow Linux instructions from here on).

### MSYS2
Once you have MSYS2 set up, you'll need to copy the MIPS toolchain over. Download it here http://static.grumpycoder.net/pixel/mips/g++-mipsel-none-elf-11.2.0.zip.

Once you have it downloaded, make sure MSYS2 is closed, then open the zip up and extract the following folders into `C:/msys64/usr/local/`
- bin
- include
- lib
- libexec
- mipsel-none-elf

**IMPORTANT - DELETE THESE FILES IN `bin` OR YOUR TERMINAL WILL BE FUCKED**
- cat
- cp
- echo
- make
- mkdir
- touch
- rm
- touch

Next, open up `MSYS2 MinGW 64-bit` from the Start Menu, and you'll need to install some libraries, so run the following command and accept the prompts that follow `pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-tinyxml2 mingw-w64-x86_64-ffmpeg `

### Linux
First, you'll need to install the GCC toolchain, run one of the following commands depending on your distro.

On Debian derivatives (Ubuntu, Mint...):

`sudo apt-get install gcc-mipsel-linux-gnu g++-mipsel-linux-gnu binutils-mipsel-linux-gnu`

On Arch derivatives (Manjaro), the mipsel environment can be installed from AUR : cross-mipsel-linux-gnu-binutils and cross-mipsel-linux-gnu-gcc using your AURhelper of choice:

`trizen -S cross-mipsel-linux-gnu-binutils cross-mipsel-linux-gnu-gcc`

You'll also need to install `ffmpeg` (you may also need to install `avformat`, `avfilter`, and `swscale` separately), and `cmake`, which of course, depends on your distro of choice.

## Compiling PSn00bSDK
First, `cd` to somewhere safe where you can put source code (such as `Documents` or the PSXFunkin folder). Then run these few commands.

- `git clone https://github.com/spicyjpeg/PSn00bSDK`

- `cd PSn00bSDK`

- `git checkout psxmdec`

- `git submodule update --init --recursive --remote`

- `cmake --preset default . --install-prefix /usr/local/` (Linux users can change install-prefix to whatever they want, MSYS2 users should not touch it at all)

- `cmake --build ./build`

- `cmake --install ./build`

## License file
You'll need to either get a PSX license file and save it as licensea.dat in the `iso/` directory (You find license files within http://www.psxdev.net/downloads.html PsyQ SDK).

Optionally, you can simplpy remove the referencing line `<license file="${PROJECT_SOURCE_DIR}/iso/LICENSEA.DAT"/>` from funkin.xml.

Without the license file, the game may fail on certain emulators due to BIOS checks or console detection.

## Compiling PSXFunkin
First, make sure to `cd` to the repo directory where all the makefile is. You're gonna want to run a few commands from here.

Now, you want to configure PSXFunkin, you have to do this before first compilation and when you want to change the configuration, such as the Title ID.

- `cmake --preset default . -DTITLE_ID="FUNK-00001"` (If you have a Title ID, you can change it here)

- `cmake --build ./build`

This will compile PSXFunkin and generate the .iso file in `build/`
