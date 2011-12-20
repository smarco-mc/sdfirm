#include <wdm/usb_cmn.h>

const char *wdm_usb_ioctl_string(ULONG code)
{
	switch (code) {
	case IOCTL_INTERNAL_USB_RESET_PORT:
		return "IOCTL_INTERNAL_USB_RESET_PORT";
	case IOCTL_INTERNAL_USB_CYCLE_PORT:
		return "IOCTL_INTERNAL_USB_CYCLE_PORT";
	case IOCTL_INTERNAL_USB_SUBMIT_URB:
		return "IOCTL_INTERNAL_USB_SUBMIT_URB";
	case IOCTL_INTERNAL_USB_GET_PORT_STATUS:
		return "IOCTL_INTERNAL_USB_GET_PORT_STATUS";
	default:
		return "IOCTL_INTERNAL_USB_UNKNOWN";
	}
}
