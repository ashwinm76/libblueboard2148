
.equ    I_BIT,          0x80
.equ    F_BIT,          0x40

.equ    USR_MODE,       0x10
.equ    FIQ_MODE,       0x11
.equ    IRQ_MODE,       0x12
.equ    SVC_MODE,       0x13
.equ    ABT_MODE,       0x17
.equ    UND_MODE,       0x1B
.equ    SYS_MODE,       0x1F

.text
.code 32
.section .init
.global _start
.extern vic_irq_handler

/*  Vector Table */
    ldr  pc, reset_handler_address
    ldr  pc, undef_handler_address
    ldr  pc, swi_handler_address
    ldr  pc, pabort_handler_address
    ldr  pc, dabort_handler_address
    .word  0xB8A06F58                  /* 0 - (sum of other vectors instructions) */
    ldr  pc, irq_handler_address
    ldr  pc, fiq_handler_address

reset_handler_address:   .word  reset_handler
undef_handler_address:   .word  undef_handler
swi_handler_address:     .word  swi_handler
pabort_handler_address:  .word  pabort_handler
dabort_handler_address:  .word  dabort_handler
                         .word   0x00
irq_handler_address:     .word  irq_handler
fiq_handler_address:     .word  fiq_handler

reset_handler:
        b  _start
undef_handler:
        b  undef_handler
swi_handler:
        b  swi_handler
pabort_handler:
        b  pabort_handler
dabort_handler:
        b  dabort_handler
irq_handler:
		stmfd sp!,{r0-r12,lr}
        bl vic_irq_handler
        ldmfd sp!,{r0-r12,lr}
        subs pc,lr,#4
fiq_handler:
        b  fiq_handler

.align 4                               /* re-align to the word boundary */
    
_start:
    /* Relocate the .data section (copy from ROM to RAM) */
    LDR     r0,=__data_load__
    LDR     r1,=__data_start__
    LDR     r2,=__data_end__
1:
    CMP     r1,r2
    LDMLTIA r0!,{r3}
    STMLTIA r1!,{r3}
    BLT     1b


    /* Clear the .bss section */
    LDR     r1,=__bss_start__
    LDR     r2,=__bss_end__
    MOV     r3,#0
1:
    CMP     r1,r2
    STMLTIA r1!,{r3}
    BLT     1b
    
    /* Initialize stack pointers for all ARM modes, keeping interrupts disabled. */
    MSR     CPSR_c,#(IRQ_MODE | I_BIT | F_BIT)
    LDR     sp,=__irq_stack_top__              /* set the IRQ stack pointer */

    MSR     CPSR_c,#(FIQ_MODE | I_BIT | F_BIT)
    LDR     sp,=__fiq_stack_top__              /* set the FIQ stack pointer */

    MSR     CPSR_c,#(SVC_MODE | I_BIT | F_BIT)
    LDR     sp,=__svc_stack_top__              /* set the SVC stack pointer */

    MSR     CPSR_c,#(ABT_MODE | I_BIT | F_BIT)
    LDR     sp,=__abt_stack_top__              /* set the ABT stack pointer */

    MSR     CPSR_c,#(UND_MODE | I_BIT | F_BIT)
    LDR     sp,=__und_stack_top__              /* set the UND stack pointer */

    MSR     CPSR_c,#(SYS_MODE | I_BIT | F_BIT)
    LDR     sp,=__c_stack_top__                /* set the C stack pointer */

    /* Call C main() */
    LDR     r12,=main
    MOV     lr,pc           
    BX      r12             

    SWI     0xFFFFFF        /* cause exception if main() ever returns */

.end
 
