global context_switch
context_switch:
        mov eax, [esp+4]
        mov edx, [esp+8]

        push ebp
        push ebx
        push esi
        push edi
        push cr3

        mov [eax], esp
        mov esp, edx

        pop cr3
        pop edi
        pop esi
        pop ebx
        pop ebp
        
        ret
