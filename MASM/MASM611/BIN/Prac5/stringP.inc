byteToHexAscii proc
    push bp
    mov bp, sp
    
    mov bx, [bp + 4]
    ;al van a ser los 4 bits mas significativos y dh van aser los 4 bits menos significativos
    mov dh, al
    shr al, 4
    and dh, 00001111b

    ;"if else de los 4 bits mas significativos"
    cmp al, 10
    jb MSD_0to9 ;msd: most significant digit
    add al, 57h
    jmp END_MSD

    MSD_0to9:
    add al, 30h

    END_MSD: 
    mov [bx], al
    ;end if else"

    ;"if else de los 4 bits menos significativos"
    cmp dh, 10
    jb LSD_0to9 ;lsd: least significant digit
    add dh, 57h
    jmp END_LSD

    LSD_0TO9:
    add dh, 30h

    END_LSD:
    mov [bx + 1], dh
    ;end if else

    pop bp
    ret 2
byteToHexAscii endp

byteToDecAscii proc
    push bp
    mov bp, sp

    xor dx, dx ;ponemos dx en 0

    mov di, [bp + 4];chapuz medio, vamos a usar di porque bx lo va a usar div
    mov bx, 100 ;La potencia maxima en decimal que se puede alcanzar con 1 byte
    mov cx, 3 ;maximo numero de digitos decimales

    LOOP_START:
    mov byte ptr[di], "0";Ponemos el caracter del string en ascii 0
    div bx
    add byte ptr[di], al
    
    push dx ;le hacemos backup al residuo porque va a ser el divisor la siguiente iteracion del ciclo
    xor dx, dx
    mov ax, bx
    mov bx, 10
    div bx
    mov bx, ax
    pop ax
    inc di
    loop LOOP_START

    pop bp
    ret 2
byteToDecAscii endp

;Asume que viene algo asi: -?[0-9]+
;Params:  [bp + 4] puntero a inicio de cadena
;         [bp + 6] tamanno de la cadena
;locals:  [bp - 2] isNegative
;         [bp - 4] potencia de 10 
;         [bp - 6] acumulador
;return:  ax sword
;         dx 0 si la cadena no es valida
;            1 si se logro hacer la conversion 
asciiToSWord proc
    push bp
    mov bp, sp
    sub sp, 6

    mov si, [bp + 4];puntero a inicio de cadena
    mov al, [si]
    cmp al, '-'
    je IS_NEGATIVE
    mov word ptr[bp - 2], 0
    jmp END_IS_NEGATIVE

    IS_NEGATIVE:
    mov word ptr[bp - 2], 1
    inc si

    END_IS_NEGATIVE:

    mov bx, [bp + 4]
    add bx, [bp + 6]
    dec bx ;bx apunta al ultimo caracter del string 

    mov word ptr[bp - 4], 1;ponemos la potencia en unidades
    mov word ptr[bp - 6], 0;ponemos el acumulador en 0
    LOOP_I:
        movzx ax, byte ptr[bx]
        cmp ax, '0'
        jb RETURN_ERROR
        cmp ax, '9'
        ja RETURN_ERROR
        sub ax, '0'
        mul word ptr[bp - 4]
        add [bp - 6], ax
    CONTINUE_I:
    mov ax, [bp - 4]
    mov dx, 10
    mul dx
    mov [bp - 4], ax
    dec bx
    cmp bx, si
    jae LOOP_I

    mov dx, 1 ;Ponemos dx en 1 para indicar que no hay error 
    mov ax, [bp - 2];Si no tiene signo podemos retornar tranquilos
    test ax, ax
    jz RETURN

    neg word ptr[bp - 6]
    jmp RETURN

    RETURN_ERROR:
    xor dx, dx

RETURN:
    mov ax, [bp - 6]
    mov sp, bp
    pop bp
    ret 4
asciiToSWord endp


;Parameters:    si     Inicio de cadena (source)
;               di     Inicio de cadena (destination)
;               cx     Tamanno de cadena a copiar
;Registers:     al, si, di, cx, 
;returns:       FLAGS en ZF = 1 si es letra, ZF = 0 si no
copyString proc

    push bp
    mov bp, sp

    LOOP_START:
    mov al, [si]
    mov [di], al
    inc si
    inc di
    loop LOOP_START

    pop bp
    ret
copyString endp

;Parameters:    si     Inicio de cadena (source)
;               al     caracter que se va a poner en cada pos del string
;               cx     Tamanno de cadena a llenar
fillString proc

    push bp
    mov bp, sp

    LOOP_START:
    mov [si], al
    inc si
    loop LOOP_START

    pop bp
    ret

fillString endp