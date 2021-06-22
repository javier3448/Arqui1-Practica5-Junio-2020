;Parameters:   [bp + 4]  puntero a funcion
;              [bp + 6]  indice de Term
;              [bp + 8]  grado
;              [bp + 10] puntero a string terminado con $
;Returns:      ax 1 si se agrego ax 2 si no se pudo agregar
funcTryAddTerm proc 
    push bp
    mov bp, sp

    mov ax, [bp + 10]
    push ax
    call tryStringToSWord;En dx tengo el resultado si ax es 1
    cmp ax, 0
    je NOT_VALID

    mov bx, [bp + 4];puntero a funcion
    assume bx : ptr FUNC
    mov ax, [bp + 6];
    mov ah, sTERM
    mul ah; ax tendria el indice en bytes de terms
    mov si, ax
    mov [bx].terms[si].coefNum, dx
    mov [bx].terms[si].coefDem, 1
    mov ax, [bp + 8]
    mov [bx].terms[si].exp, ax
    mov ax, 1
    jmp RETURN

NOT_VALID:
    xor ax, ax

RETURN:
    pop bp
    ret 8
funcTryAddTerm endp


;Parameters:   [bp + 4]  puntero a string terminado con $, el string debe cumplir con: [+|-]?digit
;return:       ax: 1 si es un string valido
;              dx: retorna signed word, si no es un string valido retorna basura
;regs:         ax, cx, dx, bx
tryStringToSWord proc
    push bp
    mov bp, sp

    mov bx, [bp + 4]

    S0:
        mov al, [bx]
        inc bx
        cmp al, '-'
        je S1
        cmp al, '+'
        je S2
        cmp al, '0'
        jb NOT_VALID
        cmp al, '9'
        ja NOT_VALID
        ;De aqui en adelante sabemos que solo es un digito, tiene signo adelante
        sub al, 30h
        movsx dx, al
        mov ax, 1;con xor ax, ax e inc sale mas rapido
        jmp RETURN

    S1:
        mov al, [bx]
        inc bx
        cmp al, '0'
        jb NOT_VALID
        cmp al, '9'
        ja NOT_VALID
        ;De aqui en adelante sabemos que es un digito negativo
        sub al, 30h
        neg al
        movsx dx, al
        mov ax, 1;con xor ax, ax e inc sale mas rapido
        jmp RETURN
    
    S2:
        mov al, [bx]
        inc bx
        cmp al, '0'
        jb NOT_VALID
        cmp al, '9'
        ja NOT_VALID
        ;De aqui en adelante sabemos que solo es un digito, tiene signo adelante
        sub al, 30h
        movsx dx, al
        mov ax, 1;con xor ax, ax e inc sale mas rapido
        jmp RETURN
    
    NOT_VALID:
        xor ax, ax

RETURN:
    pop bp
    ret 2
tryStringToSWord endp

