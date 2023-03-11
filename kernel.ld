OUTPUT_FORMAT("binary")
/*ENTRY won't work because it sets function to be first in elf format, not in binary*/
SECTIONS {
    .text 0x200:
    {
        *(start);
        *(.text);
     }

    .rodata : { *(.rodata) }
    .data : { *(.data) }
    .bss :
    {
        *(COMMON)/*https://stackoverflow.com/questions/28474675/arm-common-section-and-fno-common-flag*/
        *(.bss)
    }

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
    }
}