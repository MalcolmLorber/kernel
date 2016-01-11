global context_switch
context_switch:
        mov eax, [esp+4]
        mov edx, [esp+8]

        ; set return of new context to current context's
        ; address, so that we return to the proper location
        ; in kernel memory
        mov ecx, [esp]
        mov [edx+20], ecx

        push ebp
        push ebx
        push esi
        push edi
        mov ecx, cr3
        push ecx

        mov [eax], esp
        mov esp, edx

        pop ecx
        mov cr3, ecx
        pop edi
        pop esi
        pop ebx
        pop ebp
        
        ret
