// Converted to C++ from Java at
//... https://github.com/jsanchezv/Z80Core
//... commit c4f267e3564fa89bd88fd2d1d322f4d6b0069dbd
//... GPL 3
//... v1.0.0 (13/02/2017)
//    quick & dirty conversion by dddddd (AKA deesix)

//... compile with $ g++ -m32 -std=c++14
//... put the zen*bin files in the same directory.
#ifndef Z80CPP_H
#define Z80CPP_H

#include "gbConfig.h"
#ifdef use_lib_core_jsanchezv

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Union allowing a register pair to be accessed as bytes or as a word */
typedef union {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    struct {
        uint8_t hi, lo;
    } byte8;
#else
    struct {
        uint8_t lo, hi;
    } byte8;
#endif
    uint16_t word;
} RegisterPair;

#include "jsanchezv_z80operations.h"

#define REG_B   regBC.byte8.hi
#define REG_C   regBC.byte8.lo
#define REG_BC  regBC.word
#define REG_Bx  regBCx.byte8.hi
#define REG_Cx  regBCx.byte8.lo
#define REG_BCx regBCx.word

#define REG_D   regDE.byte8.hi
#define REG_E   regDE.byte8.lo
#define REG_DE  regDE.word
#define REG_Dx  regDEx.byte8.hi
#define REG_Ex  regDEx.byte8.lo
#define REG_DEx regDEx.word

#define REG_H   regHL.byte8.hi
#define REG_L   regHL.byte8.lo
#define REG_HL  regHL.word
#define REG_Hx  regHLx.byte8.hi
#define REG_Lx  regHLx.byte8.lo
#define REG_HLx regHLx.word

#define REG_IXh regIX.byte8.hi
#define REG_IXl regIX.byte8.lo
#define REG_IX  regIX.word

#define REG_IYh regIY.byte8.hi
#define REG_IYl regIY.byte8.lo
#define REG_IY  regIY.word

#define REG_Ax  regAFx.byte8.hi
#define REG_Fx  regAFx.byte8.lo
#define REG_AFx regAFx.word

#define REG_PCh regPC.byte8.hi
#define REG_PCl regPC.byte8.lo
#define REG_PC  regPC.word

#define REG_S   regSP.byte8.hi
#define REG_P   regSP.byte8.lo
#define REG_SP  regSP.word

#define REG_W   memptr.byte8.hi
#define REG_Z   memptr.byte8.lo
#define REG_WZ  memptr.word

using namespace std;

void zx_reset_jsanchezv(void);

class Z80 {
public:
    // Modos de interrupci�n
    enum IntMode {
        IM0, IM1, IM2
    };
private:
    Z80operations *Z80opsImpl;
    // C�digo de instrucci�n a ejecutar
    // Poner esta variable como local produce peor rendimiento
    // ZEXALL test: (local) 1:54 vs 1:47 (visitante)
    //JJ uint8_t opCode;
    unsigned char opCode;
    // Se est� ejecutando una instrucci�n prefijada con DD, ED o FD
    // Los valores permitidos son [0x00, 0xDD, 0xED, 0xFD]
    // El prefijo 0xCB queda al margen porque, detr�s de 0xCB, siempre
    // viene un c�digo de instrucci�n v�lido, tanto si delante va un
    // 0xDD o 0xFD como si no.
    //uint8_t prefixOpcode = { 0x00 };    
    unsigned char prefixOpcode;    
    // Subsistema de notificaciones
    bool execDone;
    // Posiciones de los flags
    const static uint8_t CARRY_MASK = 0x01;
    const static uint8_t ADDSUB_MASK = 0x02;
    const static uint8_t PARITY_MASK = 0x04;
    const static uint8_t OVERFLOW_MASK = 0x04; // alias de PARITY_MASK
    const static uint8_t BIT3_MASK = 0x08;
    const static uint8_t HALFCARRY_MASK = 0x10;
    const static uint8_t BIT5_MASK = 0x20;
    const static uint8_t ZERO_MASK = 0x40;
    const static uint8_t SIGN_MASK = 0x80;
    // M�scaras de conveniencia
    const static uint8_t FLAG_53_MASK = BIT5_MASK | BIT3_MASK;
    const static uint8_t FLAG_SZ_MASK = SIGN_MASK | ZERO_MASK;
    const static uint8_t FLAG_SZHN_MASK = FLAG_SZ_MASK | HALFCARRY_MASK | ADDSUB_MASK;
    const static uint8_t FLAG_SZP_MASK = FLAG_SZ_MASK | PARITY_MASK;
    const static uint8_t FLAG_SZHP_MASK = FLAG_SZP_MASK | HALFCARRY_MASK;
    // Acumulador y resto de registros de 8 bits
    uint8_t regA;
    // Flags sIGN, zERO, 5, hALFCARRY, 3, pARITY y ADDSUB (n)
    uint8_t sz5h3pnFlags;
    // El flag Carry es el �nico que se trata aparte
    bool carryFlag;
    // Registros principales y alternativos
    RegisterPair regBC, regBCx, regDE, regDEx, regHL, regHLx;
    /* Flags para indicar la modificaci�n del registro F en la instrucci�n actual
     * y en la anterior.
     * Son necesarios para emular el comportamiento de los bits 3 y 5 del
     * registro F con las instrucciones CCF/SCF.
     *
     * http://www.worldofspectrum.org/forums/showthread.php?t=41834
     * http://www.worldofspectrum.org/forums/showthread.php?t=41704
     *
     * Thanks to Patrik Rak for his tests and investigations.
     */
    bool flagQ, lastFlagQ;

