from src.parser import PTRNParser
from src.generator import CodeGenerator

def _generate_from_ptrn(ptrn_source: str) -> str:
    parser: PTRNParser = PTRNParser()
    patterns = parser.parse_patterns(source=ptrn_source)
    generator: CodeGenerator = CodeGenerator(patterns=patterns)
    return generator.generate()

if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        with open(sys.argv[1], 'r') as f:
            ptrn_source = f.read()
    else:
        exit(1)
    
    try:
        c_code = _generate_from_ptrn(ptrn_source)
        with open("peephole_generated.c", "w") as f:
            f.write(c_code)
    except Exception as e:
        import traceback
        print(f"Error: {e}")
        traceback.print_exc()