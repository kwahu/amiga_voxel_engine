# Building ACE

Historically, ACE could be built using make, ace-cli and CMake. Currently, only CMake is supported.

## Building using CMake

Currently CMake build supports only GCC (be it Bebbo's or Bartman's version). You need to use the appropriate
CMake Toolchain File. Start with cloning the
[AmigaCMakeCrossToolchains](https://github.com/AmigaPorts/AmigaCMakeCrossToolchains) repo.

This toolchain file allows you to set `M68K_CPU` and `M68K_FPU` variables to your liking.

### Using ACE as submodule dependency in your project

This is the recommended way of building ACE. Since ACE's development often breaks things, it's best to attach ACE as a submodule in your game's repository. This way, your commit history will store the last-known good ACE commit which worked well with your project.

To attach ACE repository as a submodule, do the following in your repo's root directory:

```sh
mkdir deps
git submodule add https://github.com/AmigaPorts/ACE deps/ace
```

And to link it to your main executable:

```cmake
add_subdirectory(deps/ace)
target_link_libraries(myGame ace)
```

### Building standalone library

``` sh
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/AmigaCMakeCrossToolchains/m68k.cmake -DM68K_TOOLCHAIN_PATH=/path/to/toolchain -DM68K_CPU=68000 -DM68K_FPU=soft
make
```

Some notes:

- You can pass other `-DM68K_CPU` values. Supported are `68000`, `68010`, `68020`, `68040` and `68060`. See AmigaCMakeCrossToolchains docs or sources for more info.
- If you're on cygwin, you might need to add `-G"Unix Makefiles"`.
- If you want to enable debug build (e.g. to have logs and better sanity checks), pass `-DCMAKE_BUILD_TYPE=Debug`.
- If you really want to depend on standalone-built ACE library, be sure to take note of the commit you've built it from.
  ACE breaks things very often and it's almost certain that after some time you won't be able to build your game with latest ACE version.

After building, you should have `libace.a` in your build folder.
Be sure to link it to your game.

## Building older versions using GNU Make

The following will build `.o` files:

``` sh
cd path/to/ace
make all [OPTIONS]
```

You can build ace by passing several additional options using `OPTION=VALUE`
syntax:

- `ACE_CC` - specify compiler. Currently, supported is `vc` (VBCC) and
  `m68k-amigaos-gcc` (Bebbo's GCC).
- `TARGET` - enable or disable ACE's debug features. Set to `debug` or `release`.

## Building older versions using ACE CLI

@approxit has made his own [simple build tool](https://github.com/approxit/ace-cli/).
To use it, you must have Python installed.
