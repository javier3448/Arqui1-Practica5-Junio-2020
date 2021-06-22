;Cambia decBuffer para que contenga el numero en decimal
;[!!!] No rerecive variable decBuffer debe ser una direccion
mByteToDecAscii macro decBuffer, _byte
    pusha

    mov bx, decBuffer
    push bx
    mov al, _byte
    call byteToDecAscii

    popa
endm

;reg save rappers de nmonicos de string
mCopyString macro source, destination, _size
    pusha

    mov si, source
    mov di, destination
    mov cx, _size
    call copyString

    popa
endm

;si el string destino va a estar ubicado en un offset de es podemodemos usar los
;del x86
;mov cx, _size
;mov si, source
;mov di, destination
;cld
;rep movsb
;ax: numero resultante,
;dx: muero 
mAsciiToSWord macro string, _size
    pushaButAxAndDx

    mov ax, _size
    push ax
    mov ax, offset string
    push ax
    call asciiToSWord
    
    popaButAxAndDx
endm

;ax: numero resultante,
;dx: muero 
mAsciiPtrToSWord macro string, _size
    pushaButAxAndDx

    mov ax, _size
    push ax
    mov ax, string
    push ax
    call asciiToSWord
    
    popaButAxAndDx
endm

mFillString macro sourceBeg, _size, character
    pusha

    mov si, sourceBeg
    mov cx, _size
    mov al, character
    call fillString

    popa
endm

