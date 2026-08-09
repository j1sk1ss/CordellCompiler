import argparse
import os
import platform
import subprocess
import sys
import tempfile
import unittest

from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[1]


def _default_platform_name() -> str:
    return f"{platform.system().lower()}-{platform.machine().lower()}"


def _default_cplc_path() -> Path:
    env_binary = os.environ.get("CPLC_BINARY")
    if env_binary:
        return Path(env_binary)

    return REPO_ROOT / "builds" / _default_platform_name() / "cplc"


def _tmpdir() -> tempfile.TemporaryDirectory:
    base = Path(os.environ.get("CPLC_TEST_TMPDIR", "/tmp"))
    return tempfile.TemporaryDirectory(prefix="cplc-cli-", dir=base)


class BuilderCLITests(unittest.TestCase):
    cplc: Path = _default_cplc_path()

    @classmethod
    def setUpClass(cls) -> None:
        if not cls.cplc.exists():
            raise unittest.SkipTest(
                f"cplc binary is not built: {cls.cplc}. "
                "Run `make all` or pass --cplc /path/to/cplc."
            )

    def run_cplc(
        self,
        *args: object,
        env: dict[str, str] | None = None,
        timeout: int = 10,
    ) -> subprocess.CompletedProcess[str]:
        test_env = os.environ.copy()
        if env:
            test_env.update(env)

        return subprocess.run(
            [str(self.cplc), *[str(arg) for arg in args]],
            cwd=REPO_ROOT,
            env=test_env,
            capture_output=True,
            text=True,
            timeout=timeout,
        )

    def test_help_lists_builder_options(self) -> None:
        result = self.run_cplc("--help")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("Usage: cplc [options] <input files>", result.stdout)
        self.assertIn("General options:", result.stdout)
        self.assertIn("Linker options:", result.stdout)
        self.assertIn("-c, --compile-only", result.stdout)
        self.assertIn("--emit-asm", result.stdout)
        self.assertEqual(result.stderr, "")

    def test_version_prints_single_line(self) -> None:
        result = self.run_cplc("--version")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertRegex(result.stdout, r"^cplc \S+\n$")
        self.assertEqual(result.stderr, "")

    def test_no_args_prints_help_but_exits_with_failure(self) -> None:
        result = self.run_cplc()

        self.assertNotEqual(result.returncode, 0)
        self.assertIn("Usage: cplc [options] <input files>", result.stdout)
        self.assertEqual(result.stderr, "")

    def test_rejects_missing_option_value(self) -> None:
        result = self.run_cplc("--output")

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn("Can't parse input arguments", result.stderr)

    def test_rejects_invalid_define_name(self) -> None:
        result = self.run_cplc("-D", "1BAD=1")

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn("Can't parse input arguments", result.stderr)

    def test_requires_input_files_for_build_actions(self) -> None:
        result = self.run_cplc("-O2")

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn("No input files", result.stderr)

    def test_print_stdlib_path_prefers_environment(self) -> None:
        with _tmpdir() as tmp:
            stdlib = Path(tmp) / "stdlib"
            stdlib.mkdir()

            result = self.run_cplc(
                "--print-stdlib-path",
                env={"CPL_INCLUDE_PATH": str(stdlib)},
            )

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, f"{stdlib}\n")
        self.assertEqual(result.stderr, "")

    def test_preprocess_only_applies_cli_defines(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "defines.cpl"
            source.write_text(
                "\n".join(
                    [
                        "#ifdef ENABLED",
                        "function picked(i32 value = ANSWER);",
                        "#endif",
                        "#ifndef ENABLED",
                        "function skipped();",
                        "#endif",
                        "",
                    ]
                ),
                encoding="utf-8",
            )

            result = self.run_cplc("-E", "-DENABLED", "-DANSWER=42", source)

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("function picked(i32 value = 42);", result.stdout)
        self.assertNotIn("function skipped();", result.stdout)
        self.assertEqual(result.stderr, "")

    def test_preprocess_only_writes_to_output_file(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "input.cpl"
            output = Path(tmp) / "preprocessed.cpl"
            source.write_text("function from_input();\n", encoding="utf-8")

            result = self.run_cplc("-E", "--output", output, source)
            output_text = output.read_text(encoding="utf-8")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, "")
        self.assertIn("function from_input();", output_text)
        self.assertEqual(result.stderr, "")

    def test_include_directory_is_used_by_preprocessor(self) -> None:
        with _tmpdir() as tmp:
            include_dir = Path(tmp) / "include"
            include_dir.mkdir()
            (include_dir / "cli_header.cpl").write_text(
                "function from_header();\n",
                encoding="utf-8",
            )

            source = Path(tmp) / "main.cpl"
            source.write_text(
                "#include <cli_header.cpl>\nfunction from_source();\n",
                encoding="utf-8",
            )

            result = self.run_cplc("-E", "-I", include_dir, source)

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("function from_header();", result.stdout)
        self.assertIn("function from_source();", result.stdout)
        self.assertEqual(result.stderr, "")

    def test_conflicting_preprocess_and_compile_only_modes_are_rejected(self) -> None:
        with _tmpdir() as tmp:
            source = Path(tmp) / "input.cpl"
            source.write_text("function input();\n", encoding="utf-8")

            result = self.run_cplc("-E", "-c", source)

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout, "")
        self.assertIn("Can't parse input arguments", result.stderr)


def _entry() -> None:
    parser = argparse.ArgumentParser(description="Builder CLI tests")
    parser.add_argument(
        "--cplc",
        default=str(_default_cplc_path()),
        help="Path to the cplc binary under test",
    )
    args, unittest_args = parser.parse_known_args()

    BuilderCLITests.cplc = Path(args.cplc).resolve()
    unittest.main(argv=[sys.argv[0], *unittest_args], verbosity=2)


if __name__ == "__main__":
    _entry()
