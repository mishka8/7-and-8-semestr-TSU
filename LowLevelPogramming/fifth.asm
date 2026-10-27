;задан массив, не содержащий нулевых значений. Найти в нём самую длинную
;знакопеременную подпоследовательность.

;проверка на ноль будет с помощью флага ZF
;проверка на отрицательное число будет с помощью сравнения знака с '-'
;и после установления отрицательного флага SF
;так же проверка на переполнение с помощью флага OF
;числа будем вводить так 0 18 28 221 -12 -123 32 -1

section .text
    global _start


_start:
    xor edi, edi;текущее число
    xor esi, esi;длина массива

    ReadNum:
        mov eax, 3
        xor ebx, ebx
        mov edx, 1
        mov ecx, symbol
        int 0x80

        cmp eax, 0
        jz  LastNum

        cmp byte [symbol], 10
        jz  LastNum

        cmp byte [symbol], ' '
        jz  AddToArr

        cmp byte [symbol], '-'
        jne checkNum
        mov byte [negative], 1
        jmp ReadNum

    checkNum:
        cmp byte [symbol], '0'
        jb  Error
        cmp byte [symbol], '9'
        ja  Error

        movzx eax, byte [symbol]
        imul  edi, edi, 10
        sub   eax, '0'
        add   edi, eax

        jmp ReadNum

    AddToArr:
        cmp edi, 0
        jne SaveNum
        cmp byte [negative], 1
        je  Continue

    SaveNum:
        cmp esi, ARRAY_SIZE
        jae Error

        cmp byte [negative], 1
        jne AddArr
        neg edi

    AddArr:
        mov dword [arr + esi * 4], edi
        inc esi
        xor edi, edi
        mov byte [negative], 0
        jmp ReadNum

    Continue:
        mov byte [negative], 0
        jmp ReadNum

    LastNum:
        cmp edi, 0
        jne SaveLast
        cmp byte [negative], 0
        je  EndArr

        SaveLast:
            cmp esi, ARRAY_SIZE
            jae Error

            cmp byte [negative], 1
            jne AddLastNum
            neg edi

        AddLastNum:
            mov dword [arr + esi * 4], edi
            inc esi
            xor edi, edi
            mov byte [negative], 0

    EndArr:
        mov [arrLen], esi


    main:
        cmp dword[arrLen], 0
        je LoopEnd

        mov dword[bestStart], 0
        mov dword[bestLen], 1
        mov dword[curStart], 0
        mov dword[curLen], 1

        mov eax, [arr]
        test eax, eax
        js FirstNeg
        mov byte[prevSign], 0
        jmp FirstI

    FirstNeg:
        mov byte [prevSign], 1

    FirstI:
        mov ecx, 1

    LoopCheck:
        cmp ecx, [arrLen]
        jae LoopEnd

        mov eax, [arr + ecx * 4]
        test eax, eax
        js CurNeg
        mov byte[curSign], 0
        jmp Compare

    CurNeg:
        mov byte[curSign], 1

    Compare:
        mov al, [curSign]
        cmp al, [prevSign]
        je ResetCur

        inc dword[curLen]

        mov eax, [curLen]
        cmp eax, [bestLen]
        jbe NoUpdateBest
        mov [bestLen], eax
        mov eax, [curStart]
        mov [bestStart], eax

    NoUpdateBest:
        jmp NextIter

    ResetCur:
        mov [curStart], ecx
        mov dword[curLen], 1

    NextIter:
        mov al, [curSign]
        mov [prevSign], al
        inc ecx
        jmp LoopCheck

    LoopEnd:
        cmp dword[bestLen], 0
        je Exit

        mov esi, [bestStart];индекс текущего элемента
        mov edi, [bestLen];длина - сколько нам осталось вывести

    PrintSeq:
        test edi, edi;тут ставятся флаги ZF OF SF
        jz PrintNewLine

        mov eax, [arr + esi * 4]
        test eax, eax
        jns PrintAbs;если SF = 0 то неотрицательное


        mov ebp, eax
        mov eax, 4
        mov ebx, 1
        mov ecx, minus
        mov edx, 1
        int 0x80
        mov eax, ebp
        neg eax
    
    PrintAbs:
        lea ecx, [buffer + BUFFER_SIZE]
        mov ebx, 10

    DivLoop:
        xor edx, edx
        div ebx
        add dl, '0'
        dec ecx
        mov [ecx], dl
        test eax, eax
        jnz DivLoop

        
        mov edx, buffer + BUFFER_SIZE
        sub edx, ecx

        mov eax, 4
        mov ebx, 1
        int 0x80

        ; пробел
        mov eax, 4
        mov ebx, 1
        mov ecx, Space
        mov edx, 1
        int 0x80

        inc esi
        dec edi
        jmp PrintSeq

    PrintNewLine:
        mov eax, 4
        mov ebx, 1
        mov ecx, newline
        mov edx, 1
        int 0x80
        jmp Exit

    Error:
        mov eax, 4
        mov ebx, 1
        mov ecx, errorMsg
        mov edx, errorLen
        int 0x80
        jmp Exit

    Exit:
        mov eax, 1
        xor ebx, ebx
        int 0x80

section .data
    symbol   db 0
    negative db 0
    prevSign db 0
    curSign  db 0

    minus    db '-'
    Space      db ' '
    newline  db 10

    BUFFER_SIZE equ 12
    ARRAY_SIZE equ 1024

    errorMsg db "error", 10
    errorLen equ $ - errorMsg

section .bss
    bestStart resd 1
    bestLen resd 1
    curStart resd 1
    curLen resd 1

    buffer resb 12
    arr    resd 1024
    arrLen resd 1
