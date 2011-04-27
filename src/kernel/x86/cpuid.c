#include <types.h>
#include <x86/cpuid.h>

uint32_t cpuid_features()
{
  uint32_t eax, ebx, ecx, edx;
  cpuid( 1, eax, ebx, ecx, edx );
  return edx;
}

uint32_t cpuid_extended_features()
{
  uint32_t eax, ebx, ecx, edx;
  cpuid( 1, eax, ebx, ecx, edx );
  return ecx;
}
