#ifndef __IRQC_SPIKE_H_INCLUDE__
#define __IRQC_SPIKE_H_INCLUDE__

#ifndef ARCH_HAVE_IRQC_
#define ARCH_HAVE_IRQC		1
#else
#error "Multiple IRQ controller defined"
#endif

#define irqc_hw_enable_irq(irq)		riscv_enable_irq(irq)
#define irqc_hw_disable_irq(irq)	riscv_disable_irq(irq)
#define irqc_hw_trigger_irq(irq)	riscv_trigger_irq(irq)
#define irqc_hw_clear_irq(irq)		riscv_clear_irq(irq)
#define irqc_hw_configure_irq(irq, priority, trigger)	\
	do { } while (0)

/* clint handles no external IRQs */
#define irqc_hw_handle_irq()		do { } while (0)

/* clint requires no CPU specific initialization */
#define irqc_hw_ctrl_init()		do { } while (0)
#ifdef CONFIG_SMP
#define irqc_hw_smp_init()		do { } while (0)
#endif
#ifdef CONFIG_MMU
#define irqc_hw_mmu_init()		do { } while (0)
#endif

#endif /* __IRQC_SPIKE_H_INCLUDE__ */
