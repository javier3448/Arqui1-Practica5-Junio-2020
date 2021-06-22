;INTERACTUAR CON CONSOLA
print macro msg
    mov ah, 09h
    lea dx, msg
    int 21h
endm

;necesita que newline este definido
;Creo que se rompio? no se porque???
println macro msg ;No se que conviene mas si hacer una copia del string y ponerle el salto de linea al principio o hacer dos llamadas a DOS
    print newline
    print msg
endm

;Returns:     ax = read caracter
cmdGetChar macro
    mov ax, 0100h
    int 21h
endm

cmdGetString macro buffer, stringTermination
local INICIO
INICIO:
    lea bx, buffer
    push bx
    mov dl, stringTermination
    call pCmdGetString
endm

;ARCHIVOS

;MACROS PARA DEBUGGING asume que tiene procesos pCmd.inc
debPrintln macro texto
LOCAL cadena
    .data
        cadena byte texto, 0ah, 0dh, '$'
    .code
        print cadena
endm

pushaButAx macro
    push bx
    push cx
    push dx
    push bp
    push sp
    push si
    push di
endm

popaButAx macro
    pop di
    pop si
    pop sp
    pop bp
    pop dx
    pop cx
    pop bx
endm

pushaButBx macro
    push ax
    push cx
    push dx
    push bp
    push sp
    push si
    push di
endm

popaButBx macro
    pop di
    pop si
    pop sp
    pop bp
    pop dx
    pop cx
    pop ax
endm

pushaButAxAndDx macro
    push bx
    push cx
    push bp
    push sp
    push si
    push di
endm

popaButAxAndDx macro
    pop di
    pop si
    pop sp
    pop bp
    pop cx
    pop bx
endm

mVideoMode macro
    push ax

    mov ah,00h
    mov al,13h
    int 10h
    mov ax, 0A000h
    mov es, ax

    pop ax
endm

mTextMode macro
    push ax
    
    mov ah,00h
    mov al,03h
    int 10h

    pop ax
endm