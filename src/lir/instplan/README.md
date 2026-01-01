# Target info builder
Command below represents how to use the `build_targinfo.py` script from this directory. Important note here is the base location, where you will execute this script (It should be in the project's root directory).

```bash
python3 build_targinfo.py --lir-types-location <../lir_types.c> --std-location <../std> --builder-location <../build_targinfo.c> --save-location <path> --arch <arch>
```

Example command usage for the Ivy_Bridge's targetinfo file generation:
```bash
python3 src/lir/instplan/build_targinfo.py --lir-types-location src/lir/lir_types.c --std-location . --builder-location src/lir/instplan/build_targinfo.c --save-location src/lir/instplan/ --arch Ivy_Bridge
```

Basically, this script doesn't do anything complex. It is just builds the `build_targinfo.c` code with a special flag, then executes produced binary and then cleans directory.

# Add new target information
To accomplish such a task you will need to modify the `build_targinfo.c` file. It isn't hard, but you will need to obtain / inquier neccesiry information about your acrhitecture. In the nutshell, you will need to find information such as `commutative` (flag), `issue_cost` (euristic value), `latency` (value),  `reads_memory` (flag), `sets_flags` (flag), `throughput` (value), `uses_flags` (flag) and `writes_memory` (flag). </br>
Aforementioned information can be found in related manuals and/or in the internet. Also, you can ignore majority of this flags, and setup only the important one:
- `latency`
