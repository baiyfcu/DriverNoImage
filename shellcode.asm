
.CODE

 _DoMemAlloc      proc                ; CODE XREF: HookLuDashi+11C¡üp
                                         ; DATA XREF: .pdata:0000000140006054¡ýo
                 mov     [rsp+8], rcx
                 sub     rsp, 58h
                 mov     dword ptr [rsp+30h], 0
                 lea     rdx, [rsp+38h]
                 mov     rax, [rsp+60h]
                 mov     rcx, [rax+8]
                 mov     rax, 78888030C9E8F000h
                 call    rax
                 mov     [rsp+30h], eax
                 mov     rax, [rsp+60h]
                 mov     eax, [rax+20h]
                 mov     [rsp+34h], eax
                 cmp     dword ptr [rsp+30h], 0
                 jl      loc_140001F34
                 mov     rcx, [rsp+38h]
                 mov     rax, 78888040C9E8F000h
                 call    rax
                 mov     qword ptr [rsp+40h], 0
                 mov     dword ptr [rsp+28h], 40h
                 mov     dword ptr [rsp+20h], 3000h
                 lea     r9, [rsp+34h]
                 xor     r8d, r8d
                 lea     rdx, [rsp+40h]
                 mov     rcx, 0FFFFFFFFFFFFFFFFh
                 mov     rax, 78888090C9E8F000h
                 call    rax
                 test    eax, eax
                 jge     short loc_140001EFE
                 mov     dword ptr [rsp+30h], 40000015h
                 jmp     short loc_140001F06
 ; ---------------------------------------------------------------------------

 loc_140001EFE:                          ; CODE XREF: DoMemAlloc+92¡üj
                 mov     dword ptr [rsp+30h], 0

 loc_140001F06:                          ; CODE XREF: DoMemAlloc+9C¡üj
                 mov     rax, 78888060C9E8F000h
                 call    rax
                 mov     rcx, [rsp+38h]
                 mov     rax, 78888070C9E8F000h
                 call    rax
                 mov     rax, [rsp+60h]
                 mov     rax, [rax+18h]
                 mov     rcx, [rsp+40h]
                 mov     [rax], rcx

 loc_140001F34:                          ; CODE XREF: DoMemAlloc+40¡üj
                 xor     eax, eax
                 add     rsp, 58h
                 ret
 _DoMemAlloc      endp

 ; ---------------------------------------------------------------------------
int 3
int 3
int 3
int 3

 ; =============== S U B R O U T I N E =======================================


 _DoMemBase       proc                ; CODE XREF: HookLuDashi+107¡üp
                                         ; DATA XREF: .pdata:0000000140006060¡ýo

 ; FUNCTION CHUNK AT .text:00000001400036F0 SIZE 00000022 BYTES

 ; __unwind { // __C_specific_handler_0
                 mov     [rsp+8], rcx
                 sub     rsp, 48h
                 mov     qword ptr [rsp+30h], 0
                 lea     rdx, [rsp+28h]
                 mov     rax, [rsp+50h]
                 mov     rcx, [rax+8]
                 mov     rax, 78888030C9E8F000h
                 call    rax
                 mov     [rsp+20h], eax
                 cmp     dword ptr [rsp+20h], 0
                 jnz     short loc_140001F7F
                 cmp     qword ptr [rsp+28h], 0
                 jnz     short loc_140001F86

 loc_140001F7F:                          ; CODE XREF: DoMemBase+35¡üj
                 mov     eax, 40000015h
                 jmp     short loc_140001FF7
 ; ---------------------------------------------------------------------------

 loc_140001F86:                          ; CODE XREF: DoMemBase+3D¡üj
 ;   __try { // __except at 140001FDD
                 mov     rcx, [rsp+28h]
                 mov     rax, 78888040C9E8F000h
                 call    rax
                 mov     rcx, [rsp+28h]
                 mov     rax, 78888050C9E8F000h
                 call    rax
                 mov     [rsp+30h], rax
                 mov     rax, 78888060C9E8F000h
                 call    rax
                 mov     rcx, [rsp+28h]
                 mov     rax, 78888070C9E8F000h
                 call    rax
                 mov     rax, [rsp+50h]
                 mov     rax, [rax+18h]
                 mov     rcx, [rsp+30h]
                 mov     [rax], rcx
                 jmp     short loc_140001FF5
 ;   } // starts at 140001F86
 ; ---------------------------------------------------------------------------
 ;   __except(DoMemBase$filt$0) // owned by 140001F86
                 mov     rcx, [rsp+28h]
                 mov     rax, 78888070C9E8F000h
                 call    rax
                 mov     eax, 40000015h
                 jmp     short loc_140001FF7
 ; ---------------------------------------------------------------------------

 loc_140001FF5:                          ; CODE XREF: DoMemBase+9B¡üj
                 xor     eax, eax

 loc_140001FF7:                          ; CODE XREF: DoMemBase+44¡üj
                                         ; DoMemBase+B3¡üj
                 add     rsp, 48h
                 ret
 ; } // starts at 140001F40
 _DoMemBase       endp

 ; ---------------------------------------------------------------------------