;Solo sirve si los atributos de func son numero de 0 a 9
;Parameters:   [bp + 4]  puntero a funcion
;              [bp + 6]  puntero a buffer
;Locales       [bp - 2]  en que termino de la funcion estamos, empezando en 0. [0, 5]
funcToString proc
    push bp
    mov bp, sp
    sub sp, 2

    mov bx, [bp + 4];puntero a funcion
    assume bx : ptr FUNC

    mov di, [bp + 6]

    mov word ptr[bp - 2], lTERMS - 1
    mov cx, lTERMS
    LOOP_I:
        mov ax, [bp - 2]
        mov ah, sTERM
        mul ah ;ax = al * ah
        mov si, ax
        mov ax, [bx].terms[si].coefNum ;vamos a dejar en ax coefnum hasta termina el flujo de POSITIVE o NEGATIVE
        cmp ax, 0
        je CONTINUE
        test ax, ax
        
        js NEGATIVE
        jmp POSITIVE
        POSITIVE:
            mov byte ptr[di], '+'
            inc di
            jmp END_POS_NEG

        NEGATIVE:
            mov byte ptr[di], '-'
            inc di
            neg ax
            jmp END_POS_NEG

        END_POS_NEG:
            mov byte ptr[di], ' '
            inc di
        
        mByteToDecAscii offset decBuffer, al
        
        ;beg: escribir solo los digitos que no son leading 0s del string en decBuffer
        cmp al, 100
        jb ESCRIBIR_DECENAS_N;de dec buffer, esto sirve que no escriba los leading 0s, LSD: least significant digit. (del numerador)
        mov dl, decBuffer[0]
        mov [di], dl
        inc di

        ESCRIBIR_DECENAS_N:
        cmp al, 10
        jb ESCRIBIR_UNIDADES_N
        mov dl, decBuffer[1]
        mov [di], dl 
        inc di

        ESCRIBIR_UNIDADES_N:
        mov dl, decBuffer[2]
        mov [di], dl
        inc di

        ;int 3
        ;La parte del denominador
        mov ax, [bx].terms[si].coefDem
        cmp ax, 1
        je END_DEM
        mov byte ptr[di], '/'
        inc di

        mByteToDecAscii offset decBuffer, al
        cmp al, 100
        jb ESCRIBIR_DECENAS_D;de dec buffer, esto sirve que no escriba los leading 0s, LSD: least significant digit. (del denominador)
        mov dl, decBuffer[0]
        mov [di], dl
        inc di

        ESCRIBIR_DECENAS_D:
        cmp al, 10
        jb ESCRIBIR_UNIDADES_D
        mov dl, decBuffer[1]
        mov [di], dl 
        inc di

        ESCRIBIR_UNIDADES_D:
        mov dl, decBuffer[2]
        mov [di], dl
        inc di

        END_DEM:
        ;int 3
        mov ax, [bx].terms[si].exp
        cmp ax, 0
        je END_EXP
        mov byte ptr[di], '*'
        inc di
        mov byte ptr[di], 'x'
        inc di
        cmp ax, 1
        je END_EXP
        add ax, '0'
        mov byte ptr[di], al
        inc di

        END_EXP:
        mov byte ptr[di], ' '
        inc di

    CONTINUE:
    dec word ptr[bp - 2]
    dec cx ;Estas dos lineas reemplazan loop LOOP_I, porque loop solo 
    jne LOOP_I;puede hacer saltos pequennos y nuestro loop es muy largo
    
    mov ah, charTermination
    mov byte ptr[di], ah

RETURN:
    mov sp, bp
    pop bp
    ret 4
funcToString endp

;Parameters:   [bp + 4]  puntero a funcion resultante
;              [bp + 6]  puntero a funcion original
;Locales:      [bp - 2]  en que termino de la funcion estamos, empezando en 0. [0, 5]
funcDerivate proc
    push bp
    mov bp, sp
    sub sp, 2

    mCopyString [bp + 6], [bp + 4], sizeof FUNC

    mov bx, [bp + 4]
    assume bx : ptr FUNC

    mov word ptr[bp - 2], 0
    mov cx, lTERMS
    LOOP_I:
        mov ax, [bp - 2]
        mov ah, sTERM
        mul ah ;ax = al * ah
        mov si, ax
        cmp [bx].terms[si].coefNum, 0
        je CONTINUE

        mov ax, [bx].terms[si].exp
        cmp ax, 0
        je EXP_0

        mov ah, byte ptr[bx].terms[si].coefNum
        imul ah ;en al ya tenemo el exp en al, ax = al * ah = coefNum * exp
        mov [bx].terms[si].coefNum, ax

        dec [bx].terms[si].exp
        jmp CONTINUE

        EXP_0:;Ponemos en 0 todo el coeficiente
        mov [bx].terms[si].coefDem, 1
        mov [bx].terms[si].coefNum, 0
        mov [bx].terms[si].exp, 0

    CONTINUE:
    inc word ptr[bp - 2]
    dec cx ;Estas dos lineas reemplazan loop LOOP_I, porque loop solo 
    jne LOOP_I;puede hacer saltos pequennos y nuestro loop es muy largo

    mov sp, bp
    pop bp
    ret 4
funcDerivate endp


