;задана целочисленная квадратная матрица n-го порядка.
;определить, является ли она симметричной относительно главной диагонали.

;размер матрицы задаем константой MATRIX_N
;числа вводим одной строкой через пробел, например 1 2 3 4 5 6 7 8 9
;сначала сохраняем числа в массив, потом выводим их как матрицу
;элемент a[i][j] находится по индексу i * MATRIX_N + j
;для симметричности должно выполняться a[i][j] = a[j][i]
;переполнение при вычислениях проверяем через флаг OF и переход jo
;dword - 4 байта

section .text
    global _start

_start:
    xor edi, edi;текущее число
    xor esi, esi;количество введенных элементов

    ReadNum:
        ;читаем строку по одному символу
        mov eax, 3
        xor ebx, ebx
        mov edx, 1
        mov ecx, symbol
        int 0x80

        cmp eax, 0
        jz LastNum

        cmp byte [symbol], 10
        jz LastNum

        cmp byte [symbol], 13
        jz LastNum

        cmp byte [symbol], ' '
        jz SpaceChar

        cmp byte [symbol], 9
        jz SpaceChar

        cmp byte [symbol], '-'
        jne CheckNum

        ;минус разрешен только в начале числа
        cmp byte [hasDigit], 1
        je Error
        cmp byte [negative], 1
        je Error
        mov byte [negative], 1
        jmp ReadNum

    CheckNum:
        ;проверяем что символ является цифрой
        cmp byte [symbol], '0'
        jb Error
        cmp byte [symbol], '9'
        ja Error

        movzx eax, byte [symbol]
        sub eax, '0';переводим символ в цифру от 0 до 9

        ;собираем число отрицательным: edi = edi * 10 - цифра
        ;так можно прочитать и минимальное число -2147483648
        imul edi, edi, 10
        jo Error;OF = 1, если умножение вызвало переполнение
        sub edi, eax
        jo Error;OF = 1, если вычитание вызвало переполнение

        mov byte [hasDigit], 1
        jmp ReadNum

    SpaceChar:
        ;пробел означает конец числа
        cmp byte [hasDigit], 1
        je SaveNum
        ;один минус без цифр не является числом
        cmp byte [negative], 1
        je Error
        jmp ReadNum

    LastNum:
        ;сохраняем последнее число, если строка закончилась
        cmp byte [hasDigit], 1
        je SaveLastNum
        cmp byte [negative], 1
        je Error
        jmp EndArr

    SaveLastNum:
        mov byte [finished], 1

    SaveNum:
        ;место в матрице должно быть для каждого числа
        cmp esi, MATRIX_SIZE
        jae Error

        ;число уже собрано отрицательным
        ;если перед числом был '-', оставляем как есть
        ;если минуса не было, меняем знак на положительный
        cmp byte [negative], 1
        je AddToMatrix
        neg edi
        jo Error;число 2147483648 не помещается в знаковый dword

    AddToMatrix:
        ;записываем число в массив по порядку
        mov dword [matrix + esi * 4], edi
        inc esi
        xor edi, edi
        mov byte [negative], 0
        mov byte [hasDigit], 0

        cmp byte [finished], 1
        je EndArr
        jmp ReadNum

    EndArr:
        ;в матрице n*n элементов, поэтому чисел должно быть ровно столько
        cmp esi, MATRIX_SIZE
        jne Error

        mov eax, 4
        mov ebx, 1
        mov ecx, matrixMsg
        mov edx, matrixMsgLen
        int 0x80

        xor esi, esi;индекс элемента матрицы

    PrintMatrix:
        ;выводим числа строками по MATRIX_N элементов
        cmp esi, MATRIX_SIZE
        jae Main

        mov eax, [matrix + esi * 4]
        test eax, eax
        jns PrintAbs

        ;для отрицательного числа сначала выводим минус
        mov ebp, eax
        mov eax, 4
        mov ebx, 1
        mov ecx, minus
        mov edx, 1
        int 0x80
        mov eax, ebp
        ;для -2147483648 получится 80000000h
        ;ниже используется беззнаковое деление div, поэтому это допустимо
        neg eax

    PrintAbs:
        ;преобразуем число в строку, записываем цифры справа налево
        mov ecx, buffer + BUFFER_SIZE
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

        ;после каждого n-го элемента переходим на новую строку
        inc esi
        mov eax, esi
        xor edx, edx
        mov ebx, MATRIX_N
        div ebx
        test edx, edx
        jz PrintNewLine

        mov eax, 4
        mov ebx, 1
        mov ecx, Space
        mov edx, 1
        int 0x80
        jmp PrintMatrix

    PrintNewLine:
        mov eax, 4
        mov ebx, 1
        mov ecx, newline
        mov edx, 1
        int 0x80
        jmp PrintMatrix

    Main:
        ;сравниваем a[i][j] и a[j][i]
        ;главную диагональ проверять не нужно, поэтому j = i + 1
        xor esi, esi;номер строки i

    RowLoop:
        cmp esi, MATRIX_N
        jae Symmetric
        mov edi, esi
        inc edi;номер столбца j

    ColLoop:
        cmp edi, MATRIX_N
        jae NextRow

        ;получаем a[i][j], индекс i*n+j
        mov eax, esi
        imul eax, eax, MATRIX_N
        add eax, edi
        mov ebx, [matrix + eax * 4]

        ;получаем a[j][i], индекс j*n+i
        mov eax, edi
        imul eax, eax, MATRIX_N
        add eax, esi

        cmp ebx, [matrix + eax * 4]
        jne NotSymmetric

        inc edi
        jmp ColLoop

    NextRow:
        inc esi
        jmp RowLoop

    Symmetric:
        ;если все пары совпадают, матрица симметрична
        mov eax, 4
        mov ebx, 1
        mov ecx, yesMsg
        mov edx, yesMsgLen
        int 0x80
        jmp Exit

    NotSymmetric:
        ;нашли пару элементов, которые не совпадают
        mov eax, 4
        mov ebx, 1
        mov ecx, noMsg
        mov edx, noMsgLen
        int 0x80
        jmp Exit

    Error:
        ;неверное количество чисел или некорректный ввод
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
    MATRIX_N equ 3
    MATRIX_SIZE equ MATRIX_N * MATRIX_N
    BUFFER_SIZE equ 12

    symbol db 0
    negative db 0
    hasDigit db 0
    finished db 0

    minus db '-'
    Space db ' '
    newline db 10

    matrixMsg db 'matrix:', 10
    matrixMsgLen equ $ - matrixMsg

    yesMsg db 'symmetric', 10
    yesMsgLen equ $ - yesMsg

    noMsg db 'not symmetric', 10
    noMsgLen equ $ - noMsg

    errorMsg db 'error', 10
    errorLen equ $ - errorMsg

section .bss
    buffer resb BUFFER_SIZE
    matrix resd MATRIX_SIZE
