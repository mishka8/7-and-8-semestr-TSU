;Найти натуральное число от 1 до 10000 с максимальной суммой делетилей
;в ответе я гуглил должно быть
;9240 и сумма делителей не считая самого числа 25320
section .text
    global _start

_start:
;цикл где мы будет увеличивать число
;внутри цикл где мы будем делить это число на разные делители
;и обновлять переменную
;чтобы программа работала быстрее можно сделать с самого максимального до минимального и если нашли число выход
    mov esi, 1

first_loop:
    cmp esi, [b];сравниваем с верхней границей если больше выходим
    ja print_soluton

    xor edi, edi;сумма делителей
    mov ecx, 1;делители каждый раз начинается с 1

    mov eax, esi
    shr eax, 1;с помощью сдвига делим на 2
    mov ebp, eax;чтобы цикл шел до половины числа
    add ebp, 1


    second_loop:
        cmp ecx, ebp;если делитель больше половины числа которое проверяем выходим
        jge check_sum 

        mov eax, esi;сюда кладем число которое будем делить
        xor edx, edx;очищаем здесь будет остаток будем сравнивать его с нулем 
        div ecx;тут делитель

        cmp edx, 0
        jne next_del

        add edi, ecx;добавляем к сумме делителей

        next_del:
            inc ecx
            jmp second_loop

    

    check_sum:
    cmp edi, [max_sum];сравниваем сумму делителей с максимальной суммой
    jbe next_num;если меньше или равно увеличиваем число
    mov [max_sum], edi;кладем новую максимальную сумму
    mov [best], esi;кладем новое число с максимальной суммой делителей

    next_num:
        inc esi;увеличиваем число на единицу 
        jmp first_loop

print_soluton:
    mov eax, 4
    mov ebx, 1
    mov ecx, msg_num
    mov edx, msg_num_len
    int 0x80

    mov eax, [best]
    mov esi, buff + 12
    mov ebx, 10

print_best:
    xor edx, edx
    div ebx
    add dl, '0'
    dec esi
    mov [esi], dl
    test eax, eax
    jnz print_best


    mov edx, buff + 12
    sub edx, esi
    mov ecx, esi
    mov eax, 4
    mov ebx, 1
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, msg_sum
    mov edx, msg_sum_len
    int 0x80

    ; печать [max_sum]
    mov eax, [max_sum]
    mov esi, buff + 12
    mov ebx, 10

print_summ:
    xor edx, edx
    div ebx
    add dl, '0'
    dec esi
    mov [esi], dl
    test eax, eax
    jnz print_summ

    mov edx, buff + 12
    sub edx, esi
    mov ecx, esi
    mov eax, 4
    mov ebx, 1
    int 0x80

    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    jmp exit


exit:
    mov eax, 1
    xor ebx, ebx
    int 80h

section .data
    b dd 10000;граница
    max_sum dd 0
    best dd 0

    msg_num        db "num - "
    msg_num_len    equ $ - msg_num

    msg_sum        db " sum - "
    msg_sum_len    equ $ - msg_sum

    newline     db 10


section .bss
    buff resb 12
