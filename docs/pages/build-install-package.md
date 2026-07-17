# Build, install, and package

## Build from source

Build an optimized compiler and run it directly from the repository:

```bash
make release
./builds/$(uname -s | tr '[:upper:]' '[:lower:]')-$(uname -m)/cplc --version
```

The development binary automatically finds the `cpllib` directory in the
repository.

## Build the standard library from the Makefile

The root `Makefile` has a separate target for the CPL runtime library:

```bash
make cpllib
```

This builds the compiler if needed, compiles the implementation files from
`cpllib/*.cpl` that are not headers, and writes the static archive here:

```text
builds/<platform>/cpllib/libcpl.a
```

For a release-mode runtime archive, pass the same build settings used by the
package target:

```bash
make BUILD=release PRINT_PARSE=0 cpllib
```

`make print-config` shows the resolved library paths and inputs:

```text
CPLLIBDIR
CPLRUNTIMEDIR
CPLLIB_IMPLS
CPLLIB_ARCHIVE
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
```

`DESTDIR` is supported for distribution packaging and staged installations:

```bash
make install PREFIX=/usr DESTDIR=/tmp/cpl-package-root
```

The compiler discovers installed headers automatically, so applications can
use `#include <stdio_h.cpl>` without passing `-I cpllib`.

See the [`cpllib` reference](cpllib-reference.md) for header groups,
containers, and usage examples.

Use `CPL_INCLUDE_PATH` to override the standard-library directory. The `-I`
option adds a project include directory without disabling the standard library:

```bash
CPL_INCLUDE_PATH=/opt/cpl/include cplc program.cpl
cplc -I project/include program.cpl
cplc --print-stdlib-path
```

## Create a relocatable package

Create an archive containing the compiler, matching headers, runtime archive,
and license:

```bash
make package
```

The archive is written to:

```text
builds/cpl-<version>-<platform>.tar.gz
```

Its `bin/cplc` executable discovers the adjacent `share/cpl/include` directory,
so the extracted tree can be moved to another prefix without rebuilding.
