;Дано трехзначное число. В нём зачеркнули первую справа цифру и приписали её слева. 
;Вывести полученное число.

;12 - error
;100 - 010 - 10
;213 - 312
;888 - 888
;887 - 788
;12123 - error

section .text
    global _start


_start:

readSpaceSkip:
    mov al, [symbol]
    cmp al, '0'
    jb  NotDigit
    cmp al, '9'
    ja  NotDigit
    jmp readNum

NotDigit:
    cmp al, ' '
    je  readNext
    cmp al, 10
    je  readNext
    cmp al, 13
    je  readNext
    jmp error

readNext:
    call readChar
    jmp readSpaceSkip

readNum:
    ;первая цифра трёхзначного числа не может быть 0
    cmp al, '0'
    je  error
    xor eax, eax
    mov al, [symbol]
    sub eax, '0'
    mov edx, [a]
    imul edx, edx, 10
    add edx, eax
    mov [a], edx
    call readChar

    ;вторая цифра
    cmp al, '0'
    jb  error
    cmp al, '9'
    ja  error
    xor eax, eax
    mov al, [symbol]
    sub eax, '0'
    mov edx, [a]
    imul edx, edx, 10
    add edx, eax
    mov [a], edx
    call readChar

    ;третья цифра
    cmp al, '0'
    jb  error
    cmp al, '9'
    ja  error
    xor eax, eax
    mov al, [symbol]
    sub eax, '0'
    mov edx, [a]
    imul edx, edx, 10
    add edx, eax
    mov [a], edx
    call readChar

; ============ после третьей цифры цифра быть не должна ============
    cmp al, '0'
    jb  numFlip
    cmp al, '9'
    ja  numFlip
    jmp error




numFlip:
    ; результат = (a % 10)*100 + (a / 10)
    mov eax, [a]
    xor edx, edx
    mov ebx, 10
    div ebx                 ; eax = a/10, edx = a%10
    imul edx, edx, 100
    add eax, edx
    mov [a], eax 

output:
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

    ; перевод строки
    mov eax, 4
    mov ebx, 1
    mov ecx, endStr
    mov edx, 1
    int 0x80
    jmp exit


readChar:
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80
    cmp eax, 1
    je  .ok
    mov byte [symbol], 0
.ok:
    mov al, [symbol]
    ret


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
    a dd 0
    numbuf times 12 db 0
    endStr db 10
    errorMsg db "error", 10
    errorLen equ $ - errorMsg 

