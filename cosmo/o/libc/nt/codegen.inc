/ Links function from external DLL.
/
/ This embeds a function pointer in the binary. The NT Executive
/ fills its value before control is handed off to the program.
/
/ @note only ELF toolchains are powerful enough to use this
/ @see libc/nt/master.sh
/ @see ape/ape.lds
/ @see winimp
.macro .imp dll:req fn:req actual:req hint
 .dll \dll
 .section .piro.data.sort.iat.2.\dll\().2.\actual,"aw",@progbits
 .type \fn,@object
 .align 8
\fn: .quad (((\dll\().\actual)) - (0x400000))
 .size \fn,.-\fn
 .globl \fn
 .hidden \fn
 .previous
 .section .idata.ro.ilt.\dll\().2.\actual,"a",@progbits
.Lidata.ilt.\dll\().\actual:
 .quad (((\dll\().\actual)) - (0x400000))
 .type .Lidata.ilt.\dll\().\actual,@object
 .size .Lidata.ilt.\dll\().\actual,.-.Lidata.ilt.\dll\().\actual
 .previous
 .section .idata.ro.hnt.\dll\().2.\actual,"a",@progbits
\dll\().\actual:
 .ifnb \hint # hint i.e. guess function ordinal
 .short \hint
 .else
 .short 0
 .endif
 .asciz "\actual"
 .align 2 # documented requirement
 .globl \dll\().\actual
 .hidden \dll\().\actual
 .type \dll\().\actual,@object
 .size \dll\().\actual,.-\dll\().\actual
 .previous
.endm
/ Defines DLL import.
/ @note this is an implementation detail of .imp
.macro .dll name:req
  .section .idata.ro.idt.2.\name,"aG",\name,comdat
 .equ .Lidata.idt.\name,.
 .long ((idata.ilt.\name) - (0x400000)) # ImportLookupTable
 .long 0 # TimeDateStamp
 .long 0 # ForwarderChain
 .long ((.Lidata.str.\name) - (0x400000)) # DllNameRva
 .long ((idata.iat.\name) - (0x400000)) # ImportAddressTable
 .type .Lidata.idt.\name,@object
 .size .Lidata.idt.\name,.-.Lidata.idt.\name
  .previous
  .section .idata.ro.ilt.\name\().1,"aG",\name,comdat
 .align 8
 .type idata.ilt.\name,@object
idata.ilt.\name:
 .previous .section .idata.ro.ilt.\name\().3,"aG",\name,comdat
 .quad 0
  .previous
  .section .idata.ro.hnt.\name\().1,"aG",\name,comdat
 .align 8
 .type idata.hnt.\name,@object
 .equ idata.hnt.\name,.
  .previous
  .section .piro.data.sort.iat.2.\name\().1,"awG",\name,comdat
 .align 8
 .type idata.iat.\name,@object
idata.iat.\name:
 .previous .section .piro.data.sort.iat.2.\name\().3,"awG",\name,comdat
 .quad 0
  .previous
  .section .rodata.str1.1,"aSM",@progbits,1
.Lidata.str.\name:
 .asciz "\name\().dll"
  .previous
.endm
/ Begins definition of frameless function that calls no functions.
.macro .leafprologue
 push %rbp
 mov %rsp,%rbp
.endm
/ Ends definition of frameless function that calls no functions.
.macro .leafepilogue
 pop %rbp
 ret
.endm
/ Good alignment for functions where alignment actually helps.
/ @note 16-byte
.macro .alignfunc
 .p2align 4
.endm
/ Good alignment for loops where alignment actually helps.
/ @note 16-byte if <10 padding otherwise 8-byte
.macro .alignloop
 .p2align 4,,10
 .p2align 4
.endm
/ Loads Effective Address
/ Supporting security blankets
.macro plea symbol:req reg64:req reg32:req
 mov $\symbol,\reg32
.endm
/ Loads Effective Address to Stack
/ Supporting security blankets
.macro pshaddr symbol:req
 push $\symbol
.endm
/ TODO(jart): delete
/ Loads Effective Address
/ Supporting security blankets
.macro ezlea symbol:req reg:req
 mov $\symbol,%e\reg
