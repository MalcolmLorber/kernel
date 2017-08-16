;contex switch function
;saves all relevant registers to the stack
;then changes esp to our new location
;and reforms the registers based on the stack at the new location
global context_switch
context_switch:
        mov eax, [esp+4]
        mov edx, [esp+8]

        push ebp
        push ebx
        push esi
        push edi

        mov [eax], esp
        mov esp, edx

        pop edi
        pop esi
        pop ebx
        pop ebp
        
        ret
