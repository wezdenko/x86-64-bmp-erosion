section	.text
global  MiddleErosion

MiddleErosion:

    dec rdx
    dec rcx

    ; pierwsza osemka bajtow
    mov rax, QWORD [rdi + rdx]

    ; druga osemka bajtow
    mov r8, QWORD [rdi + rcx]

    ; bajty do nadpisania
    mov r9, QWORD [rsi + rdx]


    ; przestawienie bajtow na big endian
    bswap rax
    bswap r8


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

    ; zapis 4 bajtow
    shr rax, 8
    inc rdx
    mov DWORD [rsi + rdx], eax

	ret