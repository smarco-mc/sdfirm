/*
 * ZETALOG's Personal COPYRIGHT v2
 *
 * Copyright (c) 2014
 *    ZETALOG - "Lv ZHENG".  All rights reserved.
 *    Author: Lv "Zetalog" Zheng
 *    Internet: zetalog@gmail.com
 *
 * This COPYRIGHT used to protect Personal Intelligence Rights.
 * Redistribution and use in source and binary forms with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the Lv "Zetalog" ZHENG.
 * 2. Permission of reuse of souce code only granted to ZETALOG and the
 *    developer(s) in the companies ZETALOG worked and has redistributed this
 *    software to.  Permission of redistribution of source code only granted
 *    to ZETALOG.
 * 3. Permission of redistribution and/or reuse of binary fully granted
 *    to ZETALOG and the companies ZETALOG worked and has redistributed this
 *    software to.
 * 4. Any modification of this software in the redistributed companies need
 *    not be published to ZETALOG.
 * 5. All source code modifications linked/included this software and modified
 *    by ZETALOG are of ZETALOG's personal COPYRIGHT unless the above COPYRIGHT
 *    is no long disclaimed.
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
 * @(#)acpi_core.c: ACPI locking and synchronization implementation
 * $Id: acpi_core.c,v 1.87 2011-10-17 01:40:34 zhenglv Exp $
 */
#include "acpi_int.h"

static struct acpi_namespace_node acpi_gbl_root = {
	ACPI_ROOT_NAME,
	ACPI_DESC_TYPE_NAMED,
	ACPI_TYPE_DEVICE,
	NULL,
	NULL,
	NULL,
	NULL,
};
struct acpi_namespace_node *acpi_gbl_root_node = &acpi_gbl_root;
static acpi_spinlock_t acpi_gbl_reference_lock;

static const char *acpi_gbl_mutex_names[ACPI_NUM_MUTEX] = {
	"ACPI_MTX_Interpreter",
	"ACPI_MTX_Namespace",
	"ACPI_MTX_Tables",
	"ACPI_MTX_Events",
	"ACPI_MTX_Caches",
	"ACPI_MTX_Memory",
	"ACPI_MTX_CommandComplete",
	"ACPI_MTX_CommandReady"
};

const char *acpi_mutex_name(uint32_t mutex_id)
{
	if (mutex_id > ACPI_MAX_MUTEX)
		return "Invalid Mutex ID";
	
	return (acpi_gbl_mutex_names[mutex_id]);
}

static void acpi_reference_update(struct acpi_reference *reference,
				  int action, int *new_count)
{
	acpi_cpuflags_t flags;

	if (!reference)
		return;

	flags = acpi_os_acquire_lock(&acpi_gbl_reference_lock);
	switch (action) {
	case REF_INCREMENT:
		reference->count++;
		break;
	case REF_DECREMENT:
		reference->count--;
		break;
	default:
		return;
	}
	if (new_count)
		*new_count = reference->count;
	acpi_os_release_lock(&acpi_gbl_reference_lock, flags);
}

void acpi_reference_inc(struct acpi_reference *reference)
{
	acpi_reference_update(reference, REF_INCREMENT, NULL);
}

void acpi_reference_dec(struct acpi_reference *reference)
{
	acpi_reference_update(reference, REF_DECREMENT, NULL);
}

int acpi_reference_dec_and_test(struct acpi_reference *reference)
{
	int count;
	acpi_reference_update(reference, REF_DECREMENT, &count);
	return count;
}

void acpi_reference_set(struct acpi_reference *reference, int count)
{
	acpi_cpuflags_t flags;

	if (!reference)
		return;

	flags = acpi_os_acquire_lock(&acpi_gbl_reference_lock);
	reference->count = count;
	acpi_os_release_lock(&acpi_gbl_reference_lock, flags);
}

int acpi_reference_get(struct acpi_reference *reference)
{
	acpi_cpuflags_t flags;
	int count;

	if (!reference)
		return 0;

	flags = acpi_os_acquire_lock(&acpi_gbl_reference_lock);
	count = reference->count;
	acpi_os_release_lock(&acpi_gbl_reference_lock, flags);

	return count;
}

acpi_status_t acpi_initialize_subsystem(void)
{
	acpi_status_t status;
	
	status = acpi_os_create_lock(&acpi_gbl_reference_lock);
	if (ACPI_FAILURE(status))
		return status;

	return AE_OK;
}