    // Acumulador alternativo y flags -- 8 bits
    RegisterPair regAFx;

    // Registros de prop�sito espec�fico
    // *PC -- Program Counter -- 16 bits*
    RegisterPair regPC;
    // *IX -- Registro de �ndice -- 16 bits*
    RegisterPair regIX;
    // *IY -- Registro de �ndice -- 16 bits*
    RegisterPair regIY;
    // *SP -- Stack Pointer -- 16 bits*
    RegisterPair regSP;
    // *I -- Vector de interrupci�n -- 8 bits*
    uint8_t regI;
    // *R -- Refresco de memoria -- 7 bits*
    uint8_t regR;
    // *R7 -- Refresco de memoria -- 1 bit* (bit superior de R)
    bool regRbit7;
    //Flip-flops de interrupci�n
    //JJbool ffIFF1 = false;
    //JJ bool ffIFF2 = false;
    bool ffIFF1;
    bool ffIFF2;
    // EI solo habilita las interrupciones DESPUES de ejecutar la
    // siguiente instrucci�n (excepto si la siguiente instrucci�n es un EI...)
    //JJ bool pendingEI = false;
    bool pendingEI;
    // Estado de la l�nea NMI
    //JJ bool activeNMI = false;
    bool activeNMI;
    // Modo de interrupci�n
    //JJ IntMode modeINT = IntMode::IM0;
    IntMode modeINT;
    // halted == true cuando la CPU est� ejecutando un HALT (28/03/2010)
    //JJ bool halted = false;
    bool halted;
    // pinReset == true, se ha producido un reset a trav�s de la patilla
    //JJ bool pinReset = false;
    bool pinReset;
    /*
     * Registro interno que usa la CPU de la siguiente forma
     *
     * ADD HL,xx      = Valor del registro H antes de la suma
     * LD r,(IX/IY+d) = Byte superior de la suma de IX/IY+d
     * JR d           = Byte superior de la direcci�n de destino del salto
     *
     * 04/12/2008     No se vayan todav�a, a�n hay m�s. Con lo que se ha
     *                implementado hasta ahora parece que funciona. El resto de
     *                la historia est� contada en:
     *                http://zx.pk.ru/attachment.php?attachmentid=2989
     *
     * 25/09/2009     Se ha completado la emulaci�n de MEMPTR. A se�alar que
     *                no se puede comprobar si MEMPTR se ha emulado bien hasta
     *                que no se emula el comportamiento del registro en las
     *                instrucciones CPI y CPD. Sin ello, todos los tests de
     *                z80tests.tap fallar�n aunque se haya emulado bien al
     *                registro en TODAS las otras instrucciones.
     *                Shit yourself, little parrot.
     */

    RegisterPair memptr;
    // I and R registers
    inline RegisterPair getPairIR(void);

