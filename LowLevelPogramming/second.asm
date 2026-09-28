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

;пропускаем пробелы
readSpaceSkip:
    mov al, [symbol]
    cmp al, '0'
    jb  NotDigit
    cmp al, '9'
    ja  NotDigit
    jmp readNum

;всё что не число
NotDigit:
    cmp al, ' '
    je  readNext
    cmp al, 10
    je  readNext
    jmp error

;чтение следующего
readNext:
    call readChar;работает как вызов функции
    jmp readSpaceSkip
    ;берем дарес следующей инструкции кладем в стек и возвращаемся в неё после ret кладем в стек 

;читаем и проверяем число
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

;проверка что число трехзначное если есть четвертая цифра - ошибка
    cmp al, '0'
    jb  numFlip
    cmp al, '9'
    ja  numFlip
    jmp error

;перестановка цифр
numFlip:
    ; результат = (a % 10)*100 + (a / 10)
    mov eax, [a]
    xor edx, edx
    mov ebx, 10
    div ebx                 ; eax = a/10, edx = a%10
    imul edx, edx, 100
    add eax, edx
    mov [a], eax 

;вывод
output:
    mov eax, [a]
    mov edi, numbuf+11
    mov byte [edi], 0
    mov ebx, 10
    xor ecx, ecx
;перевод числа в строку 
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

    ;перевод строки
    mov eax, 4
    mov ebx, 1
    mov ecx, endStr
    mov edx, 1
    int 0x80
    jmp exit

;читаем символ
readChar:
    mov eax, 3
    mov ebx, 0
    mov ecx, symbol
    mov edx, 1
    int 0x80
    ;читаем один символ потом проверяем что прочли один символ если все хорошо прыгаем на .ok
    ;и кладем либо ноль в случае неправильного чтения символа или кладем символ

    cmp eax, 1
    je  .ok
    mov byte [symbol], 0
.ok:
    mov al, [symbol]
    ret;возврат где мы вызывали

;ошибка
error:
    mov eax, 4
    mov ebx, 1
    mov ecx, errorMsg
    mov edx, errorLen
    int 0x80
    jmp exit

;выход
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