int 3
int 3
int 3
int 3
 ; =============== S U B R O U T I N E =======================================


 _DoMemProtect    proc                ; CODE XREF: HookLuDashi+123¡üp
                                         ; DATA XREF: .pdata:000000014000606C¡ýo

                 mov     [rsp+8], rcx
                 sub     rsp, 58h
                 mov     dword ptr [rsp+30h], 0
                 mov     rax, [rsp+60h]
                 mov     eax, [rax+20h]
                 mov     [rsp+34h], eax
                 mov     rax, [rsp+60h]
                 mov     rax, [rax+18h]
                 mov     [rsp+48h], rax
                 lea     rdx, [rsp+40h]
                 mov     rax, [rsp+60h]
                 mov     rcx, [rax+8]
                 mov     rax, 78888030C9E8F000h
                 call    rax
                 mov     [rsp+30h], eax
                 cmp     dword ptr [rsp+30h], 0
                 jl      loc_1400020E7
                 mov     rcx, [rsp+40h]
                 mov     rax, 78888040C9E8F000h
                 call    rax
                 mov     qword ptr [rsp+38h], 0
                 mov     rax, [rsp+60h]
                 add     rax, 20h
                 mov     dword ptr [rsp+28h], 40h
                 mov     qword ptr [rsp+20h], 3000h
                 mov     r9d, eax
                 xor     r8d, r8d
                 lea     rdx, [rsp+38h]
                 mov     rcx, 0FFFFFFFFFFFFFFFFh
                 mov     rax, 788880A0C9E8F000h
                 call    rax
                 test    eax, eax
                 jge     short loc_1400020B4
                 mov     dword ptr [rsp+30h], 40000015h
                 jmp     short loc_1400020BC
 ; ---------------------------------------------------------------------------

 loc_1400020B4:                          ; CODE XREF: DoMemProtect+A8¡üj
                 mov     dword ptr [rsp+30h], 0

 loc_1400020BC:                          ; CODE XREF: DoMemProtect+B2¡üj
                 mov     rax, [rsp+60h]
                 mov     rcx, [rsp+38h]
                 mov     [rax+18h], rcx
                 mov     rax, 78888060C9E8F000h
                 call    rax
                 mov     rcx, [rsp+40h]
                 mov     rax, 78888070C9E8F000h
                 call    rax

 loc_1400020E7:                          ; CODE XREF: DoMemProtect+4E¡üj
                 xor     eax, eax
                 add     rsp, 58h
                 ret
 _DoMemProtect    endp

 ; ---------------------------------------------------------------------------
 algn_1400020EE:                         ; DATA XREF: .pdata:000000014000606C¡ýo
                 align 10h

 ; =============== S U B R O U T I N E =======================================


 _DoMemRead       proc                ; CODE XREF: HookLuDashi+10E¡üp
                                         ; DATA XREF: .pdata:0000000140006078¡ýo

                 mov     [rsp+8], rcx
                 sub     rsp, 78h
                 mov     dword ptr [rsp+44h], 0
                 mov     rax, [rsp+80h]
                 mov     rax, [rax+10h]
                 mov     [rsp+58h], rax
                 mov     rax, [rsp+80h]
                 mov     eax, [rax+20h]
                 mov     [rsp+40h], eax
                 mov     rax, [rsp+80h]
                 mov     rax, [rax+18h]
                 mov     [rsp+50h], rax
                 mov     eax, [rsp+40h]
                 mov     rcx, [rsp+58h]
                 add     rcx, rax
                 mov     rax, rcx
                 cmp     rax, [rsp+58h]
                 jb      short loc_14000219A
                 mov     eax, [rsp+40h]
                 mov     rcx, [rsp+50h]
                 add     rcx, rax
                 mov     rax, rcx
                 cmp     rax, [rsp+50h]
                 jb      short loc_14000219A
                 mov     eax, [rsp+40h]
                 mov     rcx, [rsp+58h]
                 add     rcx, rax
                 mov     rax, rcx
                 mov     rcx, 788880D0C9E8F000h
                 cmp     rax, rcx
                 ja      short loc_14000219A
                 mov     eax, [rsp+40h]
                 mov     rcx, [rsp+50h]
                 add     rcx, rax
                 mov     rax, rcx
                 mov     rcx, 788880D0C9E8F000h
                 cmp     rax, rcx
                 jbe     short loc_1400021A9

 loc_14000219A:                          ; CODE XREF: DoMemRead+56¡üj
                                         ; DoMemRead+6C¡üj ...
                 mov     eax, 40000015h
                 jmp     loc_14000226B
 ; ---------------------------------------------------------------------------
                 jmp     loc_140002269
 ; ---------------------------------------------------------------------------

 loc_1400021A9:                          ; CODE XREF: DoMemRead+A8¡üj
                 lea     rdx, [rsp+60h]
                 mov     rax, [rsp+80h]
                 mov     rcx, [rax+8]
                 mov     rax, 78888030C9E8F000h
                 call    rax
                 mov     [rsp+44h], eax
                 cmp     dword ptr [rsp+44h], 0
                 jl      loc_140002269
                 lea     rdx, [rsp+68h]
                 mov     rax, [rsp+80h]
                 mov     rcx, [rax]
                 mov     rax, 78888030C9E8F000h
                 call    rax
                 mov     [rsp+44h], eax
                 cmp     dword ptr [rsp+44h], 0
                 jl      short loc_140002258
                 mov     eax, [rsp+40h]
                 lea     rcx, [rsp+48h]
                 mov     [rsp+30h], rcx
                 mov     byte ptr [rsp+28h], 0
                 mov     [rsp+20h], rax
                 mov     r9, [rsp+50h]
                 mov     r8, [rsp+68h]
                 mov     rdx, [rsp+58h]
                 mov     rcx, [rsp+60h]
                 mov     rax, 78888080C9E8F000h
                 call    rax
                 mov     rcx, [rsp+60h]
                 mov     rax, 78888070C9E8F000h
                 call    rax
                 mov     rcx, [rsp+68h]
                 mov     rax, 78888070C9E8F000h
                 call    rax
                 jmp     short loc_140002269
 ; ---------------------------------------------------------------------------

 loc_140002258:                          ; CODE XREF: DoMemRead+10A¡üj
                 mov     rcx, [rsp+60h]
                 mov     rax, 78888070C9E8F000h
                 call    rax

 loc_140002269:                          ; CODE XREF: DoMemRead+B4¡üj
                                         ; DoMemRead+DF¡üj ...
                 xor     eax, eax

 loc_14000226B:                          ; CODE XREF: DoMemRead+AF¡üj
                 add     rsp, 78h
                 ret
 _DoMemRead       endp


