KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build
M ?= $(shell pwd)

# $(WLAN_PLATFORM_ROOT) has to be a absolute path
ifeq ($(WLAN_PLATFORM_ROOT),)
WLAN_PLATFORM_ROOT = $(shell pwd)

# If it reaches here, compilation is probably without Android.mk,
# so enable all flags (including debug flag CONFIG_CNSS2_DEBUG) by
# default.
KBUILD_OPTIONS := WLAN_PLATFORM_ROOT=$(WLAN_PLATFORM_ROOT)
endif

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) modules $(KBUILD_OPTIONS)

modules_install:
	$(MAKE) INSTALL_MOD_STRIP=1 -C $(KERNEL_SRC) M=$(M) modules_install

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(M) clean
