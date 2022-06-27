/* Startup code for SCARTS (32-bit).
 * Copyright (C) 2010, 2011 Embedded Computing Systems Group,
 * Department of Computer Engineering, Vienna University of Technology.
 * Contributed by Martin Walter <mwalter@opencores.org>
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

    .file	"crt0.S"

.macro LOAD_SYM_TO_REG reg symbol
    .global \symbol
    ldhi    \reg, 4th(\symbol)
    ldliu   \reg, 3rd(\symbol)
    sli     \reg, 8
    ldliu   \reg, hi(\symbol)
    sli     \reg, 8
    ldliu   \reg, lo(\symbol)
.endm

.macro WAIT_SPI_DATA_SENT
    nop
    ldb r9, r6                                  ; p_scarts->spi.gen_if.usr_stat -> r9
    btest r9, 0                                 ; if(p_scarts->spi.gen_if.usr_stat & SPI_REG_STAT_BUSY) cond-flag = 1;
    jmpi_ct -2
.endm

    .section .text
    .global	_start
    .type start, @function
_start: ; the linker script defines start as the application entry point
    ; Copy program from FLASH to SDRAM
    LOAD_SYM_TO_REG r1, SCARTS_PROG_CONFIG      ; PROGRAMMER_CONFIG_C_BADDR -> r1
    ; LOAD_SYM_TO_REG r2, SCARTS_PROG_ADDR        ; PROGRAMMER_ADDRESS_BADDR -> r2
    mov r2, r1
    addi r2, 1
    ; LOAD_SYM_TO_REG r3, SCARTS_PROG_DATA        ; PROGRAMMER_DATA_BADDR -> r3
    mov r3, r2
    addi r3, 4
    ;               r4                          ; Instruction data
    LOAD_SYM_TO_REG r5, SCARTS_GPO              ; &p_scarts->gpio.gpo -> r5
    LOAD_SYM_TO_REG r6, SCARTS_SPI_CONFIG       ; &p_scarts->spi.gen_if.usr_conf -> r6
    LOAD_SYM_TO_REG r7, SCARTS_SPI_WR_DATA      ; &p_scarts->spi.wr_data -> r7
    LOAD_SYM_TO_REG r8, SCARTS_SPI_RD_DATA      ; &p_scarts->spi.rd_data -> r8
    ;               r9                          ; temp
    LOAD_SYM_TO_REG r10, FLASH_PROG_START_BYTE  ; FLASH_PROG_START_BYTE -> r10
    LOAD_SYM_TO_REG r11, FLASH_PROG_LEN_BYTE    ; FLASH_PROG_LEN_BYTE -> r11
    sri r11, 1                                  ; FLASH_PROG_LEN_INSTR -> r11
    ldli r12, 0x00                              ; DUMMY_DATA -> r12
    ;               r13                         ; address counter


    ; SPI init
    ldli r9, 0x02                               ; SPI_REG_CONF_CLOCK_2X -> r9
    ;ldli r9, 0x00                               ; 0x00 -> r9
    stb r9, r6                                  ; p_scarts->spi.gen_if.usr_conf = r9;

    ; SPI init done. Now use SPI status register
    addi r6, -2                                 ; &p_scarts->spi.gen_if.usr_stat -> r6

    ; copy programs
    stw r12, r2

    ldli r9, 0x0F                               ; SPI_CS_IDLE_HIGH -> r9
    stb r9, r5                                  ; p_scarts->gpio.gpo = SPI_CS_IDLE_HIGH;

    ldli r13, 0x00                              ; address counter = 0x00;

    ldli r9, 0x0D                               ; p_scarts->gpio.gpo &= ~SPI_CS_FLASH;
    stb r9, r5

    ldli r9, 0x03                               ; SPI_CMD_READ -> r9
    stb r9, r7                                  ; p_scarts->spi.wr_data = SPI_CMD_READ;
    WAIT_SPI_DATA_SENT

    mov r9, r10
    sri r9, 8
    sri r9, 8
    stb r9, r7                                  ; p_scarts->spi.wr_data = 3rd(FLASH_RO_DATA_START + &.rodata-Section);
    WAIT_SPI_DATA_SENT

    mov r9, r10
    sri r9, 8
    stb r9, r7                                  ; p_scarts->spi.wr_data = hi(FLASH_RO_DATA_START + &.rodata-Section);
    WAIT_SPI_DATA_SENT

    mov r9, r10
    stb r9, r7                                  ; p_scarts->spi.wr_data = lo(FLASH_RO_DATA_START + &.rodata-Section);
    WAIT_SPI_DATA_SENT

.prog_init_loop_first:

    cmp_lt r13, r11                             ; if(address counter < __bss_start) cond-flag = 1;
    jmpi_cf .prog_init_end

    stb r12, r7                                 ; p_scarts->spi.wr_data = DUMMY_DATA;
.prog_init_loop:
    WAIT_SPI_DATA_SENT
    ldbu r4, r8                                 ; p_scarts->spi.rd_data -> r4

    stb r12, r7                                 ; p_scarts->spi.wr_data = DUMMY_DATA;
    WAIT_SPI_DATA_SENT
    ldbu r9, r8                                 ; p_scarts->spi.rd_data -> r4

    addi r13, 1                                 ; ++address counter;
    cmp_lt r13, r11                             ; if(address counter < __bss_start) cond-flag = 1;
    jmpi_cf .prog_no_new_spi_transfer
    stb r12, r7                                 ; p_scarts->spi.wr_data = DUMMY_DATA;

.prog_no_new_spi_transfer:
    sli r9, 8
    add r9, r4

    sth r9, r3                                  ; PROGRAMMER_DATA = r4;
    ldli r9, 0x80
    stb r9, r1                                  ; Execute write

    cmp_lt r13, r11                             ; if(address counter < __bss_start) cond-flag = 1;
    jmpi_ct .prog_init_loop

.prog_init_end:
    ldli r9, 0x0F                               ; SPI_CS_IDLE_HIGH -> r9
    stb r9, r5                                  ; p_scarts->gpio.gpo = SPI_CS_IDLE_HIGH;

    addi r6, 2                                  ; &p_scarts->spi.gen_if.usr_conf -> r6
    stb r12, r6                                 ; p_scarts->spi.gen_if.usr_conf = 0x00 (DUMMY_DATA);

    ; SignalTab Debug
    ;ldli r9, 0x0B                               ; SPI_CS_IDLE_HIGH -> r9
    ;stb r9, r5                                  ; p_scarts->gpio.gpo = ~SPI_CS_EPCS16;

    ldli r9, 0x0F                               ; SPI_CS_IDLE_HIGH -> r9
    stb r9, r5                                  ; p_scarts->gpio.gpo = SPI_CS_IDLE_HIGH;

.start_prog:
    LOAD_SYM_TO_REG r13, RAM_PROG_START_BYTE
    jmp r13
    .size _start, .-_start