int 3
int 3
int 3
int 3

 ; =============== S U B R O U T I N E =======================================


 _DoMemWrite      proc                ; CODE XREF: HookLuDashi+115¡üp
                                         ; DATA XREF: .pdata:0000000140006078¡ýo ...

                 mov     [rsp+8], rcx
                 sub     rsp, 78h
                 mov     dword ptr [rsp+44h], 0
                 mov     rax, [rsp+80h]
                 mov     rax, [rax+10h]
                 mov     [rsp+50h], rax
                 mov     rax, [rsp+80h]
                 mov     eax, [rax+20h]
                 mov     [rsp+40h], eax
                 mov     rax, [rsp+80h]
                 mov     rax, [rax+18h]
                 mov     [rsp+58h], rax
                 mov     eax, [rsp+40h]
                 mov     rcx, [rsp+50h]
                 add     rcx, rax
                 mov     rax, rcx
                 cmp     rax, [rsp+50h]
                 jb      short loc_14000231A
                 mov     eax, [rsp+40h]
                 mov     rcx, [rsp+58h]
                 add     rcx, rax
                 mov     rax, rcx
                 cmp     rax, [rsp+58h]
                 jb      short loc_14000231A
                 mov     eax, [rsp+40h]
                 mov     rcx, [rsp+50h]
                 add     rcx, rax
                 mov     rax, rcx
                 mov     rcx, 788880D0C9E8F000h
                 cmp     rax, rcx
                 ja      short loc_14000231A
                 mov     eax, [rsp+40h]
                 mov     rcx, [rsp+58h]
                 add     rcx, rax
                 mov     rax, rcx
                 mov     rcx, 788880D0C9E8F000h
                 cmp     rax, rcx
                 jbe     short loc_140002329

 loc_14000231A:                          ; CODE XREF: DoMemWrite+56¡üj
                                         ; DoMemWrite+6C¡üj ...
                 mov     eax, 40000015h
                 jmp     loc_1400023EB
 ; ---------------------------------------------------------------------------
                 jmp     loc_1400023E9
 ; ---------------------------------------------------------------------------

 loc_140002329:                          ; CODE XREF: DoMemWrite+A8¡üj
                 lea     rdx, [rsp+60h]
                 mov     rax, [rsp+80h]
                 mov     rcx, [rax+8]
                 mov     rax, 78888030C9E8F000h
                 call    rax
                 mov     [rsp+44h], eax
                 cmp     dword ptr [rsp+44h], 0
                 jl      loc_1400023E9
                 lea     rdx, [rsp+68h]
                 mov     rax, [rsp+80h]
                 mov     rcx, [rax]
                 mov     rax, 78888030C9E8F000h
                 call    rax
                 mov     [rsp+44h], eax
                 cmp     dword ptr [rsp+44h], 0
                 jl      short loc_1400023D8
                 mov     eax, [rsp+40h]
                 lea     rcx, [rsp+48h]
                 mov     [rsp+30h], rcx
                 mov     byte ptr [rsp+28h], 0
                 mov     [rsp+20h], rax
                 mov     r9, [rsp+50h]
                 mov     r8, [rsp+60h]
                 mov     rdx, [rsp+58h]
                 mov     rcx, [rsp+68h]
                 mov     rax, 78888080C9E8F000h
                 call    rax
                 mov     rcx, [rsp+60h]
                 mov     rax, 78888070C9E8F000h
                 call    rax
                 mov     rcx, [rsp+68h]
                 mov     rax, 78888070C9E8F000h
                 call    rax
                 jmp     short loc_1400023E9
 ; ---------------------------------------------------------------------------

 loc_1400023D8:                          ; CODE XREF: DoMemWrite+10A¡üj
                 mov     rcx, [rsp+60h]
                 mov     rax, 78888070C9E8F000h
                 call    rax

 loc_1400023E9:                          ; CODE XREF: DoMemWrite+B4¡üj
                                         ; DoMemWrite+DF¡üj ...
                 xor     eax, eax

 loc_1400023EB:                          ; CODE XREF: DoMemWrite+AF¡üj
                 add     rsp, 78h
                 ret
 _DoMemWrite      endp


