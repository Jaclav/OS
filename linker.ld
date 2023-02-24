OUTPUT_FORMAT("binary")

ENTRY(main);

SECTIONS {
    .text.start (main) : {
        "bin/kernel.o"( .text )
    }

    .data : { *(.data) }
    .rdata : { *(.rdata) }
    .bss : { *(.bss) }

    /DISCARD/ : {
        *(.eh_frame);
        *(.comment);
    }
}