;Parameters:   [bp + 4]  puntero a funcion resultante
;              [bp + 6]  puntero a funcion original
;Locales:      [bp - 2]  en que termino de la funcion estamos, empezando en 0. [0, 5]
funcIntegrate proc
    push bp
    mov bp, sp
    sub sp, 2

    mCopyString [bp + 6], [bp + 4], sizeof FUNC

    mov bx, [bp + 4]
    assume bx : ptr FUNC

    mov word ptr[bp - 2], 0
    mov cx, lTERMS
    LOOP_I:
        mov ax, [bp - 2]
        mov ah, sTERM
        mul ah ;ax = al * ah
        mov si, ax
        cmp [bx].terms[si].coefNum, 0
        je CONTINUE

        ;sumamos 1 al exp
        inc [bx].terms[si].exp
        ;el numerador lo multiplicamos por el exp
        mov ax, [bx].terms[si].coefDem
        mov ah, byte ptr[bx].terms[si].exp
        imul ah
        mov [bx].terms[si].coefDem, ax

    CONTINUE:
    inc word ptr[bp - 2]
    dec cx ;Estas dos lineas reemplazan loop LOOP_I, porque loop solo 
    jne LOOP_I;puede hacer saltos pequennos y nuestro loop es muy largo

    mov sp, bp
    pop bp
    ret 4
funcIntegrate endp

;NO SIRVE DE NADA MAS QUE PARA METER 0 EN EL ACUMULADOR  EL BP - 4 PERO NOS DA MIEDO QUITARLO Y 
;Parameters:   [bp + 4]  puntero a funcion
;              [bp + 6]  lower bound
;              [bp + 8]  upper bound
;Locales:      [bp - 2]  en que termino de la funcion estamos, empezando en 0. [0, 5]
;              [bp - 4]  acumulador
;              [bp - 6]  exponente de la funcion, sirve para hacer el x^n facil
funcGraph proc
    push bp
    mov bp, sp
    sub sp, 6
    mov bx, [bp + 4]
    assume bx : ptr FUNC

    LOOP_I:; en [bp + 6] tenemos el x actual 
        mov word ptr[bp - 4], 0
        fldz ;ponemos el acumlador en 0    
        ;fwait ;No se si al final necesitamos el wait para procesadro 386
        mov word ptr[bp - 2], 0
        mov cx, lTERMS
        LOOP_J:
            mov ax, [bp - 2]
            mov ah, sTERM
            mul ah ;ax = al * ah
            mov si, ax
            cmp [bx].terms[si].coefNum, 0
            je CONTINUE_J
            int 3
            ;hacemos el exponente
            fld1
            mov dx, [bx].terms[si].exp
            mov [bp - 6], dx
            cmp dx, 0
            je BREAK_EXP
            LOOP_EXP:
                fild word ptr[bp + 6];lower bound x
                fmul
                fist deb1
                push ax
                mov ax, deb1
                pop ax
                ;fwait ;No se si al final necesitamos el wait para procesadro 386
            dec word ptr[bp - 6]
            mov dx, [bp - 6]
            cmp dx, 0
            jne LOOP_EXP
            BREAK_EXP:
            ;despues de loop_exp Deveriamos de tener en el top del stack x^n
            fild [bx].terms[si].coefNum
            fild [bx].terms[si].coefDem
            fdiv
            fmul
            fadd
            ;fist deb1 DEBUGGING ONLY. TODO: QUITAR
            ;push ax
            ;mov ax, deb1
            ;pop ax
            ;fwait ;No se si al final necesitamos el wait para procesadro 386

        CONTINUE_J:
            inc word ptr[bp - 2]
        dec cx ;Estas dos lineas reemplazan loop LOOP_I, porque loop solo 
        jne LOOP_J;puede hacer saltos pequennos y nuestro loop es muy largo

    fcom yUpperBound; si 
    fstsw ax
    sahf
    jae OUT_BOUNDS
    fcom yLowerBound
    fstsw ax
    sahf
    jbe OUT_BOUNDS

    IN_BOUNDS:
        fistp word ptr[bp - 4]
        mDrawInCartPlane [bp + 6], [bp - 4]
    jmp END_BOUNDS

    OUT_BOUNDS:
        fistp word ptr[bp - 4]
    jmp END_BOUNDS

    END_BOUNDS:

    CONTINUE_I:
    inc word ptr[bp + 6]
    mov ax, [bp + 6]
    cmp ax, [bp + 8]
    jle LOOP_I ;jump less of equal (signed)

    mov sp, bp
    pop bp
    ret 6
funcGraph endp