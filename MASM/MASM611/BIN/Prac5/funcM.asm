;No se si conviene tener todos los coeficientes como bytes o talvez es mas facil trabajar con 
;bytes o words para usar el fpu facil, si lo alimiamos a 2 igual va ocupar exactamente el mismo espacio como si
;fueran todos words
TERM struct 2
    coefNum sword 0
    coefDem sword 1 
    exp sword 0
TERM ends

FUNC struct 2
    terms TERM { }, { }, { }, { }, { }, { }
FUNC ends

;Se espera que func sea una varible
;Tatra de agregar un termino a la funcion, espera recibir el string ingresado por el usuario, tiene que seguir 
;la siguiente expresion regular: -?[0-9]. Retorn ax 1 si se agrego ax 2 si no se pudo agregar
mFuncTryAddTerm macro func, index, grado, userInput
    pushaButAx

    lea ax, userInput
    push ax
    mov ax, grado
    push ax
    mov ax, index
    push ax
    lea ax, func
    push ax
    call funcTryAddTerm

    popaButAx
endm

;Escribe sobre el buffer el string terminado en $ equivalente a la funcion func
;buffer debe de tener de tamanno 73 para garantizar que no ocurran errores
mFuncToString macro func, buffer
    pusha

    lea ax, buffer
    push ax
    lea ax, func
    push ax
    call funcToString

    popa
endm

mFuncDerivate macro destFunc, sourceFunc
    pusha

    lea ax, sourceFunc
    push ax
    lea ax, destFunc
    push ax
    call funcDerivate

    popa
endm

mFuncIntegrate macro destFunc, sourceFunc
    pusha

    lea ax, sourceFunc
    push ax
    lea ax, destFunc
    push ax
    call funcIntegrate

    popa
endm

;Asume que ya esta puesto el modo video y que en es esta la 
;memoria de video 
mFuncGraph macro func, lowerBound, upperBound
    pusha

    mov ax, upperBound
    push ax
    mov ax, lowerBound
    push ax
    lea ax, func
    push ax
    call funcGraph

    popa
endm

