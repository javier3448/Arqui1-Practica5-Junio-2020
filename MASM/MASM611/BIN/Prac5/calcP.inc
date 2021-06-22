;locals:  [bp - 2]: operando 1
;         [bp - 4]: operando 2
calcExec proc
    push bp
    mov bp, sp
    sub sp, 4

    int 3

    mov bx, offset sourceCode
    mov si, bx
    mAsciiPtrToSWord si, 2
    cmp dx, 0
    je LEX_NUM_NO_VAL
    mov [bp - 2], ax
    fild word ptr[bp - 2]

    add si, 3
    mAsciiPtrToSWord si, 2
    cmp dx, 0
    je LEX_NUM_NO_VAL
    mov [bp - 4], ax
    fild word ptr[bp - 4]

    mov al, byte ptr[bx + 2]
    call doOperation
    cmp ax, 0
    je LEX_SIG_NO_VAL

    fist deb1
    push ax
    mov ax, deb1
    int 3
    pop ax

    ;Tratamos de conseguir el primer numero
    add si, 2
    WHILE_TRUE:
        mov di, si
        mov al, byte ptr[si]
        cmp al, 03bh ; es el hex de punto y coma
        je PRINT_RESULT
        add di, 1 ;di tiene la direccion del disco
        mAsciiPtrToSWord di, 2
        cmp dx, 0
        je LEX_NUM_NO_VAL
        mov [bp - 4], ax
        fild word ptr[bp - 4]
        mov al, byte ptr[si]
        call doOperation
        cmp ax, 0
        je LEX_SIG_NO_VAL
        fist deb1
        push ax
        mov ax, deb1
        int 3
        pop ax
    add si, 3
    jmp WHILE_TRUE

    PRINT_RESULT:
    fist deb1
    push ax
    mov ax, deb1
    int 3
    pop ax
    fistp word ptr[bp - 6]
    mov al, byte ptr[deb1]
    mByteToDecAscii offset decBuffer, al
    mCopyString offset decBuffer, offset resultString, 3
    mov bx, offset resultString
    mov byte ptr[bx + 3], '$'
    print resultString
    jmp RETURN


    LEX_NUM_NO_VAL:
        debPrintln<"Se esperaba un numero"> 
        jmp RETURN

    LEX_SIG_NO_VAL:
        debPrintln<"Se esperaba un signo: +, -, /, *"> 
        jmp RETURN

    RETURN:
    mov sp, bp
    pop bp
    ret
calcExec endp

;params: al: character signo
;return  ax: 0 si hay error lexico
doOperation proc
    push bp
    mov bp, sp

    PLUS:
    cmp al, '+'
    jne MINUS
    fadd
    jmp RETURN

    MINUS:
    cmp al, '-'
    jne _MULT
    fsub
    mov ax, 1
    jmp RETURN

    _MULT:
    cmp al, '*'
    jne _DIV
    fmul
    mov ax, 1
    jmp RETURN

    _DIV:
    cmp al, '/'
    jne ERROR
    fdiv
    mov ax, 1
    jmp RETURN

    ERROR:
    xor ax, ax

    RETURN:
    mov sp, bp ;hella safe
    pop bp
    ret
doOperation endp