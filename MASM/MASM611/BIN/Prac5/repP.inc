writeDate proc
    push bp
    mov bp, sp
    ;reservamos espacio en el stack para:
    ;[bp - 2] dia
    ;[bp - 4] mes
    ;[bp - 6] anno
    ;[bp - 8] hora
    ;[bp - 10] min
    ;[bp - 12] seg
    sub sp, 12

    mov ah, 2ah  ;conseguir dia, mes, anno
    int 21h

    movzx ax, dl;dl contiene el dia, movzx: move zero-extends
    mov [bp - 2], ax
    movzx ax, dh;dh contiene el mes
    mov [bp - 4], ax
    sub cx, 2000;cx contiene el anno pero es un numero de 16 bits entonces le restamos 2000
    mov [bp - 6], cx

    mov ah, 2Ch
    int 21h
    
    movzx ax, ch ;contiene la hora
    mov [bp - 8], ax
    movzx ax, cl ;contiene el minuto
    mov [bp - 10], ax
    movzx ax, dh ;contiene el segundo
    mov [bp - 12], ax

    ;el dia
    lea bx, decBuffer
    push bx
    mov ax, [bp - 2]
    call byteToDecAscii
    ;Llamando a copy string
    lea si, decBuffer
    inc si
    lea di, repDay
    mov cx, 2
    call copyString


    ;el mes
    lea bx, decBuffer
    push bx
    mov ax, [bp - 4]
    call byteToDecAscii
    ;Llamando a copy string
    lea si, decBuffer
    inc si
    lea di, repMonth
    mov cx, 2
    call copyString


    ;el anno
    lea bx, decBuffer
    push bx
    mov ax, [bp - 6]
    call byteToDecAscii
    ;Llamando a copy string
    lea si, decBuffer
    inc si
    lea di, repYear
    mov cx, 2
    call copyString


    ;hora
    lea bx, decBuffer
    push bx
    mov ax, [bp - 8]
    call byteToDecAscii
    ;Llamando a copy string
    lea si, decBuffer
    inc si
    lea di, repHour
    mov cx, 2
    call copyString


    ;minuto
    lea bx, decBuffer
    push bx
    mov ax, [bp - 10]
    call byteToDecAscii
    ;Llamando a copy string
    lea si, decBuffer
    inc si
    lea di, repMinute
    mov cx, 2
    call copyString


    ;segundo
    lea bx, decBuffer
    push bx
    mov ax, [bp - 12]
    call byteToDecAscii
    ;Llamando a copy string
    lea si, decBuffer
    inc si
    lea di, repSecond
    mov cx, 2
    call copyString

    mov sp, bp
    pop bp
    ret ;sin parametros en stack
writeDate endp

;asume que tiene incluidas las variables de game: board.
;tambien: gameP.inc
;Asume que tiene los macros files

;Vamos a asumir que nunca ha error al abrir el archivo
;locales: [bp - 2] handler
mkRep proc
    push bp
    mov bp, sp
    sub sp, 2

    call writeDate

    call writeFuncs

    int 3

    newFile repFileName
    mov [bp - 2], ax ;ponemos en el stack handler [bp - 2]
    writeContent ax, repSize, repMsg0
    closeFile [bp - 2]

    mov sp, bp;hella safe
    pop bp
    ret ;sin parametros en stack
mkRep endp

writeFuncs proc
    push bp
    mov bp, sp

    mFillString offset repFuncOrg, 73, ' '
    mFillString offset repFuncDer, 73, ' '
    mFillString offset repFuncInt, 73, ' '

    mov al, charTermination
    mov charTermination, ' '

    mFuncToString originalFunc, repFuncOrg
    mFuncToString derivativeFunc, repFuncDer
    mFuncToString integralFunc, repFuncInt

    mov charTermination, al

    mov sp, bp;hella safe
    pop bp
    ret ;sin parametros en stack
writeFuncs endp