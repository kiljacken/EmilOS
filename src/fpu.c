#include <common.h>
#include <cpuid.h>

void set_fpu_control_word(const uint16_t cw)
{
    if(cpuid_features() & CPUID_FEAT_EDX_FPU) // checks for the FPU flag
    {
        // FLDCW = Load FPU Control Word
        asm volatile("fldcw %0;"    // sets the FPU control word to "cw"
                     ::"m"(cw));
        return;
    }
    panic("NO FPU IN MACHINE!!");
}
 
void setup_x87_fpu()
{
   size_t cr4; // backup of CR4
   
   if(cpuid_features() & CPUID_FEAT_EDX_FPU) // checks for the FPU flag
   {
       // place CR4 into our variable
       __asm__ __volatile__("mov %%cr4, %0;" : "=r" (cr4));

       // set the OSFXSR bit
       cr4 |= 0x200;

       // reload CR4 and INIT the FPU (FINIT)
       __asm__ __volatile__("mov %0, %%cr4; finit;" : : "r"(cr4));

       // set the FPU Control Word
       set_fpu_control_word(0x37F);
       return;
    }
    panic("NO FPU IN MACHINE!!");
}