int 3
int 3
int 3
int 3

 ; =============== S U B R O U T I N E =======================================


 _myIrpDispath    proc                ; CODE XREF: HookLuDashi+100¡üp
                                         ; DATA XREF: HookLuDashi+C6¡üo ...
                 mov     [rsp+10h], rdx
                 mov     [rsp+8], rcx
                 sub     rsp, 98h
                 mov     dword ptr [rsp+28h], 0
                 mov     qword ptr [rsp+60h], 0
                 mov     rax, [rsp+0A8h]
                 mov     rax, [rax+0B8h]
                 mov     [rsp+60h], rax
                 cmp     qword ptr [rsp+60h], 0
                 jz      loc_1400027F9
                 mov     rax, [rsp+60h]
                 movzx   eax, byte ptr [rax]
                 cmp     eax, 0Eh
                 jnz     loc_1400027F9
                 mov     rax, [rsp+60h]
                 mov     eax, [rax+18h]
                 mov     [rsp+30h], eax
                 cmp     dword ptr [rsp+30h], 222180h
                 jb      loc_1400027F9
                 mov     rax, [rsp+0A8h]
                 mov     qword ptr [rax+38h], 0
                 mov     rax, [rsp+0A8h]
                 mov     dword ptr [rax+30h], 0
                 mov     rax, 88888000C9E8F000h
                 mov     [rsp+20h], rax
                 mov     eax, [rsp+30h]
                 mov     [rsp+2Ch], eax
                 cmp     dword ptr [rsp+2Ch], 222180h
                 jz      short loc_1400024EA
                 cmp     dword ptr [rsp+2Ch], 222184h
                 jz      loc_14000257A
                 cmp     dword ptr [rsp+2Ch], 222188h
                 jz      loc_14000260E
                 cmp     dword ptr [rsp+2Ch], 22218Ch
                 jz      loc_1400026A2
                 cmp     dword ptr [rsp+2Ch], 222190h
                 jz      loc_140002742
                 jmp     loc_1400027DD
 ; ---------------------------------------------------------------------------

 loc_1400024EA:                          ; CODE XREF: myIrpDispath+AB¡üj
                 mov     rax, [rsp+20h]
                 mov     rax, [rax+8]
                 mov     [rsp+68h], rax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+10h]
                 mov     [rsp+38h], eax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax]
                 mov     [rsp+34h], eax
                 movzx   r8d, byte ptr [rsp+34h]
                 mov     edx, [rsp+38h]
                 mov     rcx, [rsp+68h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 mov     rax, [rsp+0A8h]
                 mov     rcx, [rax+18h]
                 call    qword ptr [rsp+68h]
                 mov     [rsp+28h], eax
                 mov     eax, [rsp+34h]
                 inc     eax
                 cdq
                 mov     ecx, 5Ah
                 idiv    ecx
                 mov     eax, edx
                 inc     eax
                 mov     rcx, [rsp+20h]
                 mov     [rcx], eax
                 mov     rax, [rsp+20h]
                 movzx   r8d, byte ptr [rax]
                 mov     edx, [rsp+38h]
                 mov     rcx, [rsp+68h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 jmp     loc_1400027DD
 ; ---------------------------------------------------------------------------

 loc_14000257A:                          ; CODE XREF: myIrpDispath+B5¡üj
                 mov     rax, [rsp+20h]
                 mov     rax, [rax+18h]
                 mov     [rsp+70h], rax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+20h]
                 mov     [rsp+40h], eax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+14h]
                 mov     [rsp+3Ch], eax
                 movzx   r8d, byte ptr [rsp+3Ch]
                 mov     edx, [rsp+40h]
                 mov     rcx, [rsp+70h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 mov     rax, [rsp+0A8h]
                 mov     rcx, [rax+18h]
                 call    qword ptr [rsp+70h]
                 mov     [rsp+28h], eax
                 mov     eax, [rsp+3Ch]
                 inc     eax
                 cdq
                 mov     ecx, 5Bh
                 idiv    ecx
                 mov     eax, edx
                 add     eax, 3
                 mov     rcx, [rsp+20h]
                 mov     [rcx+14h], eax
                 mov     rax, [rsp+20h]
                 movzx   r8d, byte ptr [rax+14h]
                 mov     edx, [rsp+40h]
                 mov     rcx, [rsp+70h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 jmp     loc_1400027DD
 ; ---------------------------------------------------------------------------

 loc_14000260E:                          ; CODE XREF: myIrpDispath+C3¡üj
                 mov     rax, [rsp+20h]
                 mov     rax, [rax+28h]
                 mov     [rsp+78h], rax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+30h]
                 mov     [rsp+48h], eax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+24h]
                 mov     [rsp+44h], eax
                 movzx   r8d, byte ptr [rsp+44h]
                 mov     edx, [rsp+48h]
                 mov     rcx, [rsp+78h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 mov     rax, [rsp+0A8h]
                 mov     rcx, [rax+18h]
                 call    qword ptr [rsp+78h]
                 mov     [rsp+28h], eax
                 mov     eax, [rsp+44h]
                 inc     eax
                 cdq
                 mov     ecx, 5Ch
                 idiv    ecx
                 mov     eax, edx
                 add     eax, 5
                 mov     rcx, [rsp+20h]
                 mov     [rcx+24h], eax
                 mov     rax, [rsp+20h]
                 movzx   r8d, byte ptr [rax+24h]
                 mov     edx, [rsp+48h]
                 mov     rcx, [rsp+78h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 jmp     loc_1400027DD
 ; ---------------------------------------------------------------------------

 loc_1400026A2:                          ; CODE XREF: myIrpDispath+D1¡üj
                 mov     rax, [rsp+20h]
                 mov     rax, [rax+38h]
                 mov     [rsp+80h], rax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+40h]
                 mov     [rsp+50h], eax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+34h]
                 mov     [rsp+4Ch], eax
                 movzx   r8d, byte ptr [rsp+4Ch]
                 mov     edx, [rsp+50h]
                 mov     rcx, [rsp+80h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 mov     rax, [rsp+0A8h]
                 mov     rcx, [rax+18h]
                 call    qword ptr [rsp+80h]
                 mov     [rsp+28h], eax
                 mov     eax, [rsp+4Ch]
                 inc     eax
                 cdq
                 mov     ecx, 5Dh
                 idiv    ecx
                 mov     eax, edx
                 add     eax, 7
                 mov     rcx, [rsp+20h]
                 mov     [rcx+34h], eax
                 mov     rax, [rsp+20h]
                 movzx   r8d, byte ptr [rax+34h]
                 mov     edx, [rsp+50h]
                 mov     rcx, [rsp+80h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 jmp     loc_1400027DD
 ; ---------------------------------------------------------------------------

 loc_140002742:                          ; CODE XREF: myIrpDispath+DF¡üj
                 mov     rax, [rsp+20h]
                 mov     rax, [rax+48h]
                 mov     [rsp+88h], rax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+50h]
                 mov     [rsp+58h], eax
                 mov     rax, [rsp+20h]
                 mov     eax, [rax+44h]
                 mov     [rsp+54h], eax
                 movzx   r8d, byte ptr [rsp+54h]
                 mov     edx, [rsp+58h]
                 mov     rcx, [rsp+88h]
                 mov     rax, 68888000C9E8F000h
                 call    rax
                 mov     rax, [rsp+0A8h]
                 mov     rcx, [rax+18h]
                 call    qword ptr [rsp+88h]
                 mov     [rsp+28h], eax
                 mov     eax, [rsp+54h]
                 inc     eax
                 cdq
                 mov     ecx, 5Eh
                 idiv    ecx
                 mov     eax, edx
                 add     eax, 9
                 mov     rcx, [rsp+20h]
                 mov     [rcx+44h], eax
                 mov     rax, [rsp+20h]
                 movzx   r8d, byte ptr [rax+44h]
                 mov     edx, [rsp+58h]
                 mov     rcx, [rsp+88h]
                 mov     rax, 68888000C9E8F000h
                 call    rax

 loc_1400027DD:                          ; CODE XREF: myIrpDispath+E5¡üj
                                         ; myIrpDispath+175¡üj ...
                 xor     edx, edx
                 mov     rcx, [rsp+0A8h]
                 mov     rax, 78888020C9E8F000h
                 call    rax
                 mov     eax, [rsp+28h]
                 jmp     short loc_140002813
 ; ---------------------------------------------------------------------------

 loc_1400027F9:                          ; CODE XREF: myIrpDispath+3C¡üj
                                         ; myIrpDispath+4D¡üj ...
                 xor     edx, edx
                 mov     rcx, [rsp+0A8h]
                 mov     rax, 78888020C9E8F000h
                 call    rax
                 mov     eax, [rsp+28h]

 loc_140002813:                          ; CODE XREF: myIrpDispath+3F7¡üj
                 add     rsp, 98h
                 ret
 _myIrpDispath    endp

int 3
int 3
int 3
int 3


 _EncryptDecryptBuf1 proc              ; CODE XREF: HookLuDashi+12F¡üp
                                         ; dealMyFun+43¡ýp
                                         ; DATA XREF: ...
                 mov     [rsp+18h], r8b
                 mov     [rsp+10h], edx
                 mov     [rsp+8], rcx
                 sub     rsp, 18h
                 mov     dword ptr [rsp], 0
                 jmp     short loc_140002943
 ; ---------------------------------------------------------------------------

 loc_14000293B:                          ; CODE XREF: EncryptDecryptBuf+4C¡ýj
                 mov     eax, [rsp]
                 inc     eax
                 mov     [rsp], eax

 loc_140002943:                          ; CODE XREF: EncryptDecryptBuf+19¡üj
                 mov     eax, [rsp+28h]
                 cmp     [rsp], eax
                 jge     short loc_14000296E
                 movsxd  rax, dword ptr [rsp]
                 mov     rcx, [rsp+20h]
                 movsx   eax, byte ptr [rcx+rax]
                 movsx   ecx, byte ptr [rsp+30h]
                 xor     eax, ecx
                 movsxd  rcx, dword ptr [rsp]
                 mov     rdx, [rsp+20h]
                 mov     [rdx+rcx], al
                 jmp     short loc_14000293B
 ; ---------------------------------------------------------------------------

 loc_14000296E:                          ; CODE XREF: EncryptDecryptBuf+2A¡üj
                 add     rsp, 18h
                 ret
 _EncryptDecryptBuf1 endp

 _EncryptDecryptBuf proc              ; CODE XREF: HookLuDashi+D4¡üp
                 movsxd  rax, edx
                 test    edx, edx
                 jle     short locret_140001DB3

 loc_140001DA7:                          ; CODE XREF: EncryptDecryptBuf+11¡ýj
                 xor     [rcx], r8b
                 inc     rcx
                 sub     rax, 1
                 jnz     short loc_140001DA7

 locret_140001DB3:                       ; CODE XREF: EncryptDecryptBuf+5¡üj
                 ret
 _EncryptDecryptBuf endp

int 3
int 3
int 3
int 3


END