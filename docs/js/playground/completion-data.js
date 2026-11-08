export const cplHeaders = [
  'ctype_h.cpl','errno_h.cpl','fcntl_h.cpl','float_h.cpl','http_h.cpl',
  'inttypes_h.cpl','io_h.cpl','limits_h.cpl','list_h.cpl','locale_h.cpl',
  'math_h.cpl','platform_h.cpl','poll_h.cpl','queue_h.cpl','signal_h.cpl',
  'stack_h.cpl','stdbool_h.cpl','stddef_h.cpl','stdint_h.cpl','stdio_h.cpl',
  'stdlib_h.cpl','string_h.cpl','sys_stat_h.cpl','sys_types_h.cpl',
  'sys_wait_h.cpl','technoblade_h.cpl','time_h.cpl','town_h.cpl',
  'types_h.cpl','unistd_h.cpl'
];

export const cplKeywords = [
  'as','asm','break','case','container','default','dref','else','exit',
  'extern','function','glob','if','lis','loop','not','ptr','ref','return',
  'ro','start','switch','syscall','while'
];

export const cplTypes = [
  'i0','i8','i16','i32','i64','u8','u16','u32','u64','f32','f64',
  'arr','ptr','FILE','http_request','http_response','http_server',
  'linked_list','queue','stack','string'
];

export const cplConstants = [
  'EOF','NULL','O_RDONLY','O_WRONLY','O_RDWR','O_CREAT','O_TRUNC',
  'O_APPEND','SEEK_SET','SEEK_CUR','SEEK_END','STDIN_FILENO',
  'STDOUT_FILENO','STDERR_FILENO','HTTP_MAX_ROUTES','HTTP_REQUEST_BUFFER_SIZE'
];

export const cplStdlibFunctions = [
  { label: 'printf', detail: 'stdio_h.cpl', insertText: 'printf(ref "${1:text}\\n");', documentation: 'Formatted stdout. In the Playground, call fflush(stdout) before exit if you use stdio output.' },
  { label: 'puts', detail: 'stdio_h.cpl', insertText: 'puts(ref "${1:text}");', documentation: 'Writes a string and newline through C stdio.' },
  { label: 'fflush', detail: 'stdio_h.cpl', insertText: 'fflush(${1:stdout});', documentation: 'Flushes a C stdio stream.' },
  { label: 'fopen', detail: 'stdio_h.cpl', insertText: 'fopen(ref "${1:path}", ref "${2:r}")', documentation: 'Open a C FILE stream.' },
  { label: 'fclose', detail: 'stdio_h.cpl', insertText: 'fclose(${1:file});', documentation: 'Close a C FILE stream.' },
  { label: 'strlen', detail: 'string_h.cpl', insertText: 'strlen(${1:text})', documentation: 'Length of a null-terminated string.' },
  { label: 'strcmp', detail: 'string_h.cpl', insertText: 'strcmp(${1:left}, ${2:right})', documentation: 'Compare two null-terminated strings.' },
  { label: 'strcpy', detail: 'string_h.cpl', insertText: 'strcpy(${1:dst}, ${2:src});', documentation: 'Copy a string.' },
  { label: 'strcat', detail: 'string_h.cpl', insertText: 'strcat(${1:dst}, ${2:src});', documentation: 'Append a string.' },
  { label: 'memcpy', detail: 'string_h.cpl', insertText: 'memcpy(${1:dst}, ${2:src}, ${3:n});', documentation: 'Copy bytes.' },
  { label: 'memset', detail: 'string_h.cpl', insertText: 'memset(${1:dst}, ${2:value}, ${3:n});', documentation: 'Fill bytes.' },
  { label: 'malloc', detail: 'stdlib_h.cpl', insertText: 'malloc(${1:size})', documentation: 'Allocate memory.' },
  { label: 'free', detail: 'stdlib_h.cpl', insertText: 'free(${1:pointer});', documentation: 'Free allocated memory.' },
  { label: 'atoi', detail: 'stdlib_h.cpl', insertText: 'atoi(${1:text})', documentation: 'Parse integer text.' },
  { label: 'system', detail: 'stdlib_h.cpl', insertText: 'system(ref "${1:command}")', documentation: 'Run a shell command.' },
  { label: 'read', detail: 'unistd_h.cpl', insertText: 'read(${1:fd}, ${2:buffer}, ${3:size})', documentation: 'Read bytes from a file descriptor.' },
  { label: 'write', detail: 'unistd_h.cpl', insertText: 'write(${1:fd}, ${2:buffer}, ${3:size})', documentation: 'Write bytes to a file descriptor.' },
  { label: 'close', detail: 'unistd_h.cpl', insertText: 'close(${1:fd});', documentation: 'Close a file descriptor.' },
  { label: 'sleep', detail: 'unistd_h.cpl', insertText: 'sleep(${1:seconds});', documentation: 'Sleep for seconds.' },
  { label: 'getpid', detail: 'unistd_h.cpl', insertText: 'getpid()', documentation: 'Return the current process id.' },
  { label: 'open', detail: 'fcntl_h.cpl', insertText: 'open(ref "${1:path}", ${2:O_RDONLY})', documentation: 'Open a file descriptor.' },
  { label: 'creat', detail: 'fcntl_h.cpl', insertText: 'creat(ref "${1:path}", ${2:384})', documentation: 'Create or truncate a file.' }
];

