global context_switch
context_switch:
        mov eax, [esp+4]
        mov edx, [esp+8]
        
        mov ecx, [esp]
        mov [edx+20], ecx

        push ebp
        push ebx
        push esi
        push edi
        mov ecx, cr3
        push ecx

        ; CR3 PROBLEM - STACK CHANGES, CANT ACCESS NEW MEM?
        ; WHY IS PROCS[CURPROC]->CTXT IN PROCESS MEM, NOT
        ; IN KERNEL MEM? INTERROGATE FREMMA

        mov [eax], esp
        mov esp, edx

        pop ecx
        mov cr3, ecx
        pop edi
        pop esi
        pop ebx
        pop ebp
        
        ret