.endm
.macro farcall symbol:req
 .type \symbol,@function
 call \symbol\()@plt
.endm
/ Creates first stack frame.
.macro .frame0
 and $-16,%rsp
 xor %ebp,%ebp
.endm
.macro .source symbol:req
.endm
/ Inserts profiling hook in prologue if cc wants it.
/
/ Cosmopolitan does this in a slightly different way from normal
/ GNU toolchains. We always use the -mnop-mcount behavior, since
/ the runtime is able to morph the binary at runtime. It is good
/ since we can put hooks for profiling and function tracing into
/ most builds, without any impact on performance.
/
/ @cost ≥0.3 cycles, 5 bytes
/ @see build/compile
.macro .profilable
1382:
/ nopl 0x00(%rax,%rax,1)
 .byte 0x0f,0x1f,0x44,0x00,0x00
.endm
/ Pushes RVA on stack of linktime mergeable string literal.
/ @see popstr
.macro pushstr text
 .section .rodata.str1.1,"aSM",@progbits,1
.Lstr\@: .asciz "\text"
 .endobj .Lstr\@
 .previous
 push $.Lstr\@ - 0x400000
.endm
/ Pops off stack string address.
/ @see pushstr
.macro popstr dest:req
 addl $IMAGE_BASE_VIRTUAL,(%rsp)
 pop \dest
.endm
/ Loads address of linktime mergeable string literal into register.
.macro getstr text:req reg64:req reg32 regsz64 regsz32 bias=0
 .section .rodata.str1.1,"aSM",@progbits,1
  .type .Lstr\@,@object
.Lstr\@: .asciz "\text"
.Lstr\@.size = .-.Lstr\@ - 1
  .size .Lstr\@,.-.Lstr\@
 .previous
 plea .Lstr\@,\reg64,\reg32
 .ifnb \regsz64
 mov $.Lstr\@.size,\regsz32
 .endif
.endm
/ TODO(jart): delete
/ Loads address of linktime mergeable string literal into register.
.macro loadstr text:req reg:req regsz bias=0
 .section .rodata.str1.1,"aSM",@progbits,1
  .type .Lstr\@,@object
.Lstr\@: .asciz "\text"
.Lstr\@.size = .-.Lstr\@ - 1
  .size .Lstr\@,.-.Lstr\@
 .previous
 ezlea .Lstr\@,\reg
 .ifnb \regsz
 mov $.Lstr\@.size,%e\regsz
 .endif
.endm
/ Shorthand notation for widely-acknowledged sections.
.macro .rodata
 .section .rodata,"a",@progbits
.endm
.macro .init
 .section .init,"ax",@progbits
.endm
.macro .real
 .section .text.real,"ax",@progbits
.endm
.macro .text.startup
 .section .text.startup,"ax",@progbits
.endm
.macro .text.exit
 .section .text.exit,"ax",@progbits
.endm
.macro .firstclass
 .section .text.hot,"ax",@progbits
.endm
.macro .text.unlikely
 .section .text.unlikely,"ax",@progbits
.endm
.macro .text.likely
 .section .text.hot,"ax",@progbits
.endm
.macro .text.modernity
 .section .text.modernity,"ax",@progbits
 .align 16
.endm
.macro .text.antiquity
 .section .text.antiquity,"ax",@progbits
.endm
.macro .text.hot
 .section .text.hot,"ax",@progbits
.endm
.macro .preinit_array
 .section .preinit_array,"a",@init_array
.endm
.macro .init_array
 .section .init_array,"a",@init_array
.endm
.macro .text.windows
 .section .text.windows,"ax",@progbits
.endm
/ Mergeable numeric constant sections.
/
/ @note linker de-dupes item/values across whole compile
/ @note therefore item/values are reordered w.r.t. link order
/ @note therefore no section relative addressing
.macro .rodata.cst4
 .section .rodata.cst4,"aM",@progbits,4
 .align 4
.endm
.macro .rodata.cst8
 .section .rodata.cst8,"aM",@progbits,8
 .align 8
