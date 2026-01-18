import subprocess

from enum import Enum
from pathlib import Path
from dataclasses import dataclass

class BuildCompiler(Enum):
    GCC14 = "gcc14"
    GCC = "gcc"

@dataclass
class CCBuilderConfig:
    include: str
    target: str
    config: dict
    compiler: BuildCompiler

class CCBuilder:
    """ CordellCompiler builder is the main builder class
    for a binary file building.
    """
    def __init__(self, settings: CCBuilderConfig) -> None:
        self.settings: CCBuilderConfig = settings
    
    def _expand_sources(self, source_patterns: list) -> list:
        """Collect all files from the provided paths. 

        Args:
            source_patterns (list): The list of path to directories or files.

        Returns:
            list: Expanded list with all essential files.
        """
        source_files = []
        for pattern in source_patterns:
            matched_files = list(Path(self.settings.include).glob(pattern))
            if not matched_files:
                print(f"Warning: No files found for pattern {pattern}")
                
            source_files.extend(str(f) for f in matched_files)
            
        return source_files

    def build(
        self, test_file: str, output_dir: str, extra_flags: list = []
    ) -> str | None:
        macros = [ f"-D{macro}" for macro in self.settings.config.get('macros', []) ] + [ f"-{flag}" for flag in extra_flags ]
        source_files = self._expand_sources(self.settings.config.get('sources', []))
        print(f"CCBuilder.build, macro={macros}, source_files={' '.join(source_files[:3])} ... [{len(source_files) - 4} more files]") 
        
        if not source_files:
            print(f"Error: No source files found for test {self.settings.target}")
            return None
        
        output_file: Path = Path(output_dir) / f"test_{self.settings.target}"
        base_flags: list = [
            f'-I{self.settings.include}include',
            '-g',
            '-Wall',
            '-Wextra',
        ]
        
        print(f"Building test: {self.settings.target}")
        print(f"Output: {output_file}")
        print(f"Macros: {', '.join(self.settings.config.get('macros', []))}")
        print(f"Sources: {len(source_files)} files")
        
        command: list = [ self.settings.compiler.value ] + base_flags + macros + [ test_file ] + source_files + [ '-o', str(output_file) ]
        print(f"Command: {' '.join(command[:5])} ... [{len(command) - 6} more args]")
        print("-" * 50)
        
        try:
            result = subprocess.run(command, capture_output=True, text=True)
            if result.returncode != 0:
                print(f"Compilation failed for {self.settings.target}:")
                print(result.stderr)
                return None
            else:
                print(f"Successfully built {self.settings.target}")
                print(result.stderr)
                return output_file
        except Exception as e:
            print(f"Error during compilation of {self.settings.target}: {e}")
            return None
        