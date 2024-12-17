OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)
SECTIONS
{
    . = 1M;

	.multiboot ALIGN(4k) :
	{
		*(.multiboot)
	}

	.note.gnu.build-id ALIGN(4k) :
	{
		*(.note.gnu.build-id)
	}

	.boot ALIGN(4k) :
	{
		*(.boot)
	}

	/* higher half */

	. += 0xC0000000;

	.text ALIGN(4k) : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
	}

	.rodata ALIGN(4k) : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	.data ALIGN(4k) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}

	.bss ALIGN(4k) : AT(ADDR(.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
	}

	_kernel_end = .;
}