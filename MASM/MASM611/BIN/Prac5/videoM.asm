mVideoDrawYAxis macro
LOCAL LOOP_I
    push ax
    push di

    mov al, 48
    mov di, 159
    LOOP_I:
        mov es:[di], al
        add di, 320
        cmp di, 320*200
        jb LOOP_I 
    pop di
    pop ax
endm

mVideoDrawXAxis macro
LOCAL LOOP_I
    push ax
    push di

    mov al, 48
    mov di, 320*100
    LOOP_I:
        mov es:[di], al
        inc di
        cmp di, 320*101
        jb LOOP_I 
    pop di
    pop ax
endm


;Ambos parametros tienen signo 
mDrawInCartPlane macro x, y
    push ax
    push bx
    push dx

    mov ax, y
    push ax
    mov ax, x
    push ax
    call drawInCartPlane

    pop dx
    pop bx
    pop ax
endm