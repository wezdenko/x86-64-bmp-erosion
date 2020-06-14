section	.text
global  RightEdgeErosion

RightEdgeErosion:

    dec rdx
    dec rcx

    ; bajt pierwszy
    mov rax, QWORD [rdi + rdx]

    ; bajt drugi
    mov r8, QWORD [rdi + rcx]

    ; bajt do nadpisania
    mov r9, QWORD [rsi + rdx]

    ; przestawienie bajtow na big endian
    bswap rax
    bswap r8

    ; zaslonienie bajtow za krawedzia obrazka
    and rax, 0xFFFFFFFF
    and r8, 0xFFFFFFFF


    ; erozja bajtow
	or rax, r8

	shl r8, 1
	or rax, r8

	shr r8, 2
	or rax, r8


    ; przestawienie bajtow na little endian
    bswap rax

    ; suma logiczna bajtu do nadpisania
    or rax, r9

    ; zapis bajtu
    shr rax, 8
    inc rdx
    mov DWORD [rsi + rdx], eax

	ret