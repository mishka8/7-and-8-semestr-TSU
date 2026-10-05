section .text
    global _start

_start:
;верно ли, что ладья, расположенная на поле (x1, y1) шахматной доски,  
;"бьет" поле (x2, y2) (x1, y1, x2, y2 - целые от 1 до 8);

;идея реализации
;читаем 4 символа с строки представим что вводятся просто 4 числа 
;дальше у нас есть координаты [x1, y1] [x2, y2]
;у нас есть 3 случая
;первый x1 == x2 тогда надо проверить что y1 != y2 -> yes
;второй x1 != x2 тогда проверяем y1 == y2 -> yes
;третий x1 != x2 и y1 != y2 -> No

readX1:
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80

    mov al, [symbol]
    cmp al, '1'
    jb error
    cmp al, '8'
    ja error

    mov al, [symbol]
    mov [x1], al

readY1:
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80

    mov al, [symbol]
    cmp al, '1'
    jb error
    cmp al, '8'
    ja error

    mov al, [symbol]
    mov [y1], al

readX2:
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80

    mov al, [symbol]
    cmp al, '1'
    jb error
    cmp al, '8'
    ja error

    mov al, [symbol]
    mov [x2], al

readY2:
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80

    mov al, [symbol]
    cmp al, '1'
    jb error
    cmp al, '8'
    ja error

    mov al, [symbol]
    mov [y2], al

    ;сравниваем x
    mov al, [x1]
    cmp al, [x2]
    je sameX

    ;сравниваем y если x1 != x2
    mov al, [y1]
    cmp al, [y2]
    je messageYes
    jmp messageNo

sameX:
    mov al, [y1]
    cmp al, [y2]
    je messageNo
    jmp messageYes

messageNo:
    mov eax, 4
    mov ebx, 1
    mov ecx, msgNo
    mov edx, lenNo
    int 0x80
    jmp exit

messageYes:
    mov eax, 4
    mov ebx, 1
    mov ecx, msgYes
    mov edx, lenYes
    int 0x80
    jmp exit

error:
    mov eax, 4
    mov ebx, 1
    mov ecx, errorMsg
    mov edx, errorLen
    int 0x80
    jmp exit

exit:
    mov eax, 1
    xor ebx, ebx
    int 0x80

section .data
    symbol db ' '
    x1 db 0
    x2 db 0
    y1 db 0
    y2 db 0
    errorMsg db "error", 10
    errorLen equ $ - errorMsg
    msgNo db "No", 10
    lenNo equ $ - msgNo
    msgYes db "Yes", 10
    lenYes equ $ - msgYes
