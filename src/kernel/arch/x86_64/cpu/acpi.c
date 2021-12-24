#include <cpu.h>
#include <acpi.h>
#include <video.h>
#include <framebuffer.h>
#include <stdio.h>

void parse_RSDT(RSDPDescriptor *descriptor){
    printf("Parse RSDP Descriptor\n");
    RSDT_item *root = (RSDT_item *) descriptor->RsdtAddress;
    printf("descriptor Address: 0x%x\n", descriptor->RsdtAddress);
    ACPISDTHeader header = root->header;
    printf("RSDT_Signature: %.4s\n", header.Signature);
//    _printStr(header.Signature);
    _printStr("--- ");
//    _fb_putchar(header.Signature[0], 1, 3, 0x000000, 0xFFFFFF);

//    printf("RSDT Address: 0x%x", root->header);
    //#if USE_FRAMEBUFFER == 1*/
    //_fb_putchar(header.Signature[0], 1, 3, 0x000000, 0xFFFFFF);
//    _fb_putchar(header.Signature[1], 2, 3, 0x000000, 0xFFFFFF);
//    _fb_putchar(header.Signature[2], 3, 3, 0x000000, 0xFFFFFF);
//    _fb_putchar(header.Signature[3], 4, 3, 0x000000, 0xFFFFFF);
/*    _fb_printStr(header.Signature, 0, 3, 0x000000, 0xFFFFFF);
    #endif
    _printCh(header.Signature[0], WHITE);
    _printCh(header.Signature[1], WHITE);
    _printCh(header.Signature[2], WHITE);
    _printCh(header.Signature[3], WHITE);
   _printNewLine();*/
    
}

void parse_RSDTv2(RSDPDescriptor20 *descriptor){
    printf("Parse RSDP v2 Descriptor");
}

int validate_RSDP(RSDPDescriptor *descriptor){
    uint8_t sum = 0;
    char number[30];

    for (uint32_t i=0; i < sizeof(RSDPDescriptor); i++){
        sum += ((char*) descriptor)[i];
        _getHexString(number, sum);
        _printStr(number);
        _printStr(" ");
    }
    _printNewLine();
    _getHexString(number, sum);
    _printStr("Checksum of RSDP is: ");
    _printStr(number);
    _printNewLine();
    return sum == 0;
}
