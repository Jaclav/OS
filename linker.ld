OUTPUT_FORMAT("binary")
/*ENTRY won't work because it sets function to be first in elf format, not in binary*/
SECTIONS {
    .text :
    {
        *(start);
        *(.text);
     }

    .data : { *(.data) }
    .bss : { *(.bss) }
    .rodata : { *(.rodata) }

    /DISCARD/ : {
        *(.comment);
        *(.eh_frame);
    }
}