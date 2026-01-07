import os
import sys
import json
import argparse
import subprocess
from pathlib import Path
import time

TEST_CONFIGS: dict = {}

def _expand_sources(base: str, source_patterns: list) -> list:
    source_files = []
    for pattern in source_patterns:
        matched_files = list(Path(base).glob(pattern))
        if not matched_files:
            print(f"Warning: No files found for pattern {pattern}", file=sys.stderr)
            
        source_files.extend(str(f) for f in matched_files)
        
    return source_files

def _build_test(
    base: str, test_name: str, test_config: dict, 
    compiler: str, test_file: str, output_dir: str, extra_flags: list
) -> str | None:
    macros = [f"-D{macro}" for macro in test_config['macros']] + [f"-{flag}" for flag in extra_flags]
    source_files = _expand_sources(base, test_config['sources'])
    print(f"_build_test, macro={macros}, source_files={' '.join(source_files[:3])} ... [{len(source_files) - 4} more files]") 
    
    if not source_files:
        print(f"Error: No source files found for test {test_name}", file=sys.stderr)
        return None
    
    output_file: Path = Path(output_dir) / f"test_{test_name}"
    base_flags: list = [
        f'-I{base}include',
        '-g',
        '-Wall',
        '-Wextra',
    ]
    
    command: list = [compiler] + base_flags + macros + [test_file] + source_files + ['-o', str(output_file)]
    print(f"Building test: {test_name}")
    print(f"Output: {output_file}")
    print(f"Macros: {', '.join(test_config['macros'])}")
    print(f"Sources: {len(source_files)} files")
    print(f"Command: {' '.join(command[:5])} ... [{len(command) - 6} more args]")
    print("-" * 50)
    
    try:
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode != 0:
            print(f"Compilation failed for {test_name}:")
            print(result.stderr)
            return None
        else:
            print(f"Successfully built {test_name}")
            print(result.stderr)
            return output_file
    except Exception as e:
        print(f"Error during compilation of {test_name}: {e}")
        return None

def _run_test(binary_path: Path, test_input: Path, debugger: str | None = None, log_dir: Path = Path("tests")) -> bool:
    if not binary_path.exists():
        print(f"Error: Binary {binary_path} not found", file=sys.stderr)
        return False
    
    if not test_input.exists():
        print(f"Error: Test input file {test_input} not found", file=sys.stderr)
        return False
    
    command = [str(binary_path), str(test_input), str(test_input.parent)]
    log_dir.mkdir(exist_ok=True)
    log_file = log_dir / f"{binary_path.stem}.log"
    
    if debugger:
        if not _check_debugger_available(debugger):
            print(f"Error: Debugger {debugger} not available", file=sys.stderr)
            return False
        
        debugger_command = _get_debugger_command(debugger, command)
        print(f"Running with {debugger}: {' '.join(debugger_command)}")
        
        try:
            subprocess.run(debugger_command)
            return True
        except Exception as e:
            print(f"Error running debugger: {e}", file=sys.stderr)
            return False
    else:
        print(f"Running test: {' '.join(command)}")
        try:
            with open(log_file, 'w') as f:
                f.write(f"Command: {' '.join(command)}\n")
                f.write("=" * 80 + "\n")
                f.flush()
                
                start_time = time.perf_counter()
                process = subprocess.Popen(
                    command, 
                    stdout=subprocess.PIPE, 
                    stderr=subprocess.STDOUT,
                    text=True,
                    bufsize=1,
                    universal_newlines=True
                )
                
                for line in process.stdout:
                    f.write(line)
                    f.flush()
                    print(line, end='')
                    
                return_code = process.wait()
                end_time = time.perf_counter()
                elapsed = end_time - start_time

            print("\n" + "=" * 80 + "\n")
            print(f"\nExecution time: {elapsed:.6f} seconds")
            return return_code == 0
        except Exception as e:
            print(f"Error running test: {e}", file=sys.stderr)
            return False

def _check_debugger_available(debugger: str) -> bool:
    try:
        result = subprocess.run([debugger, '--version'], capture_output=True, text=True)
        return result.returncode == 0
    except:
        return False

def _get_debugger_command(debugger: str, command: list[str]) -> list[str]:
    if debugger == 'gdb':
        return ['gdb', '--args'] + command
    elif debugger == 'lldb':
        return ['lldb'] + command
    else:
        return [debugger] + command

def _entry() -> None:
    parser = argparse.ArgumentParser(description='Compiler module testing script')
    parser.add_argument('--module', default="all", help='Module to test (or "all" for all modules)')
    parser.add_argument('--compiler', default='gcc', help='C compiler to use (default: gcc)')
    parser.add_argument('--test-file', default='tester.c', help='Path to the main test file (default: test.c)')
    parser.add_argument('--output-dir', default='./bin', help='Output directory for test binaries (default: ./bin)')
    parser.add_argument('--extra-flags', nargs='*', default=[], help='Extra compiler flags')
    parser.add_argument('--sources', default='misc/paths.json', help='JSON file with test sources')
    parser.add_argument('--base', default='../', help='Compiler root directory')
    parser.add_argument('--run', action='store_true', help='Run the test after compilation')
    parser.add_argument('--test-code', default='dummy_data/simple.cpl', help='Input file for the test (default: test1.cpl)')
    parser.add_argument('--debugger', choices=['gdb', 'lldb'], help='Run test with debugger (implies --run)')
    args = parser.parse_args()
    
    if not os.path.isfile(args.test_file):
        print(f"Error: Test file '{args.test_file}' not found", file=sys.stderr)
        sys.exit(1)
    
    os.makedirs(args.output_dir, exist_ok=True)
    with open(args.sources, 'r') as f:
        TEST_CONFIGS = json.loads(f.read())
        
    if args.module not in TEST_CONFIGS.keys():
        print(f"Unsupported module! {args.module} not in {TEST_CONFIGS.keys()}!")
        exit(1)
        
    if args.module == 'all':
        tests_to_build = TEST_CONFIGS.items()
    else:
        tests_to_build = [(args.module, TEST_CONFIGS[args.module])]
    
    success_count = 0
    total_count = len(tests_to_build)
    built_binaries: list = []
    
    for test_name, test_config in tests_to_build:
        binary_path = _build_test(
            args.base, test_name, test_config, args.compiler, 
            args.test_file, args.output_dir, args.extra_flags
        )
        
        if binary_path:
            success_count += 1
            built_binaries.append((test_name, binary_path))
    
    if args.run and built_binaries:
        print("\n" + "=" * 50)
        print("Running tests...")
        print("=" * 50)
        
        for test_name, binary_path in built_binaries:
            print(f"\nRunning test: {test_name}")
            success = _run_test(binary_path, Path(args.test_code), args.debugger)
            if success:
                print(f"Test {test_name} completed successfully")
            else:
                print(f"Test {test_name} failed")
    
    print("=" * 50)
    print(f"Build summary: {success_count}/{total_count} tests built successfully")
    
    if success_count < total_count:
        sys.exit(1)

if __name__ == '__main__':
    _entry()
