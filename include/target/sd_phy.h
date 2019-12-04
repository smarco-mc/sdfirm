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
 * @(#)sd_phy.h: secure digital (SD) physical layer definitions
 * $Id: sd_phy.h,v 1.1 2019-10-25 13:40:00 zhenglv Exp $
 */

#ifndef __SD_PHY_H_INCLUDE__
#define __SD_PHY_H_INCLUDE__

#ifdef SD_CLASS2
#define MMC_CLASS2	1
#endif
#ifdef SD_CLASS7
#define MMC_CLASS4	1
#endif
#ifdef SD_CLASS5
#define MMC_CLASS5	1
#endif
#ifdef SD_CLASS6
#define MMC_CLASS6	1
#endif
#ifdef SD_CLASS7
#define MMC_CLASS7	1
#endif
#ifdef SD_CLASS8
#define MMC_CLASS8	1
#endif

#define SD_PHY_VERSION_10		10
#define SD_PHY_VERSION_20		20
#define SD_PHY_VERSION_30		30
#define SD_PHY_VERSION_40		40
#ifndef SD_PHY_VERSION
#define SD_PHY_VERSION			(CONFIG_SD_PHY_VERSION * 10)
#endif
#define SD_PHY_VERSION_UNKNOWN		0

#if defined(CONFIG_SD_SDHC) || defined(CONFIG_SD_SDXC)
#define SD_HCS				1
#else
#define SD_HCS				0
#endif

/* Bus speed mode */
#define SD_SPEED_DEFAULT		0 /* 3.3V 25MHz 12.5Mbps */
#define SD_SPEED_HIGH			1 /* 3.3V 50MHz 25Mbps */
#define SD_SPEED_SDR12			2 /* UHS-I 1.8V 25MHz 12.5Mbps */
#define SD_SPEED_SDR25			3 /* UHS-I 1.8V 50MHz 25Mbps */
#define SD_SPEED_SDR50			4 /* UHS-I 1.8V 100MHz 50Mbps */
#define SD_SPEED_SDR104			5 /* UHS-I 1.8V 208MHz 104Mbps */
#define SD_SPEED_DDR50			6 /* UHS-I 1.8V 50MHz 50Mbps */
#define SD_SPEED_UHS156			7 /* UHS-II 26-52MHz 1.56Gbps */
#define SD_SPEED_UHS624			8 /* UHS-II 26-52MHz 6.24Gbps */

/* Used for SD_CMD_SEND_IF_COND */
#define SD_CHECK_PATTERN		0xAA

/* bc */
#define SD_CMD_SEND_IF_COND		MMC_CMD8

/* bcr */
#define SD_CMD_SEND_RELATIVE_ADDR	MMC_CMD3

/* ac */
#define SD_CMD_VOLTAGE_SWITCH		MMC_CMD11
#if defined(SD_CLASS2) || defined(SD_CLASS4)
#define SD_CMD_SPEED_CLASS_CONTROL	MMC_CMD20
#endif
#ifdef SD_CLASS5
#define SD_CMD_ERASE_WR_BLK_START	MMC_CMD32
#define SD_CMD_ERASE_WR_BLK_END		MMC_CMD33
#endif
#ifdef SD_CLASS1
#define SD_CMD_Q_MANAGEMENT		MMC_CMD43
#define SD_CMD_Q_TASK_INFO_A		MMC_CMD44
#define SD_CMD_Q_TASK_INFO_B		MMC_CMD45
#endif

/* adtc */
#ifdef SD_CLASS2
#define SD_CMD_SEND_TUNING_BLOCK	MMC_CMD19
#endif
#ifdef SD_CLASS10
#define SD_CMD_SWITCH_FUNC		MMC_CMD6  /* 1.10 */
#endif
#ifdef CONFIG_SD_SPI
#define SD_CMD_READ_OCR			MMC_CMD58
#else /* CONFIG_SD_SPI */
#ifdef SD_CLASS11
#define SD_CMD_READ_EXTRA_SINGLE	MMC_CMD48
#define SD_CMD_WRITE_EXTRA_SINGLE	MMC_CMD49
#define SD_CMD_READ_EXTRA_MULTI		MMC_CMD58
#define SD_CMD_WRITE_EXTRA_MULTI	MMC_CMD59
#endif
#endif /* CONFIG_SD_SPI */
#ifdef SD_CLASS1
#define SD_CMD_Q_RD_TASK		MMC_CMD46
#define SD_CMD_Q_WR_TASK		MMC_CMD47
#endif

/* bcr */
#define SD_ACMD_SEND_OP_COND		MMC_ACMD41

/* ac */
#define SD_ACMD_SET_BUS_WIDTH		MMC_ACMD6
#define SD_ACMD_SET_WR_BLK_ERASE_COUNT	MMC_ACMD23
#define SD_ACMD_SET_CLR_CARD_DETECT	MMC_ACMD42

/* adtc */
#define SD_ACMD_SD_STATUS		MMC_ACMD13
#define SD_ACMD_SEND_NUM_WR_BLOCKS	MMC_ACMD22
#define SD_ACMD_SEND_SCR		MMC_ACMD51

/* SD specific response types */
#define SD_R6		(MMC_RSP_TYPE(6)|MMC_R_CS)
#define SD_R7		(MMC_RSP_TYPE(7)|MMC_R_CS)

/* SD specific states */
#define SD_STATE_ver		9  /* After CMD8, SPEC version identified */

typedef uint8_t sd_r6_t[4];
typedef uint8_t sd_r7_t[4];

/* 4.10.1 CSR register - Card Status register */
#define SD_DET_CARD_ECC_DISABLED	_BV(14)
#define SD_DET_FX_EVENT			_BV(6)
#define SD_DET_AKE_SEQ_ERROR		_BV(3)

/* 5.1 OCR register - Operation Conditions register */
#define SD_OCR_S18A			_BV(24)
#define SD_OCR_UHSCS			_BV(29) /* UHS-II Card Status */
#define SD_OCR_CCS			_BV(30) /* Card Capacity Status */

/* 5.2 CID register - Card IDentification register */
typedef struct {
	uint8_t mid;	/* manuafacturer ID */
	uint16_t oid;	/* OEM/application ID */
#ifdef CONFIG_MMC_CID_ALL_FIELDS
	uint8_t pnm[5];	/* product name */
	uint8_t prv;	/* product revision */
	uint32_t psn;	/* product serial number */
	uint16_t mdt;	/* manuafacturing date */
	uint8_t crc;
#endif
} __packed sd_cid_t;

/* 5.3.3 CSD Register (CSD Version 2.0) */
/* CSD2 */
#define SD_CSD20_2_C_SIZE_OFFSET	0
#define SD_CSD20_2_C_SIZE_MASK		REG_6BIT_MASK
#define SD_CSD20_2_C_SIZE(value)	_GET_FV(SD_CSD20_2_C_SIZE, value)
/* CSD1 */
#define SD_CSD1_ERASE_BLK_EN		_BV(14)
#define SD_CSD1_SECTOR_SIZE_OFFSET	7
#define SD_CSD1_SECTOR_SIZE_MASK	REG_7BIT_MASK
#define SD_CSD1_SECTOR_SIZE(value)	_GET_FV(SD_CSD1_SECTOR_SIZE, value)
#define SD_CSD1_WP_GRP_SIZE_OFFSET	0
#define SD_CSD1_WP_GRP_SIZE_MASK	REG_7BIT_MASK
#define SD_CSD20_1_C_SIZE_OFFSET	16
#define SD_CSD20_1_C_SIZE_MASK		REG_16BIT_MASK
#define SD_CSD20_1_C_SIZE(value)	_GET_FV(SD_CSD20_1_C_SIZE, value)

#define SD_SPEED_CLASS_OFFSET	0
#define SD_SPEED_CLASS_MASK	REG_28BIT_MASK
#define SD_SPEED_CLASS(value)	_SET_FV(SD_SPEED_CLASS, value)
#define SD_SPEED_CONTROL_OFFSET	28
#define SD_SPEED_CONTROL_MASK	REG_4BIT_MASK
#define SD_SPEED_CONTROL(value)	_SET_FV(SD_SPEED_CONTROL, value)

/* CMD_SPEED_CLASS_CONTROL */
typedef struct {
	uint32_t speed_class : 28;
	uint32_t speed_control: 4;
} __packed sd_speed_t;

/* CMD_SEND_IF_COND */
/* 4.3.13 Send Interface Condition Command (CMD8)
 * 4.9.6 R1 (Card interface condition)
 */
#define SD_R7_VHS_OFFSET		8
#define SD_R7_VHS_MASK			REG_4BIT_MASK
#define SD_R7_VHS(value)		_SET_FV(SD_R7_VHS, value)
#define sd_r7_vhs(value)		_GET_FV(SD_R7_VHS, value)
#define SD_R7_CHECK_PATTERN_OFFSET	0
#define SD_R7_CHECK_PATTERN_MASK	REG_8BIT_MASK
#define SD_R7_CHECK_PATTERN(value)	_SET_FV(SD_R7_CHECK_PATTERN, value)
#define sd_r7_check_pattern(value)	_GET_FV(SD_R7_CHECK_PATTERN, value)
/* host supply voltage */
#define SD_VOLTAGE_UNDEF		0
#define SD_VOLTAGE_HIGH			1
#define SD_VOLTAGE_LOW			2

/* ACMD_SET_BUS_WIDTH */
#define SD_BUS_WIDTH_OFFSET		4
#define SD_BUS_WIDTH_MASK		REG_2BIT_MASK
#define sd_bus_width(value)		_GET_FV(SD_BUS_WIDTH, value)

/* ACMD_SET_WR_BLK_ERASE_COUNT */
#define SD_WR_BLK_ERASE_COUNT_OFFSET	0
#define SD_WR_BLK_ERASE_COUNT_MASK	REG_23BIT_MASK
#define sd_wr_blk_erase_count(value)	_GET_FV(SD_WR_BLK_ERASE_COUNT, value)

/* ACMD_SD_SEND_OP_COND, R3 */
#define SD_OCR_S18R			_BV(24)
#define SD_OCR_XPC			_BV(28)
#define SD_OCR_HCS_OFFSET		30
#define SD_OCR_HCS_MASK			REG_1BIT_MASK
#define SD_OCR_HCS			_SET_FV(SD_OCR_HCS, SD_HCS)

#define sd_state_is(state)	(mmc_state_get() == SD_STATE_##state)
#define sd_state_gt(state)	(mmc_state_get() > SD_STATE_##state)
#define sd_state_ge(state)	(mmc_state_get() >= SD_STATE_##state)
#define sd_state_lt(state)	(mmc_state_get() < SD_STATE_##state)
#define sd_state_le(state)	(mmc_state_get() <= SD_STATE_##state)
#define sd_state_enter(state)	mmc_state_set(SD_STATE_##state)

#define MMC_PHY_SLOT				\
	sd_cid_t cid;				\
	uint8_t card_version;			\
	uint32_t wr_blk_erase_count : 23;	\
	sd_speed_t speed;			\
	uint8_t bus_width;

#if defined(CONFIG_CONSOLE) && defined(CONFIG_MMC_DEBUG)
extern const char *mmc_phy_state_names[16];
extern const char *mmc_phy_cmd_names[64];
extern const char *mmc_phy_acmd_names[64];
#endif

uint32_t sd_encode_if_cond(void);

void mmc_phy_reset_slot(void);
void mmc_phy_handle_seq(void);
void mmc_phy_handle_stm(void);
void mmc_phy_recv_rsp(void);
void mmc_phy_send_cmd(void);

#endif /* __SD_PHY_H_INCLUDE__ */
