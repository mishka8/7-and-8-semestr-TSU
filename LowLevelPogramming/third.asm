;найти N - количество целых чисел, квадраты которых больше X, но меньше Y
section .text
    global _start

_start:
;читаем x
    xor edi, edi
ReadingLoopX:
    mov eax, 3
    xor ebx, ebx
    mov edx, 1
    mov ecx, symbol
    int 80h

    cmp eax, 0;EOF -> конец X
    jle ReadingEndX
    cmp byte [symbol], 0xA; '\n' -> конец X
    jz ReadingEndX
    cmp byte [symbol], ' '; -> конец X (разделитель)
    jz ReadingEndX

    movzx eax, byte [symbol]
    cmp al, '0'
    jb Error
    cmp al, '9'
    ja Error

    sub eax, '0'
    imul edi, edi, 10
    add edi, eax

    jmp ReadingLoopX

ReadingEndX:
    mov [x], edi

    
;читаем y
    xor edi, edi    
ReadingLoopY:
    mov eax, 3
    xor ebx, ebx
    mov edx, 1
    mov ecx, symbol
    int 80h

    cmp eax, 0; EOF -> конец Y
    jle ReadingEndY

    cmp byte [symbol], 0xA  ; '\n' -> конец Y
    jz ReadingEndY

    cmp byte [symbol], ' '; -> конец X (разделитель)
    jz ReadingEndY

    movzx eax, byte [symbol]
    cmp al, '0'
    jb Error
    cmp al, '9'
    ja Error



    sub eax, '0'
    imul edi, edi, 10
    add edi, eax

    jmp ReadingLoopY

ReadingEndY:
    mov [y], edi


mov eax, [y]
cmp eax, [x]
jle notSolition

;сначала проверка на то что y > x
;потом начинаем c a = x^1/2 + 1 возводим a в квадрат
;сравниваем попало ли в промежуток 
;если да прибавляем n и так идем до границы корень y
mov dword [a], 1
main_loop:
    mov eax, [a]
    imul eax, eax ;eax = a²
    
    ;если квадрат а больше y
    cmp eax, [y]
    jge endMainLoop
    
    ; если квадрат а меньше x
    cmp eax, [x]
    jle skipA
    
    ;если попали сюда значит а подходит прибавляем n
    inc dword [n]
    ;add dword [n], 2

skipA:
    inc dword [a]
    jmp main_loop


endMainLoop:
    jmp printN

printN:
    mov eax, [n]
    lea  ecx, [buffer + buffer_size]
    mov  ebx, 10

PrintLoop:
    xor  edx, edx
    div  ebx
    add  dl, '0'
    dec  ecx
    mov  [ecx], dl
    test eax, eax
    jnz  PrintLoop

    lea  edx, [buffer + buffer_size]
    sub  edx, ecx
    mov  eax, 4
    mov  ebx, 1
    int  80h

    ; перевод строки
    mov  eax, 4
    mov  ebx, 1
    mov  ecx, newline
    mov  edx, 1
    int  80h
    jmp  Exit

notSolition:
    mov eax, 4
    mov ebx, 1
    mov ecx, notSolut
    mov edx, notSolutLen
    int 80h
    jmp Exit

Error:
    mov eax, 4
    mov ebx, 1
    mov ecx, errorMsg
    mov edx, errorLen
    int 80h

Exit:
    mov eax, 1
    xor ebx, ebx
    int 80h

section .data
    x dd 0
    y dd 0
    n dd 0
    a dd 0

    newline db 10

    errorMsg db "error", 10
    errorLen equ $ - errorMsg

    notSolut db "not solution", 10
    notSolutLen equ $ - notSolut

    symbol db  0

section .bss
    buffer_size equ 100
    buffer resb buffer_size
    