    /* Algunos flags se precalculan para un tratamiento m�s r�pido
     * Concretamente, SIGN, ZERO, los bits 3, 5, PARITY y ADDSUB:
     * sz53n_addTable tiene el ADDSUB flag a 0 y paridad sin calcular
     * sz53pn_addTable tiene el ADDSUB flag a 0 y paridad calculada
     * sz53n_subTable tiene el ADDSUB flag a 1 y paridad sin calcular
     * sz53pn_subTable tiene el ADDSUB flag a 1 y paridad calculada
     * El resto de bits est�n a 0 en las cuatro tablas lo que es
     * importante para muchas operaciones que ponen ciertos flags a 0 por real
     * decreto. Si lo ponen a 1 por el mismo m�todo basta con hacer un OR con
     * la m�scara correspondiente.
     */
    //JJ uint8_t sz53n_addTable[256] = {};
    //JJ uint8_t sz53pn_addTable[256] = {};
    //JJ uint8_t sz53n_subTable[256] = {};
    //JJ uint8_t sz53pn_subTable[256] = {};
    unsigned char sz53n_addTable[256];
    unsigned char sz53pn_addTable[256];
    unsigned char sz53n_subTable[256];
    unsigned char sz53pn_subTable[256];

    // Un true en una direcci�n indica que se debe notificar que se va a
    // ejecutar la instrucci�n que est� en esa direci�n.
#ifdef WITH_BREAKPOINT_SUPPORT
    //JJ bool breakpointEnabled {false};
    bool breakpointEnabled;
#endif
    void copyToRegister(uint8_t opCode, uint8_t value);

public:
    // Constructor de la clase
    Z80(void)
    {
     prefixOpcode = 0x00;
     ffIFF1 = false;
     ffIFF2 = false;
     pendingEI = false;
     activeNMI = false;
     modeINT = IM0; // IntIntMode::IM0;
     halted = false;
     pinReset = false;
     memset(sz53n_addTable,0,256);
     memset(sz53pn_addTable,0,256);
     memset(sz53n_subTable,0,256);
     memset(sz53pn_subTable,0,256); 
     #ifdef WITH_BREAKPOINT_SUPPORT
      breakpointEnabled = false;
     #endif 
     printf("contructor Z80 defecto\n");                     
    }
    Z80(Z80operations *ops);
    ~Z80(void);

    // Acceso a registros de 8 bits
    // Access to 8-bit registers
    uint8_t getRegA(void) const { return regA; }
    void setRegA(uint8_t value) { regA = value; }

    uint8_t getRegB(void) const { return REG_B; }
    void setRegB(uint8_t value) { REG_B = value; }

    uint8_t getRegC(void) const { return REG_C; }
    void setRegC(uint8_t value) { REG_C = value; }

    uint8_t getRegD(void) const { return REG_D; }
    void setRegD(uint8_t value) { REG_D = value; }

    uint8_t getRegE(void) const { return REG_E; }
    void setRegE(uint8_t value) { REG_E = value; }

    uint8_t getRegH(void) const { return REG_H; }
    void setRegH(uint8_t value) { REG_H = value; }

    uint8_t getRegL(void) const { return REG_L; }
    void setRegL(uint8_t value) { REG_L = value; }

    // Acceso a registros alternativos de 8 bits
    // Access to alternate 8-bit registers
    uint8_t getRegAx(void) const { return REG_Ax; }
    void setRegAx(uint8_t value) { REG_Ax = value; }

    uint8_t getRegFx(void) const { return REG_Fx; }
    void setRegFx(uint8_t value) { REG_Fx = value; }

    uint8_t getRegBx(void) const { return REG_Bx; }
    void setRegBx(uint8_t value) { REG_Bx = value; }

    uint8_t getRegCx(void) const { return REG_Cx; }
    void setRegCx(uint8_t value) { REG_Cx = value; }

    uint8_t getRegDx(void) const { return REG_Dx; }
    void setRegDx(uint8_t value) { REG_Dx = value; }

    uint8_t getRegEx(void) const { return REG_Ex; }
    void setRegEx(uint8_t value) { REG_Ex = value; }

    uint8_t getRegHx(void) const { return REG_Hx; }
    void setRegHx(uint8_t value) { REG_Hx = value; }

    uint8_t getRegLx(void) const { return REG_Lx; }
    void setRegLx(uint8_t value) { REG_Lx = value; }

    // Acceso a registros de 16 bits
    // Access to registers pairs
    uint16_t getRegAF(void) const { return (regA << 8) | (carryFlag ? sz5h3pnFlags | CARRY_MASK : sz5h3pnFlags); }
    void setRegAF(uint16_t word) { regA = word >> 8; sz5h3pnFlags = word & 0xfe; carryFlag = (word & CARRY_MASK) != 0; }

    uint16_t getRegAFx(void) const { return REG_AFx; }
    void setRegAFx(uint16_t word) { REG_AFx = word; }

