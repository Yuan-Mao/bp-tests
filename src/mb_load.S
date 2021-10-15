.file  "mb_load.s"
.option nopic

.data
global_data: .zero 8
.global global_data

.text
.align 1
.globl main
.type  main, @function

main:
  li a0, 0 # return value
  la a2, global_data
  ld a3, (a2)
  ret
