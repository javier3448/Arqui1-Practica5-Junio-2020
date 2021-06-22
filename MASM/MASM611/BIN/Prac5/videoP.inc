;Parameters:   [bp + 4]  x
;              [bp + 6]  y
drawInCartPlane proc
    push bp
    mov bp, sp

    mov ax, [bp + 6]
    neg ax
    add ax, 100
    mov dx, 320
    imul dx

    mov dx, [bp + 4]
    add dx, 159
    add ax, dx

    mov bx, ax

    cmp bx, 320*200
    jae RETURN
    mov byte ptr es:[bx], 31

RETURN:
    pop bp
    ret 4
drawInCartPlane endp