OUTPUT_FORMAT("binary")

SECTIONS {
    .text :
    {
        *(kernelMain);
        *(.text);
     }

    .data : { *(.data) }
    .bss : { *(.bss) }
    .rodata : { *(.rodata) }

    .note.GNU-stack :{ *(.note.GNU-stack) }
    .rel.text         : { *(.rel.text) }
    .comment          : { *(.comment) }
    .eh_frame         : { *(.eh_frame) }
    .rel.eh_frame     : { *(.rel.eh_frame) }
    .symtab           : { *(.symtab) }
    .strtab           : { *(.strtab) }
    .shstrtab         : { *(.shstrtab) }
    .relkernelMain    : { *(.relkernelMain) }
    .text : { *(endFun) }


    /DISCARD/ : {
        *(.comment);
    }
}