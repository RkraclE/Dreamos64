#include <vm.h>
#include <video.h>

void page_fault_handler(uint64_t error_code){
    _printStr("Welcome to #PF world - Not ready yet... \n");
    uint64_t cr2_content = 0;
    uint64_t pd;
    uint64_t pdpr;
    uint64_t pml4;  
    asm ("mov %%cr2, %0" : "=r" (cr2_content) );
    _printStringAndNumber("-- Error code value: ", error_code);
    _printStringAndNumber("--  Faulting address: ", cr2_content);
    cr2_content = cr2_content & VM_OFFSET_MASK;
    _printStringAndNumber("-- Address prepared for PD/PT extraction: ", cr2_content);
    pd = PD_ENTRY(cr2_content); 
    pdpr = PDPR_ENTRY(cr2_content);
    pml4 = PML4_ENTRY(cr2_content);
#if SMALL_PAGES == 1
    uint64_t pt;
    pt = PT_ENTRY(cr2_content);
#endif
    _printStringAndNumber("pd: ", pd);
    _printStringAndNumber("pdpr: ", pdpr);
    _printStringAndNumber("pml4: ", pml4);
    if(_should_allocate(cr2_content)){
        _printStringAndNumber("Should allocate address: ", cr2_content);
        
#if SMALL_PAGES == 1
        uint64_t *recursive_address = SIGN_EXTENSION | ENTRIES_TO_ADDRESS(510l, 510l, 510l, 510l);
        _printStringAndNumber("Value: ", recursive_address[pml4]);
#endif
    }
    asm("hlt");

}

bool _should_allocate(uint64_t address){
    return true;
}


void initialize_vm(){
    //This function will map essential part of the memory (FB, ACPI stuff)
#ifdef USE_FRAMEBUFFER

#endif
}
