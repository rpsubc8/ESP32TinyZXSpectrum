#ifndef Z80OPERATIONS_H
#define Z80OPERATIONS_H

#include "gbConfig.h"
#ifdef use_lib_core_jsanchezv

#include <stdint.h>

class Z80operations {
public:
    Z80operations(void) {};

    virtual ~Z80operations() {};

    /* Read opcode from RAM */
    virtual uint8_t fetchOpcode(uint16_t address) = 0;

    /* Read/Write byte from/to RAM */
    virtual uint8_t peek8(uint16_t address) = 0;
    virtual void poke8(uint16_t address, uint8_t value) = 0;

    /* Read/Write word from/to RAM */
   virtual uint16_t peek16(uint16_t adddress) = 0;
   virtual void poke16(uint16_t address, RegisterPair word) = 0;

    /* In/Out byte from/to IO Bus */
    virtual uint8_t inPort(uint16_t port) = 0;
    virtual void outPort(uint16_t port, uint8_t value) = 0;

    /* Put an address on bus lasting 'tstates' cycles */
    virtual void addressOnBus(uint16_t address, int32_t wstates) = 0;

    /* Clocks needed for processing INT and NMI */
    virtual void interruptHandlingTime(int32_t wstates) = 0;

    /* Callback to know when the INT signal is active */
    virtual bool isActiveINT(void) = 0;

#ifdef WITH_BREAKPOINT_SUPPORT
    /* Callback for notify at PC address */
    virtual uint8_t breakpoint(uint16_t address, uint8_t opcode) = 0;
#endif

#ifdef WITH_EXEC_DONE
    /* Callback to notify that one instruction has ended */
    virtual void execDone(void) = 0;
#endif
};

#endif // Z80OPERATIONS_H

#endif
