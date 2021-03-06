#include <target/timer.h>
#include <target/jiffies.h>
#include <target/smp.h>
#include <target/sbi.h>
#include <target/irq.h>

void tmr_irq_handler(void)
{
	uint8_t cpu = smp_processor_id();

	tmr_irq_clear(cpu);
	tmr_disable_irq(cpu);
	irqc_clear_irq(IRQ_TIMER);
	irqc_disable_irq(IRQ_TIMER);
	tick_handler();
}

#ifdef SYS_BOOTLOAD
void gpt_hw_irq_poll(void)
{
	if (riscv_irq_raised(IRQ_TIMER))
		tmr_irq_handler();
}
#endif

void gpt_hw_oneshot_timeout(timeout_t tout_ms)
{
	uint8_t cpu = smp_processor_id();
	uint64_t next;

#ifdef CONFIG_DPU_TMR_VIP
	next = tmr_read_counter() + CONFIG_DPU_TMR_VIP_TOUT;
	tmr_write_compare(cpu, next);
#else
	next = tick_get_counter() + tout_ms;
	tmr_write_compare(cpu, TSC_FREQ * next);
#endif
	tmr_enable_irq(cpu);
	irqc_enable_irq(IRQ_TIMER);
}

void gpt_hw_ctrl_init(void)
{
	irq_register_vector(IRQ_TIMER, tmr_irq_handler);
}
