    repMsg0 db "UNIVERSIDAD DE SAN CARLOS DE GUATEMALA", 0ah, 0dh,
               "FACULTAD DE INGENIERIA", 0ah, 0dh,
               "CIENCIAS Y SISTEMAS", 0ah, 0dh,
               "ARQUITECTURA DE COMPUTADORES Y ENSAMBLADORES 1 A-", 0ah, 0dh
    repMsg1 db 0ah, 0dh,
             "PRIMER SEMESTRE 2020", 0ah, 0dh, 0ah, 0dh,
             "JAVIER ANTONIO ALAVREZ GONZALEZ", 0ah, 0dh,
             "201612383", 0ah, 0dh,
             0ah, 0dh,
             "REPORTE PRACTICA NO.5", 0ah, 0dh,
             "Fecha: ", 0ah, 0dh
    repDay db "00/"
    repMonth db "00/"
    repYear db "00", 0ah, 0dh
    repHour  db "00:"
    repMinute db "00:"
    repSecond db "00", 0ah, 0dh,
              0ah, 0dh,
              "Funcion Original:", 0ah, 0dh, "f(x): "
    repFuncOrg db 75 dup(' '), 0ah, 0dh
    repMsg2 db "Funcion Derivada:", 0ah, 0dh, "f'(x): "
    repFuncDer db 75 dup(' '), 0ah, 0dh
    repMsg3 db "Funcion Integral:", 0ah, 0dh, "F(x): "
    repFuncInt db 75 dup(' ')
    repMsg4 db "+ c", 0ah, 0dh

    repSize EQU sizeof repMsg0 + sizeof repMsg1 + sizeof repDay + sizeof repMonth + sizeof repYear + sizeof repHour + sizeof repMinute + sizeof repSecond+ sizeof repFuncOrg + sizeof repMsg2 + sizeof repFuncDer + sizeof repMsg3 + sizeof repFuncInt + sizeof repMsg4

    repFileName db "c:\rep.txt", 0