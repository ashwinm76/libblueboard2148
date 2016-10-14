
C_STACK_SIZE   = 512;
IRQ_STACK_SIZE = 64;
FIQ_STACK_SIZE = 64;
SVC_STACK_SIZE = 64;
ABT_STACK_SIZE = 64;
UND_STACK_SIZE = 64;

INPUT(-lblueboard2148)
INPUT(-llpc2148)
INPUT(-lgcc)

ENTRY(_start)

MEMORY
{
  ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 0x7D000
  RAM (rwx) : ORIGIN = 0x40000000, LENGTH = 16K
}

SECTIONS
{
  CODE : {
    KEEP(*(.init))
    KEEP(*(.text.main))
    *(.text .text.*)
    *(.gnu.linkonce.t.*)
	*(.glue_7)
	*(.glue_7t)
	*(.gcc_except_table)
    *(.rodata)
    *(.rodata*)
	*(.gnu.linkonce.r.*)
    . = ALIGN(4);
  } > RAM

  DATA : {
    __data_load__ = LOADADDR(DATA);
    __data_start__ = .;
    *(.data)
    . = ALIGN(4);
    __data_end__ = .;
  } > RAM

  .bss : {
    __bss_start__ = . ;
    *(.bss)
    . = ALIGN(4);
    __bss_end__ = . ;
  } > RAM

    STACK : {
        __stack_start__ = . ;

        . += IRQ_STACK_SIZE;
        . = ALIGN (4);
        __irq_stack_top__ = . ;

        . += FIQ_STACK_SIZE;
        . = ALIGN (4);
        __fiq_stack_top__ = . ;

        . += SVC_STACK_SIZE;
        . = ALIGN (4);
        __svc_stack_top__ = . ;

        . += ABT_STACK_SIZE;
        . = ALIGN (4);
        __abt_stack_top__ = . ;

        . += UND_STACK_SIZE;
        . = ALIGN (4);
        __und_stack_top__ = . ;

        . += C_STACK_SIZE;
        . = ALIGN (4);
        __c_stack_top__ = . ;

        __stack_end__ = .;
    } >RAM

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}
