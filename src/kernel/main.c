/*
 * main.c 
 * Kernel entry point from bootloader
 * */

#include <multiboot.h>
#include <main.h>
#include <idt.h>
#include <kernel/video.h>
#include <kernel/io.h>
#include <kernel/qemu.h>
#include <kernel/framebuffer.h>
#include <kernel/psf.h>

struct multiboot_tag_framebuffer *tagfb;
struct multiboot_tag_basic_meminfo *tagmem;
extern char _binary_fonts_default_psf_size;
extern char _binary_fonts_default_psf_start;
extern char _binary_fonts_default_psf_end;

void _read_configuration_from_multiboot(unsigned long addr){
    struct multiboot_tag* tag;
    char number[30];
	for (tag=(struct multiboot_tag *) (addr + 8);
		tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
										+ ((tag->size + 7) & ~7))){

        switch(tag->type){
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                qemu_write_string("Found basic Mem Info type:");
                tagmem = (struct multiboot_tag_basic_meminfo *) tag;
                _getHexString(number, tagmem->type);
                _printStr(number);
                _printNewLine();
                _getHexString(number, tagmem->mem_lower);
                _printStr("Mem lower: ");
                _printStr(number);
                _printNewLine();
                _getHexString(number, tagmem->mem_upper);
                _printStr("Mem upper: ");
                _printStr(number);
                _printNewLine();
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                qemu_write_string("Found Multiboot framebuffer: ");
                _getHexString(number, tag->type);
                qemu_write_string(number);
                qemu_write_string("\n");
                tagfb = (struct multiboot_tag_framebuffer *) tag;
                qemu_write_string("---framebuffer-type: ");
                _getHexString(number, tagfb->common.framebuffer_type);
                qemu_write_string(number);
                qemu_write_string("\n");
                qemu_write_string("---framebuffer-width: ");
                _getHexString(number, tagfb->common.framebuffer_width);
                qemu_write_string(number);
                qemu_write_string("\n");
                qemu_write_string("---framebuffer-height: ");
                _getHexString(number, tagfb->common.framebuffer_height);
                qemu_write_string(number);
                qemu_write_string("\n");
                qemu_write_string("---framebuffer-address: ");
                _getHexString(number, tagfb->common.framebuffer_addr);
                qemu_write_string(number);
                qemu_write_string("\n");
                qemu_write_string("---framebuffer-bpp: ");
                _getHexString(number, tagfb->common.framebuffer_bpp);
                qemu_write_string(number);
                qemu_write_string("\n"); 
                qemu_write_string("---framebuffer-pitch: ");
                _getHexString(number, tagfb->common.framebuffer_pitch);
                qemu_write_string(number);
                qemu_write_string("\n"); 
                set_fb_data(tagfb);
                break;
        }

    }

}
void kernel_start(unsigned long addr, unsigned long magic){
    //struct multiboot_tag *tag;
    extern unsigned int end;
    struct multiboot_tag *tag = (struct multiboot_tag*) (addr+8);
    unsigned int log_enabled = qemu_init_debug();
    qemu_write_string("Hello qemu log\n");
    qemu_write_string("==============\n");

    init_idt();
    load_idt();
    _read_configuration_from_multiboot(addr);
    //test_image();
    //asm("int $0x0e");
    
    qemu_write_string("---Ok\n");
    //unsigned int *video_mem = (unsigned int*)0xb8020;
    //*video_mem = 0x024f;
    //_printCh('@', WHITE);
    //if(boot_info->flags == 0){
    //    _printCh('Y', GREEN);
    //} else {
    //    _printCh('N', GREEN);
    //}
    unsigned size = *(unsigned*)addr;
    char number[30];
    _printNewLine();
    _printStr("Size: ");
    _printNumber(number, size, 10);
    _printCh('@', WHITE);
    _printNewLine();
    _printStr(" Trying to write something \n Now with a new line");

    _printNumber(number, 300, 10);

    _printNewLine();
    unsigned int *val = (unsigned int *) 0x100000;
    _printStr("Magic: ");
    _printHex(number, *val);
    val++;
    _printNewLine();
    _printStr("Flags: ");
    _printHex(number, *val);
    _printNewLine();
    val++;  
    _printStr("Header Length: ");
    _printHex(number, *val); 
    _printStr(" OMT: ");
    _printStr(number);

    val++;  
    _printNewLine();
    _printStr("Checksum: ");
    _printHex(number, *val); 
    val++;
    _printNewLine();
    _printStr(" -- ");
    _printHex(number, *val);
    val++;
    _printNewLine();
    _printStr(" -- ");
    _printHex(number, *val);
    val++;
    _printNewLine();
    _printStr(" -- ");
    _printHex(number, *val);
    val++;
    _printNewLine();
    _printStr(" -- ");
    _printHex(number, *val);
    _printNewLine();
	_printStr("End: ");
	_printHex(number, (unsigned int)&end);
    _printNewLine();
	_printStr("Magic: ");
	_printHex(number, magic);
    _printNewLine();
	if(magic == 0x36d76289){
		_printStr("YEEEEH!!!");
	} else {
		_printStr("Ok i'm fucked");
	}
    _printNewLine();
    _printStr("Tag = type: ");
    qemu_write_string("Tag = type: ");
    _printNumber(number, tag->type, 10);
    qemu_write_string(number);
    _printStr(" - size: ");
    qemu_write_string(" - size: ");
    _printNumber(number, tag->size, 10);
    qemu_write_string(number);
    qemu_write_string("\n");
	_printNewLine();
	for (tag = (struct multiboot_tag *) (addr + 8);
		tag->type != MULTIBOOT_TAG_TYPE_END;
		tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
										+ ((tag->size + 7) & ~7)))
		{
			qemu_write_string("Tag 0x");
			_getHexString(number, tag->type);
			qemu_write_string(number);
			qemu_write_string("\n");
			qemu_write_string(" Size 0x");
			_getHexString(number, tag->size);
			qemu_write_string(number);
			qemu_write_string("\n");
		}

    PSF_font *font = (PSF_font*)&_binary_fonts_default_psf_start;
    _getHexString(number, font->magic);
    _printStr("Magic: "); 
    _printStr(number);
    _printNewLine();
    _getHexString(number, font->numglyph);
    _printStr("Number of glyphs: "); 
    _printStr(number);
    _printNewLine();
    _getHexString(number, font->headersize);
    _printStr("Header size: "); 
    _printStr(number);
    _printNewLine();
    _getHexString(number, font->bytesperglyph);
    _printStr("Bytes per glyphs: "); 
    _printStr(number);
    _printNewLine();
    _getHexString(number, font->flags);
    _printStr("Flags: "); 
    _printStr(number);
    _printNewLine();
    _getHexString(number, font->version);
    _printStr("Version: "); 
    _printStr(number);
    _printNewLine();
     _getHexString(number, font->width);
    _printStr("Width: "); 
    _printStr(number);
    _printNewLine();
   _getHexString(number, font->height);
    _printStr("Height: "); 
    _printStr(number);
    _printNewLine();
    #if USE_FRAMEBUFFER == 1 
    _fb_putchar('C', 1, 1, 0x000000, 0xFFFFFF);
    _fb_putchar('i', 2, 1, 0x000000, 0xFFFFFF);
    _fb_putchar('a', 3, 1, 0x000000, 0xFFFFFF);
    _fb_putchar('o', 4, 1, 0x000000, 0xFFFFFF);
    _fb_putchar('!', 5, 1, 0x000000, 0xFFFFFF);
    _fb_printStr("Dreamos64", 0, 0, 0xFFFFFF, 0x3333ff);
    if(get_PSF_version(&_binary_fonts_default_psf_start) == 1){
        qemu_write_string("PSF v1 found\n");
    }  else {
        qemu_write_string("PSF v2 found\n");
    }
    _getHexString(number, sizeof(PSFv1_Font));
    qemu_write_string(number);
    #endif
    asm("hlt");
}
