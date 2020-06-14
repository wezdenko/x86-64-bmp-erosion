section	.text
global  FourBytesErosion

FourBytesErosion:

    ; pierwsza czworka bajtow
    mov eax, DWORD [rdi + rdx]

    ; druga czworka bajtow
    mov r8d, DWORD [rdi + rcx]

    ; bajty do nadpisania
    mov r9d, DWORD [rsi + rdx]


    ; przestawienie bajtow na big endian
    bswap eax
    bswap r8d


    ; erozja bajtow
	or eax, r8d

	shl r8d, 1
	or eax, r8d

	shr r8d, 2
	or eax, r8d


    ; przestawienie bajtow na little endian
    bswap eax

    ; suma logiczna bajtu do nadpisania
    or eax, r9d

    ; zapis 4 bajtow
    mov DWORD [rsi + rdx], eax

	ret