    uint16_t getRegBC(void) const { return REG_BC; }
    void setRegBC(uint16_t word) { REG_BC = word; }

    uint16_t getRegBCx(void) const { return REG_BCx; }
    void setRegBCx(uint16_t word) { REG_BCx = word; }

    uint16_t getRegDE(void) const { return REG_DE; }
    void setRegDE(uint16_t word) { REG_DE = word; }

    uint16_t getRegDEx(void) const { return REG_DEx; }
    void setRegDEx(uint16_t word) { REG_DEx = word; }

    uint16_t getRegHL(void) const { return REG_HL; }
    void setRegHL(uint16_t word) { REG_HL = word; }

    uint16_t getRegHLx(void) const { return REG_HLx; }
    void setRegHLx(uint16_t word) { REG_HLx = word; }

    // Acceso a registros de prop�sito espec�fico
    // Access to special purpose registers
    uint16_t getRegPC(void) const { return REG_PC; }
    void setRegPC(uint16_t address) { REG_PC = address; }

    uint16_t getRegSP(void) const { return REG_SP; }
    void setRegSP(uint16_t word) { REG_SP = word; }

    uint16_t getRegIX(void) const { return REG_IX; }
    void setRegIX(uint16_t word) { REG_IX = word; }

    uint16_t getRegIY(void) const { return REG_IY; }
    void setRegIY(uint16_t word) { REG_IY = word; }

    uint8_t getRegI(void) const { return regI; }
    void setRegI(uint8_t value) { regI = value; }

    uint8_t getRegR(void) const { return regRbit7 ? regR | SIGN_MASK : regR & 0x7f; }
    void setRegR(uint8_t value) { regR = value & 0x7f; regRbit7 = (value > 0x7f); }

    // Acceso al registro oculto MEMPTR
    // Hidden register MEMPTR (known as WZ at Zilog doc?)
    uint16_t getMemPtr(void) const { return REG_WZ; }
    void setMemPtr(uint16_t word) { REG_WZ = word; }

    // Acceso a los flags uno a uno
    // Access to single flags from F register
    bool isCarryFlag(void) const { return carryFlag; }
    void setCarryFlag(bool state) { carryFlag = state; }

    bool isAddSubFlag(void) const { return (sz5h3pnFlags & ADDSUB_MASK) != 0; }
    void setAddSubFlag(bool state);

    bool isParOverFlag(void) const { return (sz5h3pnFlags & PARITY_MASK) != 0; }
    void setParOverFlag(bool state);

    /* Undocumented flag */
    bool isBit3Flag(void) const { return (sz5h3pnFlags & BIT3_MASK) != 0; }
    void setBit3Fag(bool state);

    bool isHalfCarryFlag(void) const { return (sz5h3pnFlags & HALFCARRY_MASK) != 0; }
    void setHalfCarryFlag(bool state);

    /* Undocumented flag */
    bool isBit5Flag(void) const { return (sz5h3pnFlags & BIT5_MASK) != 0; }
    void setBit5Flag(bool state);

    bool isZeroFlag(void) const { return (sz5h3pnFlags & ZERO_MASK) != 0; }
    void setZeroFlag(bool state);

    bool isSignFlag(void) const { return sz5h3pnFlags >= SIGN_MASK; }
    void setSignFlag(bool state);

    // Acceso a los flags F
    // Access to F register
    uint8_t getFlags(void) const { return carryFlag ? sz5h3pnFlags | CARRY_MASK : sz5h3pnFlags; }
    void setFlags(uint8_t regF) { sz5h3pnFlags = regF & 0xfe; carryFlag = (regF & CARRY_MASK) != 0; }

    // Acceso a los flip-flops de interrupci�n
    // Interrupt flip-flops
    bool isIFF1(void) const { return ffIFF1; }
    void setIFF1(bool state) { ffIFF1 = state; }

    bool isIFF2(void) const { return ffIFF2; }
    void setIFF2(bool state) { ffIFF2 = state; }

    bool isNMI(void) const { return activeNMI; }
    void setNMI(bool nmi) { activeNMI = nmi; }

    // /NMI is negative level triggered.
    void triggerNMI(void) { activeNMI = true; }

    //Acceso al modo de interrupci�n
    // Maskable interrupt mode
    IntMode getIM(void) const { return modeINT; }
    void setIM(IntMode mode) { modeINT = mode; }

    bool isHalted(void) const { return halted; }
    void setHalted(bool state) { halted = state; }

