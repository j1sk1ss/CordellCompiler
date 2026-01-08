# Preproc
Pre-process the input file and make sure that it's ready to go further. In the nutshell, this is a regex-based text transformer. It doesn't use a lot of RAM given the creation of a new `tmp` file. 

# Main
This part takes care about keywords such as `file`, `define`, `include`, `ifdef` and `ifndef`. This, actually, is a full copy of the `C/C++` pre-processor. </br>
Main idea is simple:
- Remove comments
- Register all `defines`.
- Replace all `defines`.
- Evaluate all `ifdef`s and `ifndef`s.
- Include `file` directives.

# file directive?
The `file` directive has the next syntax:
```cpl
#file "file.cpl"
```

And marks the start of a new file. After the pre-processor part, we have a united file. This united file, actually, breaks all attempts to determine where was a token before the pre-processor part. For this purpose (preserve the source location in a file) pre-processor includes the `#line <file.cpl>` directive.
