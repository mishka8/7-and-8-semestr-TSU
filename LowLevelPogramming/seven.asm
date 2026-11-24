;Даны булевы вектора a и b длины n.
;Напечатать номера компонент, в которых вектора отличаются.

section .text
    global _start

_start:
    xor edi, edi                 ;вводимое значение n

ReadN:
    ;читаем длину вектора по одному символу
    mov eax, 3
    xor ebx, ebx
    mov ecx, symbol
    mov edx, 1
    int 0x80

    cmp eax, 0
    jle Error

    cmp byte [symbol], 10
    je EndN ;Enter завершает ввод n

    cmp byte [symbol], 13
    je ReadN

    ;длина должна состоять только из цифр
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
    cmp edi, 1
    jb Error

    mov [vectorN], edi
    xor edi, edi ;0 - читаем a, 1 - читаем b
    xor esi, esi ;номер текущей компоненты

ReadVector:
    ;читаем очередной символ вектора
    mov eax, 3
    xor ebx, ebx
    mov ecx, symbol
    mov edx, 1
    int 0x80

    cmp eax, 0
    jl Error
    je EndInput

    cmp byte [symbol], 13
    je ReadVector

    cmp byte [symbol], 10
    je EndVector ;Enter завершает вектор

    ;разрешены только символы 0 и 1
    cmp byte [symbol], '0'
    jb Error
    cmp byte [symbol], '1'
    ja Error

    ;символов не должно быть больше n
    cmp esi, [vectorN]
    jae Error

    mov al, [symbol]
    cmp edi, 0
    jne SaveB

    mov [vectorA + esi], al
    jmp NextSymbol

SaveB:
    mov [vectorB + esi], al

NextSymbol:
    inc esi
    jmp ReadVector

EndVector:
    ;в каждом векторе должно быть ровно n компонент
    cmp esi, [vectorN]
    jne Error

    cmp edi, 1
    je Main

    mov edi, 1 ;переходим к вводу b
    xor esi, esi
    jmp ReadVector

EndInput:
    ;второй вектор допускается завершить без Enter
    cmp edi, 1
    jne Error
    cmp esi, [vectorN]
    jne Error

Main:
    xor esi, esi

CompareLoop:
    cmp esi, [vectorN]
    jae Finish

    mov al, [vectorA + esi]
    cmp al, [vectorB + esi]
    je NextComponent

    ;между напечатанными номерами ставим пробел
    cmp byte [found], 0
    je PrintNumber

    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80

PrintNumber:
    mov byte [found], 1

    ;переводим номер компоненты (esi + 1) в десятичную строку
    mov eax, esi
    inc eax
    mov ebx, 10
    mov ecx, numberBuf + 12

ConvertNumber:
    xor edx, edx
    div ebx
    dec ecx
    add dl, '0'
    mov [ecx], dl
    test eax, eax
    jne ConvertNumber

    ;печатаем номер компоненты
    mov edx, numberBuf + 12
    sub edx, ecx
    mov eax, 4
    mov ebx, 1
    int 0x80

NextComponent:
    inc esi
    jmp CompareLoop

Finish:
    cmp byte [found], 0
    je SameVectors

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80
    jmp Exit

SameVectors:
    mov eax, 4
    mov ebx, 1
    mov ecx, sameMsg
    mov edx, sameMsgLen
    int 0x80
    jmp Exit

Error:
    mov eax, 4
    mov ebx, 1
    mov ecx, errorMsg
    mov edx, errorLen
    int 0x80

Exit:
    mov eax, 1
    xor ebx, ebx
    int 0x80

section .data
    MAX_N equ 1024

    symbol db 0
    found db 0
    space db ' '
    newline db 10
    numberBuf times 12 db 0

    sameMsg db 'same', 10
    sameMsgLen equ $ - sameMsg

    errorMsg db 'error', 10
    errorLen equ $ - errorMsg

section .bss
    vectorN resd 1
    vectorA resb MAX_N
    vectorB resb MAX_N