export function createSnippets(monaco) {
  return [
    {
      label: '#include <...>',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: '#include <${1:stdio_h.cpl}>\n$0',
      documentation: 'Include a cpllib header'
    },
    {
      label: 'start',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'start() {\n\t$0\n\texit 0;\n}',
      documentation: 'Program entry point'
    },
    {
      label: 'hello syscall',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'start() {\n\tsyscall(1, 1, ref "${1:Hello, World!}\\n", ${2:14});\n\texit 0;\n}',
      documentation: 'Linux stdout example that works without stdio buffering'
    },
    {
      label: 'hello printf',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: '#include <stdio_h.cpl>\n\nstart() {\n\tprintf(ref "${1:Hello!}\\n");\n\tfflush(stdout);\n\texit 0;\n}',
      documentation: 'stdio example with explicit fflush for Playground output'
    },
    {
      label: 'function',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'function ${1:name}(${2}) -> ${3:i0} {\n\t$0\n}',
      documentation: 'Function declaration'
    },
    {
      label: 'container',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'container ${1:name} {\n\t${2:i64 field};\n}',
      documentation: 'Container declaration'
    },
    {
      label: 'if',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'if ${1:condition}; {\n\t$0\n}',
      documentation: 'If block'
    },
    {
      label: 'if else',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'if ${1:condition}; {\n\t$2\n}\nelse {\n\t$0\n}',
      documentation: 'If/else block'
    },
    {
      label: 'while',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'while ${1:condition}; {\n\t$0\n}',
      documentation: 'While loop'
    },
    {
      label: 'loop',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'loop {\n\t$0\n}',
      documentation: 'Unconditional loop'
    },
    {
      label: 'switch',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: 'switch ${1:value}; {\n\tcase ${2:1}; {\n\t\t$3\n\t\tbreak;\n\t}\n\tdefault {\n\t\t$0\n\t\tbreak;\n\t}\n}',
      documentation: 'CPL switch block'
    },
    {
      label: 'http server',
      kind: monaco.languages.CompletionItemKind.Snippet,
      insertText: '#include <http_h.cpl>\n\n@[section(".bss")]\nglob http_server server;\n\nfunction root(ptr http_request req, ptr http_response res) -> i0 {\n\tres.text(200 as i32, ref "${1:Hello from CPL}\\n");\n}\n\nstart() {\n\tserver.init(ref "127.0.0.1", ${2:8080} as i32);\n\tserver.get(ref "/", root);\n\texit server.listen() as u8;\n}',
      documentation: 'Tiny cpllib/http_h.cpl server'
    }
  ];
}

export const cplAnnotations = [
  { label: '@[entry]', insertText: '@[entry("${1:main}")]', documentation: 'Mark function/start as entry symbol.' },
  { label: '@[section]', insertText: '@[section("${1:.data}")]', documentation: 'Place symbol into a section.' },
  { label: '@[align]', insertText: '@[align(${1:8})]', documentation: 'Request alignment.' },
  { label: '@[inline(always)]', insertText: '@[inline(always)]', documentation: 'Prefer always-inline.' },
  { label: '@[inline(never)]', insertText: '@[inline(never)]', documentation: 'Prevent inlining.' },
  { label: '@[abi]', insertText: '@[abi]', documentation: 'Use ABI-compatible function lowering.' },
  { label: '@[vname]', insertText: '@[vname("${1:symbol}")]', documentation: 'Set backend/linker-visible symbol name.' },
  { label: '@[like_c]', insertText: '@[like_c]', documentation: 'Use C-like container layout.' },
  { label: '@[no_fall]', insertText: '@[no_fall]', documentation: 'Make switch cases not fall through.' },
  { label: '@[select(straight)]', insertText: '@[select(straight)]', documentation: 'Use linear switch generation.' },
  { label: '@[counter]', insertText: '@[counter(${1:10})]', documentation: 'Counted loop annotation.' }
];
