; ---------------------------------------------------------------------------
; draw_logo.s
; ---------------------------------------------------------------------------
;
; Draw the GameTank logo

.export  _draw_gametank_logo

; ---------------------------------------------------------------------------
; Wait for interrupt:  Forces the assembler to emit a WAI opcode ($CB)
; ---------------------------------------------------------------------------

.segment  "LOADERS"

.proc _draw_gametank_logo: near
          PHY
          LDY #1
          SEI
          STY $4005
.include "gametank_logo.inc"
           CLI
           PLY
           RTS                    ; Return to caller

.endproc