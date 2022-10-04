# Compiler setup

Currently, only Bartman's and Bebbo's GCC are supported.

## Bartman's toolchain

Bartman has created excellent extension for Visual Studio Code, which comes with latest GCC version and integrated debugging and profiling features.
To download it, in VSCode go to Extensions page and install `bartmanabyss.amiga-debug` extension.

Note: Bartman's extension contains prebuilt compiler and slightly modified WinUAE, hence it's **Windows-only**.

Note: currently, only the very old version extension is available directly through VSCode Extensions.
To obtain the newest package:

- navigate to [GitHub Releases](https://github.com/BartmanAbyss/vscode-amiga-debug/releases) page,
- download latest .vsix file
- in VSCode, enter Extensions tab (<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>X</kbd>), select "..." icon and select "Install from VSIX..." option.

### Using Bartman's compiler with CMake

This is recommended approach by [Last Minute Creations](https://github.com/Last-Minute-Creations) team members.
The setup is a bit troublesome but in the end it allows swapping compiler with Bebbo variant easily and makes it more portable.

- Download and install any MinGW GCC compiler (e.g. [7.3.0-i686-posix-dwarf](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/7.3.0/threads-posix/dwarf/i686-7.3.0-release-posix-dwarf-rt_v5-rev0.7z/download)) if you don't have any already in your system (MSVC and others will work too).
- Make sure the directory containing the GCC executable is in the system PATH.
- Download and install [CMake](https://cmake.org) if you haven't already.
  Be sure its executable directory is in your PATH or you'll have to configure vscode extension with it manually.
- In VSCode, install `twxs.cmake` and `ms-vscode.cmake-tools` extensions for CMake support.
- If CMake's executable is not in your system's PATH, enter VSCode settings and set full path to cmake.exe.
- Clone the [AmigaCMakeCrossToolchains](https://github.com/AmigaPorts/AmigaCMakeCrossToolchains) repo.
  Be sure you have `m68k-bartman.cmake` file there.
- Confiure VSCode CMake extension:
  - In the IDE, open any directory which will contain your project.
  - Create empty CMakeLists.txt file **and restart the editor** so that it can discover that you're inside CMake-based project.
  - Enter command palette (<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>P</kbd>) and start typing "CMake Scan for kits" and select it from the options.
  - Enter command palette again and select "CMake Edit User-Local CMake Kits"
  - You will see a .json file which holds the array of configurations of compilers in your system.
    There will be one matching the MinGW compiler, but you need to append the configuration for Bartman suite manually.
    Simply add a comma after last compiler config in the file and add the following, replacing `YOUR_USER_NAME`, `m68k-bartman.cmake` path and extension name/version so that it matches your filesystem:

    ```json5
    [
      {
        "name": "Some other compiler which was automatically detected",
        // Some other fields...
      },
      {
        "name": "GCC Bartman m68k",
        "toolchainFile": "C:\\PATH_TO\\AmigaCMakeCrossToolchains\\m68k-bartman.cmake",
        "compilers": {
          "C": "C:\\Users\\YOUR_USER_NAME\\.vscode\\extensions\\bartmanabyss.amiga-debug-1.1.0-preview33\\bin\\opt\\bin\\m68k-amiga-elf-gcc.exe",
          "CXX": "C:\\Users\\YOUR_USER_NAME\\.vscode\\extensions\\bartmanabyss.amiga-debug-1.1.0-preview33\\bin\\opt\\bin\\m68k-amiga-elf-g++.exe"
        },
        "environmentVariables": {
          "PATH": "C:/Users/YOUR_USER_NAME/.vscode/extensions/bartmanabyss.amiga-debug-1.1.0-preview33/bin/opt/bin;C:/Users/YOUR_USER_NAME/.vscode/extensions/bartmanabyss.amiga-debug-1.1.0-preview33/bin;${env:PATH}"
        },
        "preferredGenerator": {
          "name": "MinGW Makefiles"
        },
        "cmakeSettings": {
          "M68K_CPU": "68000",
          "TOOLCHAIN_PREFIX": "m68k-amiga-elf",
          "TOOLCHAIN_PATH": "C:/Users/YOUR_USER_NAME/.vscode/extensions/bartmanabyss.amiga-debug-1.1.0-preview33/bin/opt"
        },
        "keep": true
      }
    ]
    ```

After this you should be good to go.

- From the command palette, select "CMake Configure" to configure your project.
- The <kbd>F7</kbd> key builds your project,
- The <kbd>F5</kbd> key runs currently selected debugging task.

Note: The Bartman's compiler produces ELF executables instead of Hunk file format.
To mitigate this, you need to invoke `elf2hunk` bundled with Bartman's toolchain.
You may want to look at how its made in CMakeLists of some other projects (e.g. [GermZ](https://github.com/tehKaiN/germz)) to see how ACE is included and debugger is invoked. Also, you may want to further investigate `CMakeLists.txt` there and `.vscode/launch.json` to see how debugger is set up.

### Integrating ACE into Bartman's sample project

TBD

The Bartman plugin comes with a command to bootstrap the makefile-based sample project.
A dirty way is to copy src/inc files from ACE to the base project.

## Bebbo's toolchain

You can get Bebbo's compiler from his [amiga-gcc](https://github.com/bebbo/amiga-gcc) repository.
You can download precompiled libraries (May require MSYS2 installed) or build it from source.
Its build system tends to be occasionally broken, so if you can't build it by following his instructions, don't be shy and drop him an issue. ;)
Also, if you're on Windows, you'll need Cygwin, MSYS2 or Windows Subsystem for Linux.

### Building with WSL

This is currently the preferred way to build, but requires Windows 10.
After installing WSL in your OS and Ubuntu on it, follow instructions from Bebbo's repo.

### Building with Cygwin

You may want to create a shortcut which launches `cmd` with cygwin & compiler paths in PATH:

```plain
C:\WINDOWS\system32\cmd.exe /c "SET PATH=path/to/bebbo/bin;path/to/cygwin/bin;%PATH%&& set PREFIX=/cygdrive/path/to/bebbo&& START cmd.exe"
```

Where `path/to/bebbo` is a path to destination where Bebbo's compiler is installed. On Cygwin, path `X:/dir1/dir2/file` gets changed to `/cygdrive/x/dir1/dir2/file` so be sure to setup `PREFIX` in proper way.
Before building ACE or Bebbo's compiler, enter `sh` or `bash` or you will experience strange errors.

Be sure there are no spaces before `&&` or one of `rm -rf` may get an empty arg and destroy your HDD contents. :)

## Prebuilt VBCC for Windows - **no longer supported**

**ACE doesn't work with VBCC Anymore.**
VBCC doesn't support latest C standards, popular language extensions, and its optimizers produce broken code.
This section is only kept for archive reasons, or in case VBCC gets better in the future.

If you're allergic to building anything and are using Windows, Kusma maintains
prebuilt VBCC on his [amiga-dev](https://github.com/kusma/amiga-dev) repo.

As noted there, you'll need to set up system variables:

- `VBCC`: your `vbcc\targets\m68k-amigaos` directory,
- `PATH`: add your `vbcc\bin` dir.

## Which compiler to choose

Bartman's GCC uses newer version (at the time of writing, GCC 10.1 vs GCC 6.5), has the debugger, latest C/C++ standards support, but doesn't have properly implemented standard library.
To mitigate this problem, ACE implements its basic parts in mini-std, but it's not and will never be a complete implementation.
Bebbo implements his own optimizers in GCC code which may produce better code than Bartman's suite, but may also break your code until they get properly tested in the field.
I'm using following ruleset:

- use Bartman's GCC as main compiler for development, or Bebbo's if you need standard library.
- when doing release builds and your code doesn't require latest compiler features, do a Bebbo build and compare size and performance.
- if anything breaks mysteriously with Bebbo's compiler, disable optimizations in ACE/game CMakeLists (change `-O3` to `-O0` or build with `-DCMAKE_BUILD_TYPE=Debug`) and try adding `-fbbb=-` in compile options to disable Bebbo's optimizers.
- if problem still persists, it's probably your bug.
- if it's not, try to isolate where bug occurs and report it on ACE repo **with code sample**

## Integrating Bebbo's compiler with Visual Studio Code

If you don't know VSCode, give it a try - You won't go back to any other IDE.
To properly set it up for work with ACE, here are some tips on how to set it up in optimal way:

- If you've installed Bebbo compiler via WSL, install `ms-vscode-remote.remote-wsl` extension.
  It will allow you to open projects from WSL point of view.
- Install `twxs.cmake` and `ms-vscode.cmake-tools` extensions for CMake support. Note that you may need to reinstall some of the extensions mentioned above when connecting to WSL as they are stored on target system.
- Clone the [AmigaCMakeCrossToolchains](https://github.com/AmigaPorts/AmigaCMakeCrossToolchains) repo
- From Command Palette (<kbd>ctrl</kbd>+<kbd>shift</kbd>+<kbd>p</kbd>), select `CMake Edit User-Local CMake Kits`.

The CMake kit config is as follows:

```json5
[
  {
    "name": "Some other compiler which was automatically detected",
    // Some other fields...
  },
  {
    "name": "GCC for m68k-amigaos 6.5.0b",
    "compilers": {
      "C": "/opt/amiga/bin/m68k-amigaos-gcc",
      "CXX": "/opt/amiga/bin/m68k-amigaos-g++"
    },
    "toolchainFile": "/path/to/AmigaCMakeCrossToolchains/m68k-amigaos.cmake",
    "cmakeSettings": {
      "M68K_CPU": "68000",
      "M68K_FPU": "soft",
      "M68K_CRT": "nix13",
      "TOOLCHAIN_PREFIX": "m68k-amigaos",
      "TOOLCHAIN_PATH": "/opt/amiga"
    }
  }
]
```
