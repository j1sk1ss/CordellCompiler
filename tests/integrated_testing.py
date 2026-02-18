import os
import sys
import json
import argparse
from pathlib import Path

from misc.runner import CCRunner
from misc.builder import (
    CCBuilder, 
    BuildCompiler, 
    CCBuilderConfig
)

TEST_CONFIGS: dict = {}

def _entry() -> None:
    parser = argparse.ArgumentParser(description='Compiler simple testing script')
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
        builder: CCBuilder = CCBuilder(
            settings=CCBuilderConfig(
                include=args.base,
                target=test_name,
                config=test_config,
                compiler=BuildCompiler(args.compiler)
            )
        )
        
        binary_path = builder.build(args.test_file, args.output_dir, args.extra_flags)
        if binary_path:
            success_count += 1
            built_binaries.append((test_name, binary_path))
    
    if args.run and built_binaries:
        print("\n" + "=" * 50)
        print("Running tests...")
        print("=" * 50)
        
        for test_name, binary_path in built_binaries:
            print(f"\nRunning test: {test_name}")
            runner: CCRunner = CCRunner()
            success = runner.run(
                binary_path=binary_path, 
                args=[ str(Path(args.test_code)), str(Path(args.test_code).parent) ], 
                debugger=args.debugger
            )
            
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
