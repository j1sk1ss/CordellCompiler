export const shared = `<style>
  .csa-drawing text{fill:currentColor;font-family:'IBM Plex Mono',monospace;font-size:18px}
  .csa-drawing .note{font-family:Caveat,'Segoe Print',cursive;font-size:27px}
  .csa-drawing .label{font-size:11px;letter-spacing:1px}.csa-drawing .large{font-size:35px}
  .csa-drawing .line{fill:none;stroke:currentColor;stroke-width:2;stroke-linecap:round;stroke-linejoin:round}
  .csa-drawing .box{fill:#fffdf4;stroke:currentColor;stroke-width:1.8}
  .csa-drawing .danger{stroke:#ce431f;fill:none;stroke-width:2.5}.csa-drawing .red{fill:#b93617}
  .csa-drawing .wash{fill:#f6532714;stroke:none}
</style>`;

export const analyzerDrawings = {
  'null-return': `<svg viewBox="0 0 600 280" role="img" aria-label="foo returns zero; the value flows into pointer a and reaches a null dereference">
    ${shared}
    <text x="32" y="37" class="note" transform="rotate(-3 32 37)">A return value travels.</text>
    <path d="M32 89 200 87 203 166 30 169Z" class="box"/>
    <text x="56" y="120">foo()</text><text x="56" y="150" class="label">RETURNS 0</text>
    <path d="M208 128Q236 111 265 127m-12-9 13 10-14 7" class="line"/>
    <path d="M279 85 385 88 382 169 277 166Z" class="box"/>
    <text x="307" y="119">a</text><text x="299" y="150" class="label red">NULL</text>
    <path d="M392 128Q418 113 445 128m-12-9 13 10-14 7" class="line"/>
    <path d="M457 83 566 87 569 171 455 168Z" class="wash"/>
    <text x="467" y="133">dref a</text>
    <path d="M449 91Q514 60 572 94L574 166Q513 196 449 165Z" class="danger"/>
    <text x="361" y="235" class="note red" transform="rotate(-5 361 235)">nothing to dereference</text>
    <path d="M460 214Q504 209 512 178" class="danger"/>
    <text x="33" y="243" class="label">FUNCTION → POINTER → USE</text>
  </svg>`,
  'buffer-bounds': `<svg viewBox="0 0 600 280" role="img" aria-label="A ten-element buffer has valid indices zero through nine; access at index eleven falls outside it">
    ${shared}
    <text x="31" y="37" class="note" transform="rotate(-3 31 37)">Ten slots. Index eleven.</text>
    <path d="M30 103H430V162H30Z" class="box"/>
    <path d="M70 104v57m40-57v57m40-57v57m40-57v57m40-57v57m40-57v57m40-57v57m40-57v57m40-57v57" class="line"/>
    <text x="43" y="140">0</text><text x="83" y="140">1</text><text x="200" y="140">…</text><text x="403" y="140">9</text>
    <path d="M430 103H550V162H430" class="danger" stroke-dasharray="5 6"/>
    <text x="491" y="140" class="red">11</text>
    <path d="M478 92Q507 73 531 93L532 170Q509 189 480 171Z" class="danger"/>
    <path d="M31 189v10h398v-10" class="line"/>
    <text x="166" y="226" class="label">VALID: [0, 10)</text>
    <text x="400" y="243" class="note red" transform="rotate(-6 400 243)">outside the buffer</text>
  </svg>`,
  'divide-zero': `<svg viewBox="0 0 600 280" role="img" aria-label="Variable a holds zero and flows into the divisor of ten divided by a">
    ${shared}
    <text x="31" y="37" class="note" transform="rotate(-3 31 37)">Look at the denominator.</text>
    <path d="M35 93 213 91 216 170 32 173Z" class="box"/>
    <text x="69" y="141">a = 0</text>
    <path d="M224 132Q272 104 310 131m-12-10 13 11-15 7" class="line"/>
    <text x="358" y="108" class="large">10</text>
    <path d="M336 127h113" class="line"/>
    <text x="368" y="174" class="large red">a</text>
    <path d="M344 143Q384 120 427 143L432 181Q393 205 344 180Z" class="danger"/>
    <text x="440" y="151" class="label red">= 0</text>
    <text x="230" y="240" class="note red" transform="rotate(-4 230 240)">this division cannot work</text>
  </svg>`,
  readonly: `<svg viewBox="0 0 600 280" role="img" aria-label="Read-only variable a is initialized to zero; the later assignment of one violates its read-only contract">
    ${shared}
    <text x="31" y="37" class="note" transform="rotate(-3 31 37)">Declared once. Then overwritten?</text>
    <path d="M45 95 283 93 286 183 42 186Z" class="box"/>
    <text x="64" y="130" class="label">READ-ONLY</text><text x="65" y="163">ro i8 a = 0;</text>
    <path d="M302 141h68m-12-9 13 9-13 9" class="line"/>
    <text x="397" y="151">a = 1;</text>
    <path d="M390 161Q463 149 527 161" class="danger"/>
    <path d="M385 107 538 180M392 183 534 106" class="danger"/>
    <text x="289" y="238" class="note red" transform="rotate(-4 289 238)">that write breaks the contract</text>
  </svg>`
};
