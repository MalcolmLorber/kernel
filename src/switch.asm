global context_switch
context_switch:
        mov eax, [esp+4]
        mov edx, [esp+8]
        mov ecx, [esp+12]
        
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

        mov cr3, ecx
        ret