    // Reset requested by /RESET signal (not power-on)
    void setPinReset(void) { pinReset = true; }

    bool isPendingEI(void) const { return pendingEI; }
    void setPendingEI(bool state) { pendingEI = state; }

    // Reset
    void reset(void);

    // Execute one instruction
    void execute(void);
    void jj_execute_one_frame(void); //One frame complete 69887 tstates    

#ifdef WITH_BREAKPOINT_SUPPORT
    bool isBreakpoint(void) { return breakpointEnabled; }
    void setBreakpoint(bool state) { breakpointEnabled = state; }
#endif

#ifdef WITH_EXEC_DONE
    void setExecDone(bool status) { execDone = status; }
#endif

private:
    // Rota a la izquierda el valor del argumento
    inline void rlc(uint8_t &oper8);

    // Rota a la izquierda el valor del argumento
    inline void rl(uint8_t &oper8);

    // Rota a la izquierda el valor del argumento
    inline void sla(uint8_t &oper8);

    // Rota a la izquierda el valor del argumento (como sla salvo por el bit 0)
    inline void sll(uint8_t &oper8);

    // Rota a la derecha el valor del argumento
    inline void rrc(uint8_t &oper8);

    // Rota a la derecha el valor del argumento
    inline void rr(uint8_t &oper8);

    // Rota a la derecha 1 bit el valor del argumento
    inline void sra(uint8_t &oper8);

    // Rota a la derecha 1 bit el valor del argumento
    inline void srl(uint8_t &oper8);

    // Incrementa un valor de 8 bits modificando los flags oportunos
    inline void inc8(uint8_t &oper8);

    // Decrementa un valor de 8 bits modificando los flags oportunos
    inline void dec8(uint8_t &oper8);

    // Suma de 8 bits afectando a los flags
    inline void add(uint8_t oper8);

    // Suma con acarreo de 8 bits
    inline void adc(uint8_t oper8);

    // Suma dos operandos de 16 bits sin carry afectando a los flags
    inline void add16(RegisterPair &reg16, uint16_t oper16);

    // Suma con acarreo de 16 bits
    inline void adc16(uint16_t reg16);

    // Resta de 8 bits
    inline void sub(uint8_t oper8);

    // Resta con acarreo de 8 bits
    inline void sbc(uint8_t oper8);

    // Resta con acarreo de 16 bits
    inline void sbc16(uint16_t reg16);

    // Operaci�n AND l�gica
    // Simple 'and' is C++ reserved keyword
    inline void and_(uint8_t oper8);

    // Operaci�n XOR l�gica
    // Simple 'xor' is C++ reserved keyword
    inline void xor_(uint8_t oper8);

    // Operaci�n OR l�gica
    // Simple 'or' is C++ reserved keyword
    inline void or_(uint8_t oper8);

    // Operaci�n de comparaci�n con el registro A
    // es como SUB, pero solo afecta a los flags
    // Los flags SIGN y ZERO se calculan a partir del resultado
    // Los flags 3 y 5 se copian desde el operando (sigh!)
    inline void cp(uint8_t oper8);

    // DAA
    inline void daa(void);

    // POP
    inline uint16_t pop(void);

    // PUSH
    inline void push(uint16_t word);

    // LDI
    void ldi(void);

    // LDD
    void ldd(void);

    // CPI
    void cpi(void);

    // CPD
    void cpd(void);

    // INI
    void ini(void);

    // IND
    void ind(void);

    // OUTI
    void outi(void);

    // OUTD
    void outd(void);

    // BIT n,r
    inline void bitTest(uint8_t mask, uint8_t reg);

    //Interrupci�n
    //JJ void interrupt(void);
    public: void interrupt(void);

    //Interrupci�n NMI
    void nmi(void);

    // Decode main opcodes
    void decodeOpcode(uint8_t opCode);

    // Subconjunto de instrucciones 0xCB
    // decode CBXX opcodes
    void decodeCB(void);

    //Subconjunto de instrucciones 0xDD / 0xFD
    // Decode DD/FD opcodes
    void decodeDDFD(uint8_t opCode, RegisterPair& regIXY);

    // Subconjunto de instrucciones 0xDD / 0xFD 0xCB
    // Decode DD / FD CB opcodes
    void decodeDDFDCB(uint8_t opCode, uint16_t address);

    //Subconjunto de instrucciones 0xED
    // Decode EDXX opcodes
    void decodeED(uint8_t opCode);
};
#endif // Z80CPP_H

#endif
