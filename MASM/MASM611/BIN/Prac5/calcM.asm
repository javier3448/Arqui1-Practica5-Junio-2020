
mCalcExec macro path
    pusha

    mGetFileContent path, sourceCode, sizeof sourceCode, sourceCodeLength

    call calcExec

    popa
endm