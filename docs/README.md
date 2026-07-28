<div class="cpl-home">
    <section class="cpl-home-hero">
        <div class="cpl-scroll-monkey" aria-hidden="true">
            <div class="cpl-scroll-monkey-art">
                <img class="cpl-scroll-monkey-base" src="bg/base.png" alt="" />
                <img class="cpl-scroll-monkey-arm" src="bg/arm.png" alt="" />
            </div>
        </div>
        <div>
        <div class="hero-kicker"> Language and compiler documentation </div>
            <h1 class="hero-title">
                <span> Cordell </span>
                <span class="hero-title-mark"> Programming </span>
                <span> Language </span>
            </h1>
            <p class="hero-note"> CPL is a deliberately small systems-language experiment and compiler infrastructure for studying parsing, semantic diagnostics, intermediate representations, optimization, register allocation, assembly generation, symtable optimization, and ML application. </p>
            <div class="hero-actions">
            <a href="#/pages/summary" class="hero-button primary">Overview</a>
            <a href="#/pages/hello-world-example" class="hero-button secondary">Start learning</a>
            <a href="#/pages/compiler-architecture" class="hero-button secondary">Compiler architecture</a>
        </div>
        <div class="cpl-command"><span>$</span><code>git clone https://github.com/j1sk1ss/CordellCompiler.git</code></div>
        </div>
    </section>
    <section class="cpl-home-band">
        <h2> Read this documentation by intent </h2>
        <div class="feature-grid">
            <div class="feature-card">
                <h3> Language </h3>
                <p> The language reference covers types, entry points, control flow, functions, macros, annotations, and low-level facilities such as pointers, syscalls, and inline assembly. </p>
            </div>
            <div class="feature-card">
                <h3> Compiler </h3>
                <p> The compiler documentation explains preprocessing, AST/HIR/LIR stages, CFG and SSA construction, optimization, register allocation, diagnostics, and backend constraints. </p>
            </div>
            <div class="feature-card">
                <h3> Status </h3>
                <p> The status pages state what CPL currently supports, what is intentionally out of scope, and how benchmark and changelog material should be interpreted. </p>
            </div>
        </div>
    </section>
    <section class="cpl-home-band dark">
        <h2> Recommended path </h2>
        <div class="doc-grid">
            <a href="#/pages/summary" class="doc-card">
                <h3> 1. Summary </h3>
                <p> A concise statement of the language, the compiler pipeline, and the intended use of the project. </p>
            </a>
            <a href="#/pages/project-status" class="doc-card">
                <h3> 2. Status and scope </h3>
                <p> Current capabilities, limitations, non-goals, target assumptions, and what remains experimental. </p>
            </a>
            <a href="#/pages/hello-world-example" class="doc-card">
                <h3> 3. First program </h3>
                <p> A minimal entry point, explicit process exit, string output, and entry annotation form. </p>
            </a>
            <a href="#/pages/compiler-architecture" class="doc-card">
                <h3> 4. Compiler architecture </h3>
                <p> How the repository maps to the implementation pipeline and where each compiler stage lives. </p>
            </a>
        </div>
    </section>
    <section class="cpl-home-band">
        <h2> Reference sections </h2>
        <div class="doc-grid">
            <a href="#/pages/types" class="doc-card">
                <h3> Types and memory </h3>
                <p> Primitive values, pointers, strings, arrays, containers, storage classes, and external symbols. </p>
            </a>
            <a href="#/pages/functions-and-inbuilt-macros" class="doc-card">
                <h3> Functions </h3>
                <p> Definitions, prototypes, overloads, generics, function pointers, local functions, lambdas, varargs, syscalls, and assembly. </p>
            </a>
            <a href="#/pages/cpllib-reference" class="doc-card">
                <h3> Standard library </h3>
                <p> How to include <code>cpllib</code> headers, which libc-style declarations are available, and which CPL containers are linked from <code>libcpl.a</code>. </p>
            </a>
            <a href="#/pages/containers" class="doc-card">
                <h3> Containers </h3>
                <p> Value-layout custom types, nested fields, array fields, and explicit self calls. </p>
            </a>
            <a href="#/pages/compiler-usage" class="doc-card">
                <h3> Compiler usage </h3>
                <p> Build commands, target options, output modes, optimization flags, and analysis flags. </p>
            </a>
            <a href="#/pages/semantic-static-checker" class="doc-card">
                <h3> Diagnostics </h3>
                <p> AST-level and IR-level checks, including low-level experiments with symbolic reasoning. </p>
            </a>
            <a href="#/pages/benchmarking" class="doc-card">
                <h3> Evaluation </h3>
                <p> Microbenchmark methodology and current performance measurements against GCC and Clang baselines. </p>
            </a>
            <a href="#/pages/used-links-and-literature" class="doc-card">
                <h3> Literature </h3>
                <p> Compiler texts, SSA references, assembly material, and sources that shaped the implementation. </p>
            </a>
        </div>
    </section>
</div>