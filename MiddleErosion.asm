section	.text
global  MiddleErosion

MiddleErosion:

    ; bajt pierwszy
    ; wczytanie zawartosci 8 bajtow
    mov rax, QWORD [rdi + rdx]

    ; bajt drugi
    ; wczytanie zawartosci 8 bajtow
    mov r8, QWORD [rdi + rcx]


    ; przestawienie bajtow na big endian
    bswap rax
    bswap r8


    ; erozja bajtow
	or rax, r8

	shl r8, 1
	or rax, r8

	shr r8, 2
	or rax, r8


    ; nadpisanie bajtu
    mov r8, QWORD [rsi + rdx]
    bswap r8

    ; suma logiczna bajtow
    or rax, r8

    ; zapis bajtu
    bswap rax
    mov QWORD [rsi + rdx], rax

	ret