.endm
.macro .rodata.cst16
 .section .rodata.cst16,"aM",@progbits,16
 .align 16
.endm
.macro .rodata.cst32
 .section .rodata.cst32,"aM",@progbits,32
 .align 32
.endm
.macro .rodata.cst64
 .section .rodata.cst64,"aM",@progbits,64
 .align 64
.endm
.macro .tdata
 .section .tdata,"awT",@progbits
 .align 4
.endm
.macro .tbss
 .section .tdata,"awT",@nobits
 .align 4
.endm
/ Mergeable NUL-terminated UTF-8 string constant section.
/
/ @note linker de-dupes C strings here across whole compile
/ @note therefore item/values are reordered w.r.t. link order
/ @note therefore no section relative addressing
.macro .rodata.str1.1
 .section .rodata.str1.1,"aSM",@progbits,1
 .align 1
.endm
/ Locates unreferenced code invulnerable to --gc-sections.
.macro .keep.text
 .section .keep.text,"ax",@progbits
.endm
/ Flags code as only allowed for testing purposes.
.macro .testonly
 .section .test,"ax",@progbits
.endm
/ Makes code runnable while code morphing.
.macro .privileged
 .section .privileged,"ax",@progbits
.endm
/ Post-Initialization Read-Only (PIRO) BSS section.
/ @param ss is an optional string, for control image locality
.macro .piro ss
 .ifnb \ss
 .section .piro.sort.bss.\ss,"aw",@nobits
 .else
 .section .piro.bss,"aw",@nobits
 .endif
.endm
/ Helpers for Cosmopolitan _init() amalgamation magic.
/ @param name should be consistent across macros for a module
/ @see libc/runtime/_init.S
.macro .initro number:req name:req
 .section .initro.\number\().\name,"a",@progbits
 .align 8
.endm
.macro .initbss number:req name:req
 .section .piro.bss.init.2.\number\().\name,"aw",@nobits
 .align 8
.endm
.macro .init.start number:req name:req
 .section .init.\number\().\name,"ax",@progbits
\name:
.endm
.macro .init.end number:req name:req bnd=globl vis
 .endfn \name,\bnd,\vis
 .previous
.endm
/ Declares alternative implementation of function.
/ @param implement e.g. tinymath_pow
/ @param canonical e.g. pow
.macro .alias implement:req canonical:req
 .equ \canonical,\implement
 .weak \canonical
.endm
/ Ends function definition.
/ @cost saves 1-3 lines of code
.macro .endfn name:req bnd vis
 .size \name,.-\name
 .type \name,@function
 .ifnb \bnd
  .\bnd \name
 .endif
 .ifnb \vis
  .\vis \name
 .endif
.endm
/ Ends variable definition.
/ @cost saves 1-3 lines of code
.macro .endobj name:req bnd vis
 .size \name,.-\name
 .type \name,@object
 .ifnb \bnd
  .\bnd \name
 .endif
 .ifnb \vis
  .\vis \name
 .endif
.endm
/ LOOP Instruction Replacement.
/ With its mop-Fusion Mexican equivalent.
/ Thus avoiding 3x legacy pipeline slowdown.
.macro .loop label:req
 .byte 0x83,0xe9,0x01 # sub $1,%ecx
 jnz \label
.endm
/ Pushes CONSTEXPR ∈ [-128,127].
/ @note assembler is wrong for non-literal constexprs
.macro pushb x:req
 .byte 0x6a,\x
.endm
/ Sign-extends CONSTEXPR ∈ [-128,127] to REGISTER.
/ @cost ≥1 cycles, -2 bytes
.macro pushpop constexpr:req register:req
 pushb \constexpr
 pop \register
.endm
/ Moves REGISTER to REGISTER.
/ @cost ≥1 cycles, -1 REX byte
.macro movpp src:req dest:req
 push \src
 pop \dest
.endm
/ Declares optional function.
.macro .optfn fn:req
 .globl \fn
 .weak \fn
 .equ \fn,missingno
 .type \fn,@function
.endm
/ Embeds fixed-width zero-filled string table.
/ @note zero-padded ≠ nul-terminated
.macro .fxstr width head rest:vararg
 .ifnb \head
