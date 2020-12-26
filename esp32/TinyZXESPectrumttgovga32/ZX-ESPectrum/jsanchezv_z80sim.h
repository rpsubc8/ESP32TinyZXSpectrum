#ifndef Z80SIM_H
#define Z80SIM_H

#include "gbConfig.h"
#ifdef use_lib_core_jsanchezv

#include <iostream>
#include <fstream>

#include "jsanchezv_z80.h"
#include "jsanchezv_z80operations.h"

class Z80sim : public Z80operations
{
private:
    uint64_t tstates;
    Z80 cpu;
    //JJ uint8_t z80Ram[0x10000];
    //uint8_t z80Ports[0x10000];
    //JJ unsigned char z80Ram[0x10000];
    //JJ unsigned char z80Ports[0x10000];
    unsigned char z80Ram[0x10000];
    unsigned char z80Ports[0x4000]; //No hay memoria
    bool finish;

public:
    Z80sim(void);
    //JJ virtual ~Z80sim() override;
    virtual ~Z80sim();

    //JJ uint8_t fetchOpcode(uint16_t address) override;
    //JJ uint8_t peek8(uint16_t address) override;
    //JJ void poke8(uint16_t address, uint8_t value) override;
    //JJ uint16_t peek16(uint16_t address) override;
    //JJ void poke16(uint16_t address, RegisterPair word) override;
    //JJ uint8_t inPort(uint16_t port) override;
    //JJ void outPort(uint16_t port, uint8_t value) override;
    //JJ void addressOnBus(uint16_t address, int32_t tstates) override;
    //JJ void interruptHandlingTime(int32_t tstates) override;
    //JJ bool isActiveINT(void) override;
    
    uint8_t fetchOpcode(uint16_t address);
    uint8_t peek8(uint16_t address);
    void poke8(uint16_t address, uint8_t value);
    uint16_t peek16(uint16_t address);
    void poke16(uint16_t address, RegisterPair word);
    uint8_t inPort(uint16_t port);
    void outPort(uint16_t port, uint8_t value);
    void addressOnBus(uint16_t address, int32_t tstates);
    void interruptHandlingTime(int32_t tstates);
    bool isActiveINT(void);

#ifdef WITH_BREAKPOINT_SUPPORT
    //JJ uint8_t breakpoint(uint16_t address, uint8_t opcode) override;
    uint8_t breakpoint(uint16_t address, uint8_t opcode);
#else
    uint8_t breakpoint(uint16_t address, uint8_t opcode);
#endif

#ifdef WITH_EXEC_DONE
    //void execDone(void) override;
    void execDone(void);
#endif

    void runTest(std::ifstream* f);
    void runTestJJ(void);
    void runTestJJ_poll(void);
};
#endif // Z80SIM_H

#endif
