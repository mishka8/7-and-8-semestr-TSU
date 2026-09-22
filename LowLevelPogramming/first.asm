section .text
    global _start

_start:

; читаем пробелы первые и пропускаем их
readASpaceSkip:
    mov al, [symbol]
    cmp al, '0'
    jb NotDigitA ;переход если меньше 
    cmp al, '9'
    ja NotDigitA ;переход если больше
    jmp readA ;в остальных случаях продолжаем читать а


NotDigitA:
    cmp al, ' '
    je  readANext      ; пробел — пропускаем
    jmp error           ; ни цифра, ни пробел — выход

;читаем следующий символ - вспомогательная для readASpaceSkip
readANext:
    mov eax, 3 ;системный вызов read
    mov ebx, 0 ;ввод
    mov ecx, symbol ;куда складываем
    mov edx, 1 ;читаем один байт
    int 0x80 ;системный выход
    jmp readASpaceSkip 

; читаем а
readA:
    movzx eax, byte [symbol] ;увеличиваем до 32 бит 
    sub eax, '0' ;получаем число с помощью аски
    mov edx, [a] ;убираем значение а
    imul edx, edx, 10 ;умножаем на 10
    add edx, eax ;прибавляем что было в а
    mov [a], edx ;кладем назад в а 

    ;читаем 1, a = 0      0 * 10 + 1 = 1
    ;читаем 2, a = 1      1 * 10 + 2 = 12

    ;чтение след символа
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80

    ;сравнение и либо конец а либо продолжаем читать
    mov al, [symbol]
    cmp al, '0'
    jb endA
    cmp al, '9'
    ja endA
    jmp readA

;конец а и добавление в space1 пробел
endA:
    mov al, [symbol]
    mov [space1], al
    jmp readBSpaceSkip

;тоже самое что и в а сначала читаем пробелы между числами
readBSpaceSkip:
    mov al, [symbol]
    cmp al, '0'
    jb  NotDigitB
    cmp al, '9'
    ja  NotDigitB
    jmp readB

NotDigitB:
    cmp al, ' '
    je  readBNext
    jmp error

;читаем след символ - вспомогательная
readBNext:
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80
    jmp readBSpaceSkip


;читаем б
readB:
    movzx eax, byte [symbol]
    sub eax, '0'
    mov edx, [b]
    imul edx, edx, 10
    add edx, eax
    mov [b], edx

    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80

    mov al, [symbol]
    cmp al, '0'
    jb  endB
    cmp al, '9'
    ja  endB
    jmp readB

;конец б
endB:
    mov al, [symbol]
    mov [space2], al
    jmp readCSpaceSkip

;всё тоже самое
readCSpaceSkip:
    mov al, [symbol]
    cmp al, '0'
    jb  NotDigitC
    cmp al, '9'
    ja  NotDigitC
    jmp readC
    

NotDigitC:
    cmp al, ' '
    je  readCNext

    jmp error

readCNext:
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80
    jmp readCSpaceSkip

readC:
    movzx eax, byte [symbol]
    sub eax, '0'
    mov edx, [c]
    imul edx, edx, 10
    add edx, eax
    mov [c], edx

    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80

    mov al, [symbol]
    cmp al, '0'
    jb  endC
    cmp al, '9'
    ja  endC
    jmp readC

endC:
    jmp output

;вывод 
output:
    ;вывод "[ "
    mov eax, 4
    mov ebx, 1
    mov ecx, left_close
    mov edx, 2
    int 0x80

    ;вывод числа а
    mov eax, [a]
    mov edi, numbuf+11
    mov byte [edi], 0
    mov ebx, 10
    xor ecx, ecx
convA:
    xor edx, edx
    div ebx
    add dl, '0'
    dec edi
    mov [edi], dl
    inc ecx
    test eax, eax
    jnz convA
    mov eax, 4
    mov ebx, 1
    mov edx, ecx
    mov ecx, edi
    int 0x80

    ;вывод " ]"
    mov eax, 4
    mov ebx, 1
    mov ecx, right_close
    mov edx, 2
    int 0x80

    ;вывод пробела
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80

    ;вывод "[ "
    mov eax, 4
    mov ebx, 1
    mov ecx, left_close
    mov edx, 2
    int 0x80


    ;вывод числа б
    mov eax, [b]
    mov edi, numbuf+11
    mov byte [edi], 0
    mov ebx, 10
    xor ecx, ecx
convB:
    xor edx, edx
    div ebx
    add dl, '0'
    dec edi
    mov [edi], dl
    inc ecx
    test eax, eax
    jnz convB
    mov eax, 4
    mov ebx, 1
    mov edx, ecx
    mov ecx, edi
    int 0x80


    ;вывод " ]"
    mov eax, 4
    mov ebx, 1
    mov ecx, right_close
    mov edx, 2
    int 0x80

    ;вывод пробела
    mov eax, 4
    mov ebx, 1
    mov ecx, space
    mov edx, 1
    int 0x80

    ;вывод "[ "
    mov eax, 4
    mov ebx, 1
    mov ecx, left_close
    mov edx, 2
    int 0x80

    ;вывод числа с
     mov eax, [c]
    mov edi, numbuf+11
    mov byte [edi], 0
    mov ebx, 10
    xor ecx, ecx
convC:
    xor edx, edx
    div ebx
    add dl, '0'
    dec edi
    mov [edi], dl
    inc ecx
    test eax, eax
    jnz convC
    mov eax, 4
    mov ebx, 1
    mov edx, ecx
    mov ecx, edi
    int 0x80


    ;вывод " ]"
    mov eax, 4
    mov ebx, 1
    mov ecx, right_close
    mov edx, 2
    int 0x80

    ;перевод строки - '\n'
    mov eax, 4
    mov ebx, 1
    mov ecx, endStr
    mov edx, 1
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
    left_close db "[ "
    right_close db " ]"
    space db " "
    endStr db 10
    symbol db ' '
    a dd 0
    b dd 0
    c dd 0
    space1 db 0
    space2 db 0
    numbuf times 12 db 0
    errorMsg db "error", 10
    errorLen equ $ - errorMsg 
