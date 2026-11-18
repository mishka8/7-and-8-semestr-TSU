;задана целочисленная квадратная матрица n-го порядка.
;определить, является ли она симметричной относительно главной диагонали.

;сначала вводим n с клавиатуры и нажимаем Enter
;затем вводим n строк, в каждой по n целых чисел
;для симметричности должно выполняться a[i][j] = a[j][i]
;переполнение при чтении чисел проверяем через флаг OF и переход jo
;MAX_N - максимально допустимый размер матрицы

section .text
    global _start

_start:
    xor edi, edi;вводимое значение n

    ReadN:
        ;читаем размер матрицы по одному символу
        mov eax, 3
        xor ebx, ebx
        mov edx, 1
        mov ecx, symbol
        int 0x80

        cmp eax, 0
        jle Error

        cmp byte [symbol], 10
        je EndN;Enter завершает ввод n

        cmp byte [symbol], 13
        je ReadN;пропускаем возврат каретки

        ;размер матрицы должен состоять только из цифр
        cmp byte [symbol], '0'
        jb Error
        cmp byte [symbol], '9'
        ja Error

        movzx eax, byte [symbol]
        sub eax, '0'

        ;собираем n: edi = edi * 10 + цифра
        imul edi, edi, 10
        jo Error
        add edi, eax
        jo Error

        cmp edi, MAX_N
        ja Error
        jmp ReadN

    EndN:
        ;размер должен быть больше нуля и не больше MAX_N
        cmp edi, 1
        jb Error
        cmp edi, MAX_N
        ja Error

        mov [matrixN], edi;сохраняем введенный размер n
        mov eax, edi
        imul eax, edi;вычисляем n*n
        jo Error
        mov [matrixSize], eax

        xor edi, edi;текущее число матрицы
        xor esi, esi;количество записанных элементов
        xor ebp, ebp;количество элементов в текущей строке

    ReadNum:
        ;читаем число матрицы по одному символу
        mov eax, 3
        xor ebx, ebx
        mov edx, 1
        mov ecx, symbol
        int 0x80

        cmp eax, 0
        jl Error
        jz LastNum

        cmp byte [symbol], 10
        je NewLine;Enter завершает строку матрицы

        cmp byte [symbol], 13
        je ReadNum;пропускаем возврат каретки

        cmp byte [symbol], ' '
        je SpaceChar

        cmp byte [symbol], 9
        je SpaceChar

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
        sub eax, '0'

        ;собираем число отрицательным: edi = edi * 10 - цифра
        imul edi, edi, 10
        jo Error
        sub edi, eax
        jo Error

        mov byte [hasDigit], 1
        jmp ReadNum

    SpaceChar:
        ;пробел заканчивает только число, но не строку матрицы
        cmp byte [hasDigit], 1
        je SaveNum
        cmp byte [negative], 1
        je Error
        jmp ReadNum

    NewLine:
        ;Enter означает конец строки матрицы
        cmp byte [hasDigit], 1
        jne CheckRow
        mov byte [endRow], 1
        jmp SaveNum

    LastNum:
        ;если ввод закончился без Enter
        cmp byte [hasDigit], 1
        jne CheckRow
        mov byte [endRow], 1
        jmp SaveNum

    SaveNum:
        ;проверяем количество чисел в текущей строке
        cmp ebp, [matrixN]
        jae Error

        ;проверяем количество чисел во всей матрице
        cmp esi, [matrixSize]
        jae Error

        ;если минуса не было, меняем знак на положительный
        cmp byte [negative], 1
        je AddToMatrix
        neg edi
        jo Error

    AddToMatrix:
        ;сохраняем число в массив
        mov dword [matrix + esi * 4], edi
        inc esi
        inc ebp

        ;сбрасываем текущее число и флаги
        xor edi, edi
        mov byte [negative], 0
        mov byte [hasDigit], 0

        ;если был Enter, переходим к концу строки
        cmp byte [endRow], 1
        je EndRow
        jmp ReadNum

    CheckRow:
        ;минус без цифр является ошибкой
        cmp byte [negative], 1
        je Error

    EndRow:
        ;в строке должно быть ровно n чисел
        cmp ebp, [matrixN]
        jne Error

        ;сбрасываем количество элементов строки
        xor ebp, ebp
        mov byte [endRow], 0

        ;если вся матрица заполнена, переходим в Main
        cmp esi, [matrixSize]
        je Main

        ;иначе читаем следующую строку
        jmp ReadNum

    Main:
        ;сравниваем a[i][j] и a[j][i]
        ;главную диагональ проверять не нужно
        xor esi, esi;номер строки i

    RowLoop:
        cmp esi, [matrixN]
        jae Symmetric
        mov edi, esi
        inc edi;номер столбца j = i + 1

    ColLoop:
        cmp edi, [matrixN]
        jae NextRow

        ;получаем a[i][j], индекс i*n+j
        mov eax, esi
        imul eax, dword [matrixN]
        add eax, edi
        mov ebx, [matrix + eax * 4]

        ;получаем a[j][i], индекс j*n+i
        mov eax, edi
        imul eax, dword [matrixN]
        add eax, esi

        ;сравниваем два элемента
        cmp ebx, [matrix + eax * 4]
        jne NotSymmetric

        inc edi
        jmp ColLoop

    NextRow:
        inc esi
        jmp RowLoop

    Symmetric:
        ;если все пары совпадают
        mov eax, 4
        mov ebx, 1
        mov ecx, yesMsg
        mov edx, yesMsgLen
        int 0x80
        jmp Exit

    NotSymmetric:
        ;если хотя бы одна пара не совпадает
        mov eax, 4
        mov ebx, 1
        mov ecx, noMsg
        mov edx, noMsgLen
        int 0x80
        jmp Exit

    Error:
        ;неверный размер, количество чисел или некорректный ввод
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
    MAX_N equ 32
    MAX_SIZE equ MAX_N * MAX_N

    symbol db 0
    negative db 0
    hasDigit db 0
    endRow db 0

    yesMsg db 'symmetric', 10
    yesMsgLen equ $ - yesMsg

    noMsg db 'not symmetric', 10
    noMsgLen equ $ - noMsg

    errorMsg db 'error', 10
    errorLen equ $ - errorMsg

section .bss
    matrixN resd 1
    matrixSize resd 1
    matrix resd MAX_SIZE
