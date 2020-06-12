section	.text
global  LeftEdgeErosion

LeftEdgeErosion:
	push ebp
	mov	ebp, esp

    ; bajt pierwszy
    ; obliczanie adresu bajtu
    mov	eax, DWORD [ebp+8]
    mov edx, DWORD [ebp+16]

    add eax, edx

    ; wczytanie zawartosci dwoch bajtow
    mov dh, byte [eax]
    mov dl, byte [eax+1]
    push dx

    ; bajt drugi
    ; obliczanie adresu bajtu
    mov	eax, DWORD [ebp+8]
    mov edx, DWORD [ebp+20]

    add eax, edx

    ; wczytanie zawartosci dwoch bajtow
    mov dh, byte [eax]
    mov dl, byte [eax+1]


    pop ax ; pierwszy WORD
    ; dx - drugi WORD

    ; erozja bajtow
	or ax, dx

	shr dx, 1
	or ax, dx

	shl dx, 2
	or ax, dx

    ; obliczenie adresu do zapisu
    mov edx, DWORD [ebp+12]
    mov ecx, DWORD [ebp+16]

    add edx, ecx

    ; suma logiczna bajtow
    mov cl, byte [edx]
    or ah, cl

    ; zapis bajtu
    mov [edx], ah

	pop	ebp
	ret