include macros.asm
include filesM.asm
include funcM.asm
include stringM.asm
include videoM.asm
include repM.asm
include calcM.asm

.model small, stdcall
.386
.stack 400h

.data
    
    include funcD.asm
    include stringD.asm
    include videoD.asm
    include repD.asm
    include calcD.asm

    deb1 dw -3
    deb2 dw 4

    originalFunc FUNC { }
    funcString db 73 dup('$')
    derivativeFunc FUNC { }
    integralFunc FUNC { }
    hasFunc db 0
    plusConstant db "+ c $"

    newLine db 0ah, 0dh, "$"
    
    menuMsg db 0ah, 0dh,
               "UNIVERSIDAD DE SAN CARLOS DE GUATEMALA", 0ah, 0dh,
               "FACULTAD DE INGENIERIA", 0ah, 0dh,
               "ESCUELA DE CIENCIAS Y SISTEMAS", 0ah, 0dh,
               "ARQUITECTURA DE COMPUTADORES Y ENSAMBLADORES 1 A", 0ah, 0dh,
               "PRIMER SEMESTRE 2020", 0ah, 0dh,
               "JAVIER ANTONIO ALVAREZ GONZALEZ", 0ah, 0dh,
               "201612383", 0ah, 0dh,
               "QUINTA PRACTICA", 0ah, 0dh
    menuMsg1 db 0ah, 0dh,
               "1. Ingresar Funcion f(x)", 0ah, 0dh,
               "2. Funcion en Memoria", 0ah, 0dh,
               "3. Derivada f'(x)", 0ah, 0dh,
               "4. Integral F(x)", 0ah, 0dh,
               "5. Graficar Funciones", 0ah, 0dh,
               "6. Reporte", 0ah, 0dh,
               "7. Modo Calculadora", 0ah, 0dh,
               "8. Salir", 0ah, 0dh, "$"

    graphMenuMsg db 0ah, 0dh,
               "1. Graficar Original f(x)", 0ah, 0dh,
               "2. Graficar Derivada f'(x)", 0ah, 0dh,
               "3. Graficar Integral", 0ah, 0dh,
               "4. regresar", 0ah, 0dh, "$"

    lowerXMsg db 0ah, 0dh, "limite inferior: ", "$"
    upperXMsg db 0ah, 0dh, "limite superior: ", "$"

    nonValLimits db "Los limites no son validos", '$'

    usrInput db 32 dup('$')

    coefDe db "Coeficiente de $"

    noValid db "No valido, intente de nuevo: $"
.code 
    include cmdP.inc
    include stringP.inc
    include funcP.inc
    include videoP.inc
    include repP.inc
    include calcP.inc

