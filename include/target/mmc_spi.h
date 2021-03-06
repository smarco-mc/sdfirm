/*
 * ZETALOG's Personal COPYRIGHT
 *
 * Copyright (c) 2019
 *    ZETALOG - "Lv ZHENG".  All rights reserved.
 *    Author: Lv "Zetalog" Zheng
 *    Internet: zhenglv@hotmail.com
 *
 * This COPYRIGHT used to protect Personal Intelligence Rights.
 * Redistribution and use in source and binary forms with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the Lv "Zetalog" ZHENG.
 * 3. Neither the name of this software nor the names of its developers may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 4. Permission of redistribution and/or reuse of souce code partially only
 *    granted to the developer(s) in the companies ZETALOG worked.
 * 5. Any modification of this software should be published to ZETALOG unless
 *    the above copyright notice is no longer declaimed.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ZETALOG AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE ZETALOG OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)mmc_spi.h: SPI mode MMC definitions
 * $Id: mmc_spi.h,v 1.1 2019-10-24 16:00:00 zhenglv Exp $
 */

#ifndef __MMC_SPI_H_INCLUDE__
#define __MMC_SPI_H_INCLUDE__

#ifndef ARCH_HAVE_MMC
#define ARCH_HAVE_MMC		1
#else
#error "Multiple MMC controller defined"
#endif

#include <target/spi.h>

/* 9.5 SPI Bus Protocol:
 * Only single and multiple block read/write operations are supported in
 * SPI mode (sequential mode is not supported).
 */
#define MMC_CLASS2 1
#define MMC_CLASS4 1

#define MMC_CMD_CRC_ON_OFF	MMC_CMD59

/* 9.5.2 Responses */
/* Format R1 */
#define MMC_R1_IN_IDLE_STATE		_BV(0)
#define MMC_R1_ERASE_RESET		_BV(1)
#define MMC_R1_ILLEGAL_COMMAND		_BV(2)
#define MMC_R1_COM_CRC_ERROR		_BV(3)
#define MMC_R1_ERASE_SEQUENCE_ERROR	_BV(4)
#define MMC_R1_ADDRESS_MISALIGN		_BV(5)
#define MMC_R1_ADDRESS_OUT_OF_RANGE	_BV(6)
#define MMC_R1_ERRORS						\
	(MMC_R1_ERASE_RESET | MMC_R1_ILLEGAL_COMMAND |		\
	 MMC_R1_COM_CRC_ERROR | MMC_R1_ERASE_SEQUENCE_ERROR |	\
	 MMC_R1_ADDRESS_MISALIGN | MMC_R1_ADDRESS_OUT_OF_RANGE)

/* Command frame starts by asserting low and then high for first two clock
 * edges.
 */
#define MMC_SPI_CMD(cmd)		(0x40 | (cmd))
#define MMC_SPI_CRC(cmd)		(0x01 | (crc))

/* Data token for commands 17, 18, 24 */
#define SD_DATA_TOKEN			0xfe

#ifdef CONFIG_MMC_SPI
#define mmc_hw_ctrl_init()		mmc_spi_init()
#define mmc_hw_slot_select(rca)		mmc_spi_select(rca)
#define mmc_hw_send_command(cmd, arg)	mmc_spi_send(cmd, arg)
#define mmc_hw_recv_response(resp, len)	mmc_spi_recv(resp, len)
#define mmc_hw_tran_data(dat, len, cnt)	mmc_spi_tran(dat, len, cnt)
#define mmc_hw_card_busy()		mmc_spi_busy()
#endif

void mmc_spi_init(void);
void mmc_spi_select(mmc_rca_t rca);
void mmc_spi_send(uint8_t cmd, uint32_t arg);
void mmc_spi_recv(uint8_t *resp, uint16_t len);
void mmc_spi_tran(uint8_t *dat, uint32_t len, uint16_t cnt);
bool mmc_spi_busy(void);

/* Send dummy byte (all ones).
 * Used in many cases to read one byte from SD card, since SPI is a
 * full-duplex protocol and it is necessary to send a byte in order to read
 * a byte.
 */
uint8_t mmc_spi_dummy(void);
uint8_t mmc_spi_txrx(uint8_t tx);
void mmc_spi_cmpl(void);

extern uint8_t mmc_spi_resp;

#endif /* __MMC_SPI_H_INCLUDE__ */
