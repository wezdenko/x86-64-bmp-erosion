section	.text
global  MiddleErosion

MiddleErosion:

    ; bajt pierwszy
    ; wczytanie zawartosci 8 bajtow
    movbe rax, QWORD [rdi + rdx]

    ; bajt drugi
    ; wczytanie zawartosci 8 bajtow
    movbe r8, QWORD [rdi + rcx]


    ; erozja bajtow
	or rax, r8

	shl r8, 1
	or rax, r8

	shr r8, 2
	or rax, r8


    ; nadpisanie bajtu
    movbe r8, QWORD [rsi + rdx]

    ; suma logiczna bajtow
    or rax, r8

    ; zapis bajtu
    movbe [rsi + rdx], rax

	ret