;задан массив, не содержащий нулевых значений. Найти в нём самую длинную
;знакопеременную подпоследовательность.

;проверка на ноль будет с помощью флага ZF
;проверка на отрицательное число будет с помощью сравнения знака с '-'
;и после установления отрицательного флага SF
;так же проверка на переполнение с помощью флага OF
;числа будем вводить так 0 18 28 221 -12 -123 32 -1
;dword - 4 байта

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
        mov byte [negative], 1;если - щапоминаем знак
        jmp ReadNum

    checkNum:
        cmp byte [symbol], '0'
        jb  Error
        cmp byte [symbol], '9'
        ja  Error

        movzx eax, byte [symbol]
        imul  edi, edi, 10;собираем число с помощью умножения
        sub   eax, '0'
        add   edi, eax

        jmp ReadNum

    AddToArr:
        ;сюда попадаем при пробеле смотрим число и 
        ;если оно не ноль сохраняем в массив
        cmp edi, 0
        jne SaveNum
        cmp byte [negative], 1
        je  Continue

    SaveNum:
        ;проверка на переполнение
        cmp esi, ARRAY_SIZE
        jae Error
        ;если стоял минус то делаем число отрицатешльным с помощью neg
        cmp byte [negative], 1
        jne AddArr
        neg edi

    AddArr:
        ;запись числа в массив елементы по 4 байта 
        mov dword [arr + esi * 4], edi
        inc esi;добавляем размер массива
        xor edi, edi;сбрасываем число которое добавили
        mov byte [negative], 0;и сбрасываем флаг отрицательного числа
        jmp ReadNum

    Continue:
        ;метка пропуска нулей и сброса флага отрицательного числа
        mov byte [negative], 0 
        jmp ReadNum

    LastNum:
        ;тут всё тоже самое что и выше только для последнего числа
        cmp edi, 0
        jne SaveLast
        cmp byte [negative], 0
        je  EndArr

        SaveLast:
            ;проверка на то что место в массиве ещё есть
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
        ;записываем длину массива
        mov [arrLen], esi


    main:
        ;смотрим длину массива если ноль прыгаем в выход
        cmp dword[arrLen], 0
        je LoopEnd

        mov dword[bestStart], 0
        mov dword[bestLen], 0
        mov dword[curStart], 0
        mov dword[curLen], 1

        ;берем первый элемент и устанавливаем флаги ZF SF OF 
        ;с помощью test и проверяем если sf = 1 значит отрицательный
        ;прыгаем в FirstNeg и ставим что prevSign - отриц
        ;иначе prevSign = 0 это положительное число
        mov eax, [arr]
        test eax, eax
        js FirstNeg
        mov byte[prevSign], 0
        jmp FirstI

    FirstNeg:
        mov byte [prevSign], 1

    FirstI:
        ;первый элемент уже учли и поэтому ecx начинается с 1 для обхода по масиву
        mov ecx, 1

    LoopCheck:
        ;условие пока ecx меньше длины массива
        cmp ecx, [arrLen]
        jae LoopEnd

        ;читаем новый елемент с помощью test проверяем по флагам отрицательный или нет
        ;запоминаем знак в CurNeg и дальше сравниваем
        mov eax, [arr + ecx * 4]
        test eax, eax
        js CurNeg
        mov byte[curSign], 0
        jmp Compare

    CurNeg:
        mov byte[curSign], 1

    Compare:
        ;сравниваем текущий и предыдущий знаки
        mov al, [curSign]
        cmp al, [prevSign]
        je ResetCur;если знаки одинаковые то чередования нет идем сбрасывать знаки

        inc dword[curLen];+1 к текущей последовательности

        ;тут сравниваем текущую и лучшую и обновляем если надо
        mov eax, [curLen]
        cmp eax, [bestLen]
        jbe NoUpdateBest
        mov [bestLen], eax
        mov eax, [curStart]
        mov [bestStart], eax

    NoUpdateBest:
        jmp NextIter

    ResetCur:
        ;если знаки не совпали мы обновляем начало последовательности и длину
        mov [curStart], ecx
        mov dword[curLen], 1

    NextIter:
        ;предыдущий знак становится текущим и ecx++
        mov al, [curSign]
        mov [prevSign], al
        inc ecx
        jmp LoopCheck

    LoopEnd:
        cmp dword[bestLen], 0
        je NoSolution

        mov esi, [bestStart];индекс текущего элемента
        mov edi, [bestLen];длина - сколько нам осталось вывести

    PrintSeq:
        ;если длина последовательности ноль то перходим и выводим новую строку и сообщение no solution
        test edi, edi;тут ставятся флаги ZF OF SF
        jz PrintNewLine

        mov eax, [arr + esi * 4]
        test eax, eax;тут проверяем отрицательное или нет
        jns PrintAbs;если SF = 0 то неотрицательное

        ;выводим минус если число отрицательное
        mov ebp, eax
        mov eax, 4
        mov ebx, 1
        mov ecx, minus
        mov edx, 1
        int 0x80
        mov eax, ebp
        neg eax
    
    PrintAbs:
        ;тут выводим число так как у нас либо уже выведен минус или число положительное
        ;ecx указывает на конец буфера
        mov ecx, buffer + BUFFER_SIZE
        mov ebx, 10

    DivLoop:
        ;считаем цифры из числа и записываем справа налево в буфер
        xor edx, edx
        div ebx
        add dl, '0'
        dec ecx
        mov [ecx], dl
        test eax, eax
        jnz DivLoop

        ;кладем в edi адрес конца буфера
        ;edx длина строки цифр в байтах
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

        ;переход к следующему элементу подпоследовательности
        inc esi
        dec edi
        jmp PrintSeq

    PrintNewLine:
        ;выводим '\n'
        mov eax, 4
        mov ebx, 1
        mov ecx, newline
        mov edx, 1
        int 0x80
        jmp Exit
    
    NoSolution:
        mov eax, 4
        mov ebx, 1
        mov ecx, noSolutionMsg
        mov edx, noSolutionLen
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

    noSolutionMsg db "not solution", 10
    noSolutionLen equ $ - noSolutionMsg

section .bss
    bestStart resd 1
    bestLen resd 1
    curStart resd 1
    curLen resd 1

    buffer resb 12
    arr    resd 1024
    arrLen resd 1
