%include "third_party/x86inc/x86inc.asm"

%define stack_size_padded 0

; This file uses SSE
  SECTION_TEXT
  CPU       SSE

; void vector4_add_simd(float* src,
;                       float* dest);
%define SYMBOL vector4_add_simd 
  global mangle(SYMBOL)
  align     function_align

mangle(SYMBOL):
  %assign   stack_offset 0

; Parameters are in the following order:
; 1. src
; 2. dest
PROLOGUE  2, 2, 2, V_SRC, V_DEST
  movaps xmm0, [V_SRCq]
  movaps xmm1, [V_DESTq]
  addps xmm1, xmm0
  movaps [V_DESTq], xmm1
  RET
