// ------------------------------------------------------------
// ARMv8-A separate core program
//
// Description: Test case
// ------------------------------------------------------------

#ifdef TESTOS
#include <target/raven.h>
ENTRY(asm_test_xu_c3023)
#else
    .global asm_test_xu_c3023
    .type asm_test_xu_c3023, "function"
    .cfi_startproc
asm_test_xu_c3023:
#endif
    nop
    mov w9, 0

    // add your core code
    MOV x1, #0xffffffff
    MOV x0, #0x01
    bic w2, w0, w1, lsl #2
    bic w3, w2, w2, lsr #2
    bic w4, w3, w1, asr #2
    bic w5, w3, w1, ror #2
    bic x6, x0, x1, lsl #2
    bic x7, x3, x1, lsr #2
    bic x8, x7, x6, asr #2
    bic x9, x7, x6, ror #2
    bics w10, w9, w9, lsl #2
    bics w11, w0, w1, lsr #2
    bics w12, w9, w10, asr #2
    bics w13, w12, w11, ror #2
    bics x14, x12, x11, lsl #2
    bics x15, x12, x11, lsr #2
    bics x16, x12, x11, asr #2
    bics x17, x16, x16, ror #2
    ldr x18, =0x1
    ldr x19, =0x0
    subs x18, x6, x18
    cbnz x18, _skip_pass
    subs x19, x15, x19
    cbnz x19, _skip_pass
    // end of add your code

    mov w9, 1
_skip_pass:
    // write to address 44'h0FF_7003_FFC0 if your program pass; if fail, write 0
#ifdef TESTOS
    mov w0, w9
#else
#ifdef RTL_SIM
    movk x8, #0xFFC0
    movk x8, #0x7003, lsl #16
    movk x8, #0x00FF, lsl #32
    movk x8, #0x0000, lsl #48
#else
    movk x8, #0xFFC0
    movk x8, #0x8003, lsl #16
    movk x8, #0x0000, lsl #32
    movk x8, #0x0000, lsl #48
#endif
    str x30, [sp, #-0x8]	// x30 is lr
    bl GetCPUID			// 0-47
    add x8, x8, x0
    strb w9, [x8, #0]
    nop
    dsb ish
    ldr x30, [sp, #-0x8]
#endif
    ret
#ifdef TESTOS
ENDPROC(asm_test_xu_c3023)
define_asm_testfn asm_test_xu_c3023 0
#else
    .cfi_endproc
#endif
