import time
import subprocess
from pathlib import Path

class CCRunner:
    def _check_debugger_available(self, debugger: str) -> bool:
        try:
            result = subprocess.run([debugger, '--version'], capture_output=True, text=True)
            return result.returncode == 0
        except:
            return False

    def _get_debugger_command(self, debugger: str, command: list[str]) -> list[str]:
        if debugger == 'gdb':
            return [ 'gdb', '--args' ] + command
        elif debugger == 'lldb':
            return [ 'lldb' ] + command
        else:
            return [ debugger ] + command
    
    def run(
        self, binary_path: Path, args: list = [], debugger: str | None = None, log_dir: Path = Path("tests")
    ) -> bool:
        if not binary_path.exists():
            print(f"Error: Binary {binary_path} not found")
            return False
        
        command = [ str(binary_path), *args ]
        log_dir.mkdir(exist_ok=True)
        log_file = log_dir / f"{binary_path.stem}.log"
        
        if debugger:
            if not self._check_debugger_available(debugger):
                print(f"Error: Debugger {debugger} not available")
                return False
            
            debugger_command = self._get_debugger_command(debugger, command)
            print(f"Running with {debugger}: {' '.join(debugger_command)}")
            
            try:
                subprocess.run(debugger_command)
                return True
            except Exception as e:
                print(f"Error running debugger: {e}")
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
                print(f"Error running test: {e}")
                return False
            