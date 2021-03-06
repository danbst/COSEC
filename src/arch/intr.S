#define NOT_CC

#define KERN_DS     0x0010
#define KERN_CS     0x0008
/*
 *      This file contains most of assembly routines used, most of them
 *  are interrupts and exections entry points now.
 */

/*
 *    <---- stack grows to less addresses --
 *  Stack state during an interrupt:
 *     args    fs  gs  es  ds edi esi ebp esp ebx edx ecx eax
 *  -|-------|---|---|---|---|---|---|---|---|---|---|---|---| sys-info
 *           ^
 *      context_esp
 *
 *  sys-info:
 *  1) interrupted from kernel mode:
 *      [|---]|---|---|---|---
 *       [err] eip  cs efl
 *  2) interrupted from user mode:
 *      [|---]|---|---|---|---|---|----
         [err] eip  cs efl esp  ss
 */

.data
/* temporary 48bit structure for IDT/GDT loading */
dtreg:
.word 0
.long 0

/* points to a stack position before context data */
context_esp:
.long 0

/* last exception error code */
intr_error:
.long 0

#if INTR_PROFILING
start_tick:
.long   0
.long   0
#endif


/********************** text *****************************/
.text
.align 4

/******** Tables-related *************/

.macro DTREG_LOAD
    movw 4(%esp), %ax
    movw %ax, dtreg
    movl 8(%esp), %eax
    movl %eax, dtreg + 2
.endm

.macro SHAKE_REGS
    movw $KERN_DS, %dx
    movw %dx, %ds
    movw %dx, %es
    movw %dx, %fs
    movw %dx, %gs
.endm


.global gdt_load

gdt_load:
    DTREG_LOAD
    lgdt dtreg

    SHAKE_REGS
    ljmp $KERN_CS, $reload_gdt
reload_gdt:
    ret

.global gdt_get
gdt_get:
    movl 4(%esp), %esi
    sgdt dtreg
    movw dtreg, %ax
    movw %ax, (%esi)
    movl dtreg+2, %eax
    movl %eax, 2(%esi)
    ret

.global idt_load
idt_load:
    DTREG_LOAD
    lidt dtreg
    ret

.global intr_context_esp
intr_context_esp:
    movl context_esp, %eax
    ret

.global intr_set_context_esp
intr_set_context_esp:
    movl 4(%esp), %eax
    movl %eax, context_esp
    ret

.global intr_err_code
intr_err_code:
    movl intr_error, %eax
    ret


#if INTR_PROFILING

.global intr_start_rdtsc
intr_start_rdtsc:
    movl 4(%esp), %ebx
    movl start_tick + 4, %eax
    movl %eax, 4(%ebx)
    movl start_tick, %eax
    movl %eax, (%ebx)
    ret

.extern intr_profile

.macro INTR_PROFILING_START
    rdtsc
    movl %eax, start_tick
    movl %edx, start_tick + 4
.endm

.macro INTR_PROFILING_END
    rdtsc
    movl %edx, 4(%esp)
    movl %eax, (%esp)
    call intr_profile
.endm
#else
#   define INTR_PROFILING_START
#   define INTR_PROFILING_END
#endif


/********** ENTRY macros *************/

.macro INTR_PROLOG
    /* eflags already saved by CPU as (uint)(context_esp[2]) */
    cli
    pusha
    pushl %ds
    pushl %es
    pushl %gs
    pushl %fs

    SHAKE_REGS

    /* save an interrupt's stack context pointer:
        here is the point of return to a task */
    movl %esp, context_esp

    /* for arguments */
    subl $8, %esp
.endm

.macro INTR_END
    movl context_esp, %esp
    popl %fs
    popl %gs
    popl %es
    popl %ds
    popa
.endm

.macro ENTRY_NOERR name, handler
.extern \handler
.global \name
\name :
    INTR_PROLOG

    movl %esp, (%esp)
    call \handler
    jmp noerr_return
.endm

.macro ENTRY_ERR name, handler
.extern \handler
.global \name
\name :
    INTR_PROLOG

    movl 0x38(%esp), %ebx
    movl %ebx, intr_error

    movl %esp, (%esp)
    call \handler
    jmp err_return
.endm

.macro ENTRY_IRQ name, num
.global \name
\name :
    INTR_PROLOG
    INTR_PROFILING_START

    /* call the handler */
    movl \num, (%esp)
    jmp irq_return
.endm

noerr_return:
    INTR_END
    iret

err_return:
    INTR_END
    addl $4, %esp
    iret

irq_return:
    call irq_handler

    INTR_PROFILING_END
    INTR_END
    iret

/************ Handlers ***************/
.extern int_handlers_table

.extern int_syscall
.extern int_dummy
.extern int_odd_exception
.extern irq_stub

/******** multiple entries ***********/

ENTRY_NOERR dummyentry,     int_dummy           /* unrecognized interrupt */
ENTRY_NOERR syscallentry,   int_syscall         /* sys int            */
ENTRY_NOERR isr14to1F,      int_odd_exception   /* unused exceptions  */

/************* exceptions ************/
ENTRY_NOERR isr00, int_division_by_zero
ENTRY_NOERR isr01, int_odd_exception
ENTRY_NOERR isr02, int_nonmaskable
ENTRY_NOERR isr03, int_breakpoint
ENTRY_NOERR isr04, int_overflow
ENTRY_NOERR isr05, int_out_of_bounds
ENTRY_NOERR isr06, int_invalid_op
ENTRY_NOERR isr07, int_odd_exception
ENTRY_ERR   isr08, int_double_fault
ENTRY_NOERR isr09, int_odd_exception
ENTRY_ERR   isr0A, int_invalid_tss
ENTRY_ERR   isr0B, int_segment_not_present
ENTRY_ERR   isr0C, int_stack_segment
ENTRY_ERR   isr0D, int_gpf
ENTRY_ERR   isr0E, int_page_fault
ENTRY_NOERR isr0F, int_odd_exception
ENTRY_NOERR isr10, int_odd_exception
ENTRY_NOERR isr11, int_odd_exception
ENTRY_NOERR isr12, int_odd_exception
ENTRY_NOERR isr13, int_odd_exception

/************ IRQS  **************/
.extern irq_handler
ENTRY_IRQ   irq00, $0x0
ENTRY_IRQ   irq01, $0x1
ENTRY_IRQ   irq02, $0x2
ENTRY_IRQ   irq03, $0x3
ENTRY_IRQ   irq04, $0x4
ENTRY_IRQ   irq05, $0x5
ENTRY_IRQ   irq06, $0x6
ENTRY_IRQ   irq07, $0x7
ENTRY_IRQ   irq08, $0x8
ENTRY_IRQ   irq09, $0x9
ENTRY_IRQ   irq0A, $0xA
ENTRY_IRQ   irq0B, $0xB
ENTRY_IRQ   irq0C, $0xC
ENTRY_IRQ   irq0D, $0xD
ENTRY_IRQ   irq0E, $0xE
ENTRY_IRQ   irq0F, $0xF

