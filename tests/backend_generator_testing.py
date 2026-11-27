"""Backend scaffolding regressions (make backend-generator-test)."""

from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import tempfile
import unittest

from misc.builder import default_compiler


ROOT = Path(__file__).resolve().parents[1]
GENERATOR = ROOT / "tools/generate_backend.py"


class BackendGeneratorTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="cpl-backend-generator-")
        self.addCleanup(temporary.cleanup)
        self.directory = Path(temporary.name)
        self.output = self.directory / "generated"

    def run_generator(self, *options, success=True, output=None):
        command = [sys.executable, str(GENERATOR), "--arch", "aarch64",
                   "--abi", "gnu", "--asm", "gas", "--output-root", str(output or self.output)]
        result = subprocess.run(command + list(options), cwd=self.directory,
                                text=True, capture_output=True)
        if success:
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        else:
            self.assertNotEqual(result.returncode, 0)
            self.assertNotIn("Traceback", result.stderr)
        return result

    def run_checked(self, command):
        result = subprocess.run(command, cwd=self.directory, text=True, capture_output=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return result.stdout

    def test_dry_run_and_complete_tree(self):
        preview = self.run_generator("--dry-run")
        self.assertIn("Planned 13 files", preview.stdout)
        self.assertNotIn(".md", preview.stdout)
        self.assertFalse(self.output.exists())
        self.run_generator()
        expected = {
            "include/asm/aarch64_gnu_gas_asmgen.h",
            "include/lir/selector/aarch64_gnu_gas.h",
            "include/lir/regalloc/aarch64_gnu_precolor.h",
            "include/lir/peephole/aarch64_gnu_gas.h",
            "src/asm/aarch64_gnu_gas/asmgen.c",
            "src/asm/aarch64_gnu_gas/vargen.c",
            "src/lir/selector/aarch64_gnu_gas/inst.c",
            "src/lir/selector/aarch64_gnu_gas/mem.c",
            "src/lir/selector/aarch64_gnu_gas/caller_saving.c",
            "src/lir/selector/aarch64_gnu_gas/float.c",
            "src/lir/selector/aarch64_gnu_gas/misc.c",
            "src/lir/regalloc/gc/aarch64_gnu_precolor.c",
            "src/lir/peephole/aarch64_gnu_gas.c",
        }
        actual = {str(path.relative_to(self.output)) for path in self.output.rglob("*") if path.is_file()}
        self.assertEqual(actual, expected)
        for path in expected:
            self.assertIn(str(self.output / path), preview.stdout)

    def test_conflicts_do_not_write_any_files(self):
        for index, relative in enumerate(("include/lir/peephole/aarch64_gnu_gas.h",
                                          "src/lir/regalloc/gc/aarch64_gnu_precolor.c",
                                          "src/asm/aarch64_gnu_gas",
                                          "src/lir/selector/aarch64_gnu_gas")):
            with self.subTest(path=relative):
                root = self.directory / str(index)
                location = root / relative
                location.parent.mkdir(parents=True, exist_ok=True)
                if relative.endswith((".h", ".c")):
                    location.write_text("keep me\n")
                else:
                    location.mkdir()
                before = set(root.rglob("*"))
                for arguments in ((), ("--dry-run",)):
                    result = self.run_generator(*arguments, success=False, output=root)
                    self.assertIn("refusing to overwrite", result.stderr)
                    self.assertEqual(set(root.rglob("*")), before)
                if location.is_file():
                    self.assertEqual(location.read_text(), "keep me\n")

    def test_invalid_names_and_missing_shared_modules(self):
        for name in ("../escape", "/tmp/escape", "x86-64", "X86", "9arch", "", "a;touch", "арх"):
            with self.subTest(name=name):
                self.run_generator("--arch", name, success=False)
                self.assertFalse(self.output.exists())
        for option in ("--precolor-from", "--peephole-from"):
            with self.subTest(option=option):
                result = self.run_generator(option, "missing", success=False)
                self.assertIn("module is missing", result.stderr)
                self.assertFalse(self.output.exists())

    def test_symlink_and_invalid_parent_do_not_write(self):
        outside = self.directory / "outside"
        outside.mkdir()
        self.output.mkdir()
        (self.output / "src").symlink_to(outside, target_is_directory=True)
        result = self.run_generator(success=False)
        self.assertIn("symlink", result.stderr)
        self.assertEqual(list(outside.iterdir()), [])
        self.assertEqual(list(self.output.iterdir()), [self.output / "src"])
        (self.output / "src").unlink()
        (self.output / "src").write_text("keep me\n")
        result = self.run_generator(success=False)
        self.assertIn("parent is not a directory", result.stderr)
        self.assertFalse((self.output / "include").exists())

    def test_variant_reuses_modules_without_modifying_them(self):
        for relative in ("include/lir/regalloc/x86_64_gnu_precolor.h",
                         "src/lir/regalloc/gc/x86_64_gnu_precolor.c",
                         "include/lir/peephole/x86_64_gnu_nasm.h",
                         "src/lir/peephole/x86_64_gnu_nasm.c"):
            destination = self.output / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / relative, destination)
        shared = {path: path.read_bytes() for path in self.output.rglob("*") if path.is_file()}
        result = self.run_generator("--arch", "x86_64", "--abi", "windows", "--asm", "nasm",
                                    "--precolor-from", "x86_64_gnu",
                                    "--peephole-from", "x86_64_gnu_nasm")
        self.assertIn("Generated 9 files", result.stdout)
        self.assertNotIn(".md", result.stdout)
        for path, contents in shared.items():
            self.assertEqual(path.read_bytes(), contents)
        self.assertFalse((self.output / "src/lir/regalloc/gc/x86_64_windows_precolor.c").exists())
        self.assertFalse((self.output / "src/lir/peephole/x86_64_windows_nasm.c").exists())
        self.assertEqual(list(self.output.rglob("*.md")), [])

    def test_second_dialect_requires_explicit_precolor_sharing(self):
        self.run_generator()
        before = {path: path.read_bytes() for path in self.output.rglob("*") if path.is_file()}
        result = self.run_generator("--asm", "custom", success=False)
        self.assertIn("aarch64_gnu_precolor.h", result.stderr)
        self.assertEqual({path: path.read_bytes() for path in self.output.rglob("*") if path.is_file()}, before)
        self.run_generator("--asm", "custom", "--precolor-from", "aarch64_gnu")
        for path, contents in before.items():
            self.assertEqual(path.read_bytes(), contents)
        self.assertTrue((self.output / "src/lir/peephole/aarch64_gnu_custom.c").is_file())
        self.assertTrue((self.output / "src/lir/selector/aarch64_gnu_custom/inst.c").is_file())

    def test_generated_sources_link_with_pipeline_contracts(self):
        self.run_generator()
        compiler = shlex.split(default_compiler())
        if not shutil.which(compiler[0]):
            self.skipTest(f"compiler not found: {compiler[0]}")
        harness = self.directory / "contracts.c"
        harness.write_text("""
#include <asm/aarch64_gnu_gas_asmgen.h>
#include <lir/selector/aarch64_gnu_gas.h>
#include <lir/regalloc/aarch64_gnu_precolor.h>
#include <lir/peephole/aarch64_gnu_gas.h>
#include <lir/selector/savereg.h>
#include <asm/x86_64_gnu_nasm_asmgen.h>
#include <lir/selector/x86_64_gnu_nasm.h>
#include <lir/regalloc/x86_64_gnu_precolor.h>
#include <lir/peephole/x86_64_gnu_nasm.h>

int main(void) {
    asm_gen_t assembler = { .generator = aarch64_gnu_gas_generate_asm };
    inst_selector_t selector = { .select_instructions = aarch64_gnu_gas_instruction_selection };
    mem_selector_t memory = { .select_memory = aarch64_gnu_gas_memory_selection,
                             .validate_memory = aarch64_gnu_gas_memory_validation };
    register_saver_t saver = { .save_registers = aarch64_gnu_gas_caller_saving };
    peephole_t peephole = { .perform_peephole = aarch64_gnu_gas_peephole_optimization };
    long (*precolor)(lir_registers_t) = aarch64_gnu_precolored_reg_to_color;
    if (!assembler.generator || !selector.select_instructions || !memory.select_memory ||
        !memory.validate_memory || !saver.save_registers || !precolor) return 1;
    return peephole.perform_peephole(NULL);
}
""", encoding="utf-8")
        sources = sorted(self.output.rglob("*.c"))
        executable = self.directory / "contracts"
        flags = ["-std=gnu11", "-Wall", "-Wextra", "-Werror", "-Wmissing-prototypes",
                 f"-I{self.output / 'include'}", f"-I{ROOT / 'include'}"]
        self.run_checked(compiler + flags + [str(harness)] + [str(path) for path in sources] +
                         ["-o", str(executable)])
        self.run_checked([str(executable)])
        if not shutil.which("nm"):
            return
        for index, source in enumerate(sources):
            with self.subTest(source=source):
                output = self.directory / f"disabled-{index}.o"
                self.run_checked(compiler + ["-DCPL_DISABLE_AARCH64_GNU_GAS", "-c", str(source),
                                             "-o", str(output)])
                self.assertNotIn("aarch64_", self.run_checked(["nm", str(output)]))


if __name__ == "__main__":
    unittest.main(verbosity=2)