0: .ascii "\head"
 .org 0b+\width
 .fxstr \width,\rest
 .endif
.endm
/ Embeds Fixed-Width Zero-Padded String.
/ @note .fxstr is better
.macro .ascin str:req fieldsize:req
1347: .ascii "\str"
 .org 1347b+\fieldsize,0x00
.endm
/ Marks symbols as object en-masse.
/ @note zero-padded ≠ nul-terminated
.macro .object symbol rest:vararg
 .ifnb \symbol
 .type \symbol,@object
 .object \rest
 .endif
.endm
/ Pads function prologue unconditionally for runtime hooking.
/ @cost ≥0.3 cycles, 5 bytes
/ @see .profilable
.macro .hookable
/ nopl 0x00(%rax,%rax,1)
83457: .byte 0x0f,0x1f,0x44,0x00,0x00
 .section __mcount_loc,"a",@progbits
  .quad 83457b
 .previous
.endm
/ Puts initialized data in uninitialized data section.
.macro .bsdata name:req expr:req bnd vis
  .section .initbss.300._init_\name,"aw",@nobits
\name: .quad 0
 .endobj \name,\bnd,\vis
  .previous
  .section .initro.300._init_\name,"a",@progbits
 .quad \expr
  .previous
  .section .init.300._init_\name,"ax",@progbits
_init_\name:
 movsq
 .endfn _init_\name
  .previous
.endm
/ ICE Breakpoint.
/ Modern gas forgot this but objdump knows
/ @mode long,legacy,real
 .macro icebp
 .byte 0xF1
 .endm
 .macro int1
 icebp
 .endm
/ Sets breakpoint for software debugger.
/ @mode long,legacy,real
.macro .softicebp
 .byte 0x53 # push bx
 .byte 0x87,0xdb # xchg bx,bx (bochs breakpoint)
 .byte 0x5b # pop bx
 .byte 0x66,0x90 # xchg ax,ax (microsoft breakpoint)
 int3 # gdb breakpoint
.endm
/ Assembles Intel Official 4-Byte NOP.
.macro fatnop4
 .byte 0x0f,0x1f,0x40,0x00
.endm
/ Pulls unrelated module into linkage.
/
/ In order for this technique to work with --gc-sections, another
/ module somewhere might want to weakly reference whats yoinked.
.macro yoink symbol:req
 .section .yoink
 nop "\symbol"
 .previous
.endm
.macro .yoink symbol:req
 .section .yoink
 nop "\symbol"
 .previous
.endm
/ Calls Windows function.
/
/ @param cx,dx,r8,r9,stack
/ @return ax
/ @clob ax,cx,dx,r8-r11
.macro ntcall symbol:req
 sub $32,%rsp
 call *\symbol(%rip)
 add $32,%rsp
.endm
/ Custom emulator instruction for bottom stack frame.
.macro bofram endfunc:req
 .byte 0x0f,0x1f,0105,\endfunc-. # nopl disp8(%rbp)
.endm
/ Calls near (i.e. pc+pcrel<64kB) FUNCTION.
/ @mode long,legacy,real
/ @cost 9 bytes overhead
.macro rlcall function:req
 .byte 0x50 # push %[er]ax
 .byte 0xb8,0,0 # mov $?,%[e]ax
 jmp 911f
 .byte 0x58 # pop %[er]ax
 .byte 0xe8 # call Jvds
 .long \function\()-.-4
 jmp 912f
911: .byte 0x58 # pop %[er]ax
 .byte 0xe8 # call Jvds
 .short \function\()-.-2
912:
.endm
/ Loads far (i.e. <1mb) abs constexpr ADDRESS into ES:DI+EDX+RDX.
/ @mode long,legacy,real
.macro movesdi address:req
 .byte 0xbf # mov $0x????xxxx,%[e]di
 .short \address>>4
 .byte 0x8e,0xc7 # mov %di,%es
 .byte 0xbf # mov $0x????xxxx,%[e]di
 .short \address&0xf
 jmp 297f
 .byte 0xbf # mov $0x????xxxx,%edi
 .long \address
