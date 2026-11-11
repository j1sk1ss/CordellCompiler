import { shared } from './analyzer-drawings.js';

const colors = `<style>
  .csa-drawing .ok{fill:#32634a}.csa-drawing .ok-line{fill:none;stroke:#32634a;stroke-width:2}
  .csa-drawing .ghost{opacity:.35}
</style>`;

function divisionDrawing(guarded) {
  return `<svg viewBox="0 0 600 330" role="img" aria-label="${guarded ? 'The unguarded call is removed; the remaining call reaches division only after a non-zero check' : 'One unguarded call can pass zero; a second call is restricted by a non-zero guard'}">
    ${shared}${colors}
    <text x="27" y="36" class="note">Same function. Different constraints.</text>
    <path d="M29 116 151 114 154 195 27 198Z" class="box"/>
    <text x="63" y="148">a</text><text x="44" y="177" class="label">UNKNOWN</text>
    <g ${guarded ? 'class="ghost"' : ''}>
      <path d="M157 136Q201 72 244 88m-12-9 13 10-14 7" class="line"/>
      <path d="M253 63 402 65 403 119 251 117Z" class="box"/>
      <text x="278" y="98">foo(a)</text>
      <text x="266" y="52" class="label red">${guarded ? 'CALL REMOVED' : 'ZERO IS POSSIBLE'}</text>
      ${guarded ? '<path d="m254 64 149 55m-149-2 147-54" class="danger"/>' : ''}
    </g>
    <path d="M157 176Q196 222 242 220m-12-9 13 10-14 7" class="ok-line"/>
    <path d="M251 181 404 183 405 240 249 238Z" class="box"/>
    <text x="271" y="218">a != 0</text>
    <text x="266" y="265" class="label ok">GUARDED CALL</text>
    <path d="M408 208Q476 212 501 173" class="ok-line"/>
    ${guarded ? '' : '<path d="M408 91Q475 88 503 129" class="danger"/>'}
    <text x="452" y="158">100 / a</text>
    <text x="103" y="311" class="note ${guarded ? 'ok' : 'red'}">${guarded ? 'zero cannot reach the remaining call' : 'one unchecked caller is enough'}</text>
  </svg>`;
}

export const advancedAnalyzerDrawings = {
  'call-traces': `<svg viewBox="0 0 600 330" role="img" aria-label="Two callers reach bar: one passes NULL, the other a non-NULL value. Their shared dereference is a possible NULL error.">
    ${shared}${colors}
    <text x="27" y="36" class="note">One callee. Two callers.</text>
    <path d="M28 80 218 78 221 142 26 145Z" class="box"/>
    <text x="49" y="108">bar(0)</text><text x="49" y="131" class="label red">NULL · LINE 11</text>
    <path d="M28 188 219 186 222 253 26 256Z" class="box"/>
    <text x="49" y="217">bar(1)</text><text x="49" y="242" class="label ok">NON-NULL · LINE 12</text>
    <path d="M225 111Q283 112 306 149" class="danger"/>
    <path d="M226 220Q282 217 306 181" class="ok-line"/>
    <path d="M315 117 552 115 555 214 312 217Z" class="box"/>
    <text x="340" y="148">bar(msg)</text>
    <text x="340" y="186">dref msg</text>
    <path d="M328 196Q438 189 538 197" class="danger"/>
    <text x="283" y="280" class="note red">possible, not unconditional</text>
    <path d="M456 252Q487 239 491 214" class="danger"/>
    <text x="28" y="309" class="label">CALL SITES → ARGUMENT VALUE → DEREFERENCE</text>
  </svg>`,
  'guarded-division': divisionDrawing(false),
  'guarded-division-fixed': divisionDrawing(true),
  'symbolic-bounds': `<svg viewBox="0 0 600 330" role="img" aria-label="argc is an external input without a proven range. The first buffer access happens before the later range check.">
    ${shared}${colors}
    <text x="27" y="36" class="note">The bad index is not a literal.</text>
    <path d="M29 68 199 66 202 123 27 126Z" class="box"/>
    <text x="55" y="102">argc</text><text x="113" y="102" class="label">INPUT</text>
    <path d="M210 98H317m-13-9 14 9-14 9" class="line"/>
    <text x="332" y="105">_buffer[argc]</text>
    <path d="M329 116 552 114" class="danger"/>
    <path d="M134 179H432V228H134Z" fill="#dfead2"/>
    <path d="M32 205H560m-12-8 13 8-13 8M134 188v34m298-34v34" class="line"/>
    <path d="M33 205H125m315 0h105" class="danger" stroke-dasharray="5 6"/>
    <text x="126" y="251">0</text><text x="419" y="251">10</text>
    <text x="216" y="209" class="label ok">VALID RANGE</text>
    <text x="43" y="302" class="note red">a later check cannot protect an earlier read</text>
  </svg>`,
  'path-exclusion': `<svg viewBox="0 0 600 330" role="img" aria-label="Values between eleven and twenty return in the first branch. On the remaining path a is at most ten or above twenty, so a later equality to fifteen cannot hold.">
    ${shared}${colors}
    <text x="27" y="35" class="note">The first return changes what is possible.</text>
    <path d="M26 63 264 61 267 120 24 123Z" class="box"/>
    <text x="44" y="99">10 &lt; a &amp;&amp; a &lt;= 20</text>
    <path d="M272 92h100m-13-8 14 8-14 8" class="line"/>
    <text x="293" y="75" class="label">TRUE</text><text x="385" y="99">return 1</text>
    <path d="M144 128v52m-8-12 8 13 8-13" class="line"/>
    <text x="157" y="157" class="label">FALSE</text>
    <path d="M27 190 264 188 267 245 24 248Z" class="box"/>
    <text x="94" y="224">a == 15</text>
    <path d="M273 218h100" class="danger" stroke-dasharray="5 6"/>
    <path d="m308 205 26 26m-26 0 26-26" class="danger"/>
    <text x="383" y="225" class="red">return 10</text>
    <text x="333" y="262" class="note red">unreachable</text>
    <text x="26" y="305" class="label">SURVIVING PATH: a &lt;= 10 || a &gt; 20</text>
  </svg>`
};