main proc far
    mov ax,@DATA 
    mov ds,ax

    MENU:
        print menuMsg
        cmdGetChar

        cmp al, "1"
        je INGRESAR_FUNCTION
        cmp al, "2"
        je FUNCION
        cmp al, "3"
        je DERIVADA
        cmp al, "4"
        je INTEGRAL
        cmp al, "5"
        je GRAFICAR
        cmp al, "6"
        je REPORTE
        cmp al, "7"
        je CALC
        cmp al, "8"
        je SALIR
        cmp al, "9"
        je DEBUG
        cmp al, "q";SOLO PARA DEBUGGING
        je RETURN
        cmp al, "Q";SOLO PARA DEBUGGING
        je RETURN
        cmp al, "g";SOLO PARA DEBUGGING
        je DEBUG
        cmp al, "G";SOLO PARA DEBUGGING
        je DEBUG

        debPrintln <"Input no valido">
        jmp MENU

        INGRESAR_FUNCTION:
            debPrintln <"Ingrese coeficientes">
            cmdGetChar
            X4:
                print coefDe
                debPrintln <"x4: ">
                cmdGetString usrInput, "$"
                mFuncTryAddTerm originalFunc, 4, 4, usrInput ;string con el digito (i.e. -4, 1), 0: posicion en el. En al retorna 0 ni no logro agreagar
                cmp al, 1
                je X3
                print noValid
                print newLine
                jmp X4

            X3:
                print coefDe
                debPrintln <"x3: ">
                cmdGetString usrInput, "$"
                mFuncTryAddTerm originalFunc, 3, 3, usrInput ;string con el digito (i.e. -4, 1), 0: posicion en el. En al retorna 0 ni no logro agreagar
                cmp al, 1
                je X2
                print noValid
                print newLine
                jmp X3

            X2:
                print coefDe
                debPrintln <"x2: ">
                cmdGetString usrInput, "$"
                mFuncTryAddTerm originalFunc, 2, 2, usrInput ;string con el digito (i.e. -4, 1), 0: posicion en el. En al retorna 0 ni no logro agreagar
                cmp al, 1
                je X1
                print noValid
                print newLine
                jmp X2

            X1:
                print coefDe
                debPrintln <"x1: ">
                cmdGetString usrInput, "$"
                mFuncTryAddTerm originalFunc, 1, 1, usrInput ;string con el digito (i.e. -4, 1), 0: posicion en el. En al retorna 0 ni no logro agreagar
                cmp al, 1
                je X0
                print noValid
                print newLine
                jmp X1

            X0:
                print coefDe
                debPrintln <"x0: ">
                cmdGetString usrInput, "$"
                mFuncTryAddTerm originalFunc, 0, 0, usrInput ;string con el digito (i.e. -4, 1), 0: posicion en el. En al retorna 0 ni no logro agreagar
                cmp al, 1
                je END_INGRESAR_FUNCTION
                print noValid
                print newLine
                jmp X0
                
            END_INGRESAR_FUNCTION:
            
            ;De una vez le sacamos la derivada y la integral
            mFuncDerivate derivativeFunc, originalFunc
            mFuncIntegrate integralFunc, originalFunc
            mov hasFunc, 1
            debPrintln <"Se ha ingresado la funcion exitosamente">
            cmdGetChar

        jmp MENU

        SIN_FUNCION:
            debPrintln <"No se ha cargado ninguna funcion">
            cmdGetChar
        jmp MENU

        FUNCION:
            cmp hasFunc, 0
            je SIN_FUNCION
            mFuncToString originalFunc, funcString
            println funcString
            print newLine
            cmdGetChar
        jmp MENU

        DERIVADA:
            cmp hasFunc, 0
            je SIN_FUNCION
            mFuncToString derivativeFunc, funcString
            println funcString
            print newLine
            cmdGetChar
        jmp MENU

        INTEGRAL:
            cmp hasFunc, 0
            je SIN_FUNCION
            mFuncToString integralFunc, funcString
            print newLine
            print funcString
            print plusConstant
            print newLine
            cmdGetChar
        jmp MENU

        GRAFICAR:
            cmp hasFunc, 0
            je SIN_FUNCION
            print graphMenuMsg
            cmdGetChar
            cmp al, '1'
            je G_ORIGINAL
            cmp al, '2'
            je G_DERIVADA
            cmp al, '3'
            je G_INTEGRAL
            cmp al, '4'
            je MENU

            G_ORIGINAL:
                print lowerXMsg
                cmdGetString usrInput, '$'
                mAsciiToSword usrInput, ax
                int 3
                cmp dx, 0
                je ORIG_NO_VAL
                mov bx, ax
            push bx
                print upperXMsg
                cmdGetString usrInput, '$'
                mAsciiToSword usrInput, ax
            pop bx
                int 3
                cmp dx, 0
                je ORIG_NO_VAL
                cmp bx, ax
                jge ORIG_NO_VAL

                
                mVideoMode
                mVideoDrawXAxis
                mVideoDrawYAxis

                mFuncGraph originalFunc, bx, ax

                cmdGetChar
                mTextMode
                jmp MENU

                ORIG_NO_VAL:
                println nonValLimits
                cmdGetChar
                jmp G_ORIGINAL

            G_DERIVADA:
                print lowerXMsg
                cmdGetString usrInput, '$'
                mAsciiToSword usrInput, ax
                cmp dx, 0
                je DERI_NO_VAL
                mov bx, ax
            push bx
                print upperXMsg
                cmdGetString usrInput, '$'
                mAsciiToSword usrInput, ax
            pop bx
                cmp dx, 0
                je DERI_NO_VAL
                cmp bx, ax
                jge DERI_NO_VAL

                mVideoMode
                mVideoDrawXAxis
                mVideoDrawYAxis

                mFuncGraph derivativeFunc, bx, ax

                cmdGetChar
                mTextMode
                jmp MENU

                DERI_NO_VAL:
                println nonValLimits
                cmdGetChar
                jmp G_DERIVADA

            G_INTEGRAL:
                print lowerXMsg
                cmdGetString usrInput, '$'
                mAsciiToSword usrInput, ax
                cmp dx, 0
                je INTE_NO_VAL
                mov bx, ax
            push bx
                print upperXMsg
                cmdGetString usrInput, '$'
                mAsciiToSword usrInput, ax
            pop bx
                cmp dx, 0
                je INTE_NO_VAL
                cmp bx, ax
                jge INTE_NO_VAL

                mVideoMode
                mVideoDrawXAxis
                mVideoDrawYAxis

                mFuncGraph integralFunc, bx, ax

                cmdGetChar
                mTextMode
                jmp MENU

                INTE_NO_VAL:
                println nonValLimits
                cmdGetChar
                jmp G_INTEGRAL

        REPORTE:
            debPrintln <"REPORTE">
            int 3
            mMkRep
            int 3
            debPrintln <"Archivo reporte generado">
            cmdGetChar
        jmp MENU

        CALC:
            debPrintln <"CALC">
            mFillString offset sourceCode, sizeof sourceCode, 0
            debPrintln <"nombre del archivo: ">
            cmdGetString usrInput, 0
            int 3
            mGetFileContent usrInput, sourceCode, sizeof sourceCode, ax
            jc FILE_NO_VALIDO
            lea bx, sourceCode
            int 3
            call calcExec
            cmdGetChar
            int 3

        jmp MENU

        FILE_NO_VALIDO:
        debPrintln <"NO SE PUEDE LEER EL ARCHIVO">
        cmdGetChar
        jmp CALC

        SALIR:
            debPrintln <"SALIR">
            cmdGetChar
        jmp RETURN

        DEBUG:
            debPrintln <"DEBUG">

            mov originalFunc.terms[0].coefNum, 0
            mov originalFunc.terms[0].coefDem, 1
            mov originalFunc.terms[0].exp, 0

            lea bx, originalFunc

            mov originalFunc.terms[1 * sizeof TERM].coefNum, -1
            mov originalFunc.terms[1 * sizeof TERM].coefDem, 5
            mov originalFunc.terms[1 * sizeof TERM].exp, 1

            mov originalFunc.terms[2 * sizeof TERM].coefNum, 1
            mov originalFunc.terms[2 * sizeof TERM].coefDem, 100
            mov originalFunc.terms[2 * sizeof TERM].exp, 2

            mov originalFunc.terms[3 * sizeof TERM].coefNum, 0
            mov originalFunc.terms[3 * sizeof TERM].coefDem, 1
            mov originalFunc.terms[3 * sizeof TERM].exp, 3

            mov originalFunc.terms[4 * sizeof TERM].coefNum, 0
            mov originalFunc.terms[4 * sizeof TERM].coefDem, 1
            mov originalFunc.terms[4 * sizeof TERM].exp, 4
            int 3
            mFuncToString originalFunc, funcString
            println funcString
            print newLine
            int 3
            mFuncDerivate derivativeFunc, originalFunc
            mFuncToString derivativeFunc, funcString
            println funcString
            print newLine
            int 3
            mFuncIntegrate integralFunc, originalFunc
            mFuncToString integralFunc, funcString
            println funcString
            print newLine
            int 3
            mVideoMode
            mVideoDrawXAxis
            mVideoDrawYAxis

            mFuncGraph originalFunc, -159, 159

            cmdGetChar
            mTextMode
        jmp MENU

    RETURN:
    mov ah,4ch
    int 21h 

main endp 
end main 