297:
.endm
/ Loads 16-bit CONSTEXPR into Qw-register w/ optional zero-extend.
/ @mode long,legacy,real
.macro bbmov constexpr:req abcd abcd.hi:req abcd.lo:req
 .ifnb \abcd
  .if (\constexpr)<128 && (\constexpr)>=0
 pushpop \constexpr,\abcd
   .exitm
  .endif
 .endif
 movb $(\constexpr)>>8&0xff,\abcd.hi
 movb $(\constexpr)&0xff,\abcd.lo
.endm
/ Compares 16-bit CONSTEXPR with Qw-register.
/ @mode long,legacy,real
.macro bbcmp constexpr:req abcd.hi:req abcd.lo:req
 cmpb $(\constexpr)>>8&0xff,\abcd.hi
 jnz 387f
 cmpb $(\constexpr)&0xff,\abcd.lo
387:
.endm
/ Adds 16-bit CONSTEXPR to Qw-register.
/ @mode long,legacy,real
.macro bbadd constexpr:req abcd.hi:req abcd.lo:req
 addb $(\constexpr)&0xff,\abcd.lo
 .if (\constexpr) != 0
 adcb $(\constexpr)>>8&0xff,\abcd.hi
 .endif
.endm
/ Subtracts 16-bit CONSTEXPR from Qw-register.
/ @mode long,legacy,real
.macro bbsub constexpr:req abcd.hi:req abcd.lo:req
 subb $(\constexpr)&0xff,\abcd.lo
 .if (\constexpr) != 0
 sbbb $(\constexpr)>>8&0xff,\abcd.hi
 .endif
.endm
/ Ands Qw-register with 16-bit CONSTEXPR.
/ @mode long,legacy,real
.macro bband constexpr:req abcd.hi:req abcd.lo:req
 .if ((\constexpr)&0xff) != 0xff || ((\constexpr)>>8&0xff) == 0xff
 andb $(\constexpr)&0xff,\abcd.lo
 .endif
 .if ((\constexpr)>>8&0xff) != 0xff
 andb $(\constexpr)>>8&0xff,\abcd.hi
 .endif
.endm
/ Ors Qw-register with 16-bit CONSTEXPR.
/ @mode long,legacy,real
.macro bbor constexpr:req abcd.hi:req abcd.lo:req
 .if ((\constexpr)&0xff) != 0 || ((\constexpr)>>8&0xff) != 0
 orb $(\constexpr)&0xff,\abcd.lo
 .endif
 .if ((\constexpr)>>8&0xff) != 0
 orb $(\constexpr)>>8&0xff,\abcd.hi
 .endif
.endm
/ Performs ACTION only if in real mode.
/ @mode long,legacy,real
.macro rlo clobber:req action:vararg
990: mov $0,\clobber
 .if .-990b!=3
 .error "bad clobber or assembler mode"
 .endif
991: \action
 .rept 2-(.-991b)
 nop
 .endr
 .if .-991b!=2
 .error "ACTION must be 1-2 bytes"
 .endif
.endm
/ Initializes real mode stack.
/ The most holiest of holy code.
/ @mode real
/ @see www.pcjs.org/pubs/pc/reference/intel/8086/
.macro rlstack seg:req addr:req
 cli
 mov \seg,%ss
 mov \addr,%sp
 sti
.endm
/ Symbolic Linker-Defined Binary Content.
.macro .stub name:req kind:req default type=@object
 .ifnb \default
  .equ \name,\default
 .endif
 .\kind \name
 .type \name,\type
 .weak \name
.hidden \name
.endm
/ Symbolic Linker-Defined Binary-Encoded-Bourne Content.
/ @param units is the number of encoded 32-bit values to insert,
/ e.g. \000 can be encoded as 0x3030305c.
.macro .shstub name:req units:req
 ss \name,0
 .if \units>1
  ss \name,1
   .if \units>2
    ss \name,2
    ss \name,3
    .if \units>4
     ss \name,4
     ss \name,5
     ss \name,6
     ss \name,7
   .endif
  .endif
 .endif
.endm
.macro ss name n
 .stub \name\()_bcs\n,long
.endm
