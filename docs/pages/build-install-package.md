# Build, install, and package

## Build from source

Build an optimized compiler and run it directly from the repository:

```bash
make release
./builds/$(uname -s | tr '[:upper:]' '[:lower:]')-$(uname -m)/cplc --version
```

The development binary automatically finds the `cpllib` submodule directory in the repository. </br>
Clone with submodules, or initialize them after cloning:

```bash
git clone --recurse-submodules https://github.com/j1sk1ss/CordellCompiler.git
make submodules
```

## Build the standard library from the Makefile

The root `Makefile` has a separate target for the CPL runtime library:

```bash
make cpllib
```

This builds the compiler if needed, compiles the implementation files from the standard-library source directory, and writes the static archive here:

```text
builds/<platform>/cpllib/libcpl.a
```

For a release-mode runtime archive, pass the same build settings used by the package target:

```bash
make BUILD=release PRINT_PARSE=0 cpllib
```

`make print-config` shows the resolved library paths and inputs:

```text
CPLLIBDIR
CPLLIB_SRC_DIR
CPLRUNTIMEDIR
CPLLIB_SOURCES
CPLLIB_IMPLS
CPLLIB_ARCHIVE
```

`CPLLIB_SRC_DIR` defaults to the `cpllib` submodule. Sibling checkouts can still be selected explicitly:

```bash
make CPLLIB_SRC_DIR=../cpllib cpllib
```

## Install

Install both `cplc` and its CPL standard library:

```bash
make release
sudo make install PREFIX=/usr/local
```

The default installation layout is:

```text
/usr/local/bin/cplc
/usr/local/share/cpl/include/
/usr/local/share/doc/cpl/
/usr/local/share/bash-completion/completions/cplc
/usr/local/share/zsh/site-functions/_cplc
/usr/local/share/fish/vendor_completions.d/cplc.fish
```

`DESTDIR` is supported for distribution packaging and staged installations:

```bash
make install PREFIX=/usr DESTDIR=/tmp/cpl-package-root
```

The compiler discovers installed headers automatically, so applications can use `#include <stdio_h.cpl>` without passing `-I cpllib`. </br>
See the [`cpllib` reference](cpllib-reference.md) for header groups, containers, and usage examples. </br>
Use `CPL_INCLUDE_PATH` to override the standard-library directory. The `-I` option adds a project include directory without disabling the standard library:

```bash
CPL_INCLUDE_PATH=/opt/cpl/include cplc program.cpl
cplc -I project/include program.cpl
cplc --print-stdlib-path
```

## Shell completion

`make install` installs TAB completion for bash, zsh, and fish. Open a new shell after installation; most distributions load files from the standard completion directories automatically.

For a local checkout without installing, load completion manually:

```bash
# bash
source completions/cplc.bash

# zsh
fpath=("$PWD/completions" $fpath)
autoload -Uz compinit
compinit

# fish
mkdir -p ~/.config/fish/completions
ln -sf "$PWD/completions/cplc.fish" ~/.config/fish/completions/cplc.fish
```

The install paths can be overridden for unusual distributions:

```bash
make install PREFIX=/usr \
  BASH_COMPLETION_DIR=/usr/share/bash-completion/completions \
  ZSH_COMPLETION_DIR=/usr/share/zsh/site-functions \
  FISH_COMPLETION_DIR=/usr/share/fish/vendor_completions.d
```

## Build the VS Code extension package

The Docker targets use `VSCODE_DIR`, which defaults to `vscode`:

```bash
make vscode-docker-package
make VSCODE_DIR=../cordell-vscode vscode-docker-package
```

When `vscode` is a submodule and has not been initialized, the Makefile prints the matching `git submodule update` command instead of failing later inside Docker.

## Create a relocatable package

Create an archive containing the compiler, matching headers, runtime archive, and license:

```bash
make package
```

The archive is written to:

```text
builds/cpl-<version>-<platform>.tar.gz
```

Its `bin/cplc` executable discovers the adjacent `share/cpl/include` directory, so the extracted tree can be moved to another prefix without rebuilding.
