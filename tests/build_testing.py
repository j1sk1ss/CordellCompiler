"""GCC selection and backend build regressions (make build-test)."""
import itertools
import os
from pathlib import Path
import shlex
import subprocess
import tempfile
import unittest
from unittest.mock import patch

from misc.builder import CCBuilder, CCBuilderConfig, default_compiler


ROOT = Path(__file__).resolve().parents[1]
BACKENDS = {
    "x86_64_gnu_nasm": ("x86_64", "linux64"),
    "x86_64_macho_nasm": ("x86_64", "macho64"),
    "i386_gnu_nasm": ("i386", "i386"),
}


class CompilerSelectionTests(unittest.TestCase):
    def test_make_overrides_builtin_cc(self):
        environment = os.environ.copy()
        for name in ("CC", "MAKEFLAGS", "MFLAGS", "MAKEOVERRIDES"):
            environment.pop(name, None)
        for system, expected in (("Darwin", "gcc-14"), ("Linux", "gcc")):
            with self.subTest(system=system):
                result = subprocess.run(
                    ["make", "-s", "print-config", f"UNAME_S={system}", "ENABLE_Z3=0"],
                    cwd=ROOT, env=environment, text=True, capture_output=True)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertIn(f"CC={expected}", result.stdout.splitlines())

    def test_platform_defaults_and_environment(self):
        with patch.dict(os.environ, {}, clear=True):
            for platform, expected in (("darwin", "gcc-14"), ("linux", "gcc")):
                with self.subTest(platform=platform), patch("sys.platform", platform):
                    self.assertEqual(default_compiler(), expected)
            with patch.dict(os.environ, {"CC": "/opt/gcc/bin/gcc-15"}):
                self.assertEqual(default_compiler(), "/opt/gcc/bin/gcc-15")

    def test_explicit_compiler_is_not_silently_replaced(self):
        for compiler in ("gcc-14", "/opt/gcc/bin/gcc-15", "ccache gcc-14"):
            with self.subTest(compiler=compiler):
                builder = CCBuilder(CCBuilderConfig("../", "unit", {}, compiler))
                with patch.object(builder, "_expand_sources", return_value=["source.c"]), \
                     patch("misc.builder.subprocess.run") as run:
                    run.return_value.returncode = 0
                    builder.build("test.c", "/tmp", show_logs=False)
                    command = run.call_args.args[0]
                    self.assertEqual(command[:len(shlex.split(compiler))], shlex.split(compiler))


class BackendBuildTests(unittest.TestCase):
    def run_checked(self, command, cwd=ROOT):
        result = subprocess.run(command, cwd=cwd, text=True, capture_output=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return result.stdout

    def test_all_backend_combinations(self):
        with tempfile.TemporaryDirectory(prefix="cpl-backends-") as tmp:
            root = Path(tmp)
            binary = root / "cplc"
            objects = root / "obj"
            source = root / "main.cpl"
            source.write_text("start() { i32 value = 1; value = value + 2; }\n")
            make = ["make", "--no-print-directory", "-j4", "all",
                    f"CC={default_compiler()}", f"OUTPUT={binary}", f"OBJDIR={objects}",
                    "ENABLE_Z3=0", "PRINT_PARSE=0"]
            combinations = list(itertools.product((0, 1), repeat=3))
            # Return to the original configuration without cleaning its objects.
            combinations.append((0, 0, 0))
            for flags in combinations:
                with self.subTest(disabled=flags):
                    switches = [f"CPL_DISABLE_{name.upper()}={flag}"
                                for name, flag in zip(BACKENDS, flags)]
                    self.run_checked(make + switches)
                    symbols = self.run_checked(["nm", str(binary)])
                    for (name, (arch, system)), disabled in zip(BACKENDS.items(), flags):
                        symbol = name + "_generate_asm"
                        self.assertEqual(symbol in symbols, not disabled)
                        self.assertEqual(name + "_instruction_selection" in symbols, not disabled)
                        assembly = root / (name + ".s")
                        command = [str(binary), "--arch", arch, "--sys-type", system,
                                   "--no-object", "--emit-asm", "--asm-output", str(assembly),
                                   "-O2", str(source)]
                        result = subprocess.run(command, cwd=root, text=True, capture_output=True)
                        if disabled:
                            self.assertNotEqual(result.returncode, 0)
                            self.assertIn(f"Backend {name} is disabled", result.stderr)
                            self.run_checked([str(binary), "--sys-type", system, "-E", str(source)], root)
                            self.run_checked([str(binary), "--sys-type", system, "--CSA", str(source)], root)
                        else:
                            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                            self.assertTrue(assembly.read_text().strip())
                    # Unchanged flags must not cause a rebuild on the next invocation.
                    timestamp = binary.stat().st_mtime_ns
                    self.run_checked(make + switches)
                    self.assertEqual(binary.stat().st_mtime_ns, timestamp)
                    print(f"Backend configuration {flags}: OK", flush=True)

    def test_direct_macro_guards(self):
        # Custom build systems may still compile every source with -D switches.
        with tempfile.TemporaryDirectory(prefix="cpl-guards-") as tmp:
            for backend in BACKENDS:
                paths = list((ROOT / "src/asm" / backend).glob("*.c"))
                paths += list((ROOT / "src/lir/selector" / backend).glob("*.c"))
                for source in paths:
                    with self.subTest(source=source):
                        output = Path(tmp) / "disabled.o"
                        self.run_checked(shlex.split(default_compiler()) + [
                            f"-DCPL_DISABLE_{backend.upper()}", "-c", str(source), "-o", str(output)])
                        self.assertNotIn(backend + "_", self.run_checked(["nm", str(output)]))


if __name__ == "__main__":
    unittest.main(verbosity=2)
