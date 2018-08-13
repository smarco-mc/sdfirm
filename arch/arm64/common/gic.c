/*
 * ZETALOG's Personal COPYRIGHT
 *
 * Copyright (c) 2018
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
 * @(#)vic_gic.c: generic interrupt controller implementation
 * $Id: vic_gic.c,v 1.279 2011-10-19 10:19:18 zhenglv Exp $
 */

#include <target/irq.h>

void irqc_hw_ack_irq(irq_t irq)
{
	/* CPU ID is 0 */
	gic_end_irq(irq, 0);
}

void irqc_hw_handle_irq(void)
{
	irq_t irq;
	uint8_t cpu;

	gic_begin_irq(irq, cpu);
	if (irq >= NR_IRQS) {
		gic_end_irq(irq, cpu);
		return;
	}
	if (!do_IRQ(irq)) {
		irqc_hw_disable_irq(irq);
		gic_end_irq(irq, cpu);
	}
}

void irqc_hw_configure_irq(irq_t irq, uint8_t prio,
			   uint8_t trigger)
{
	uint32_t cfg;

	__raw_writel_mask(GIC_PRIORITY(irq, prio),
			  GIC_PRIORITY_MASK,
			  GICD_IPRIORITYR(irq));
	if (trigger == IRQ_LEVEL_TRIGGERED)
		cfg = GIC_TRIGGER(GIC_TRIGGER_LEVEL);
	else
		cfg = GIC_TRIGGER(GIC_TRIGGER_EDGE);
	__raw_writel_mask(GIC_INT_CONFIG(irq, cfg),
			  GIC_INT_CONFIG_MASK,
			  GICD_ICFGR(irq));
}