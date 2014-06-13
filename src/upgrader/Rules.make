.PHONY: dummy

first_rule: sub_dirs all_targets

#include $(TOPDIR)/.config

SUB_DIRS = $(subdir-y)
CFLAGS += $(cflags-y) -DBOARD_$(BOARD) -DVERSION=\"$(CONFIG_TI_NSP_VERSION)\"
LDFLAGS += $(ldflags-y)
SRCDIR = $(TOPDIR)/src
INCLUDE = $(SRCDIR)/include
ZLIBDIR = $(TI_base)$(TI_rlibdir)
AR=ar
RANLIB=ranlib

CFLAGS += -I$(INCLUDE) -Wall

ifdef DMALLOC
CFLAGS += -g -DDMALLOC
LDFLAGS += -ldmalloc --static
else
CFLAGS += -Os
endif

# main directory list
subdir-list = $(patsubst %, _subdir_%, $(SUB_DIRS))
sub_dirs: dummy $(subdir-list)

ifdef SUB_DIRS
$(subdir-list) : dummy
	@ $(MAKE) -C $(patsubst _subdir_%,%,$@) BOARD=$(BOARD)
endif

# dependency directory list
dep-subdir-list = $(patsubst %, dep_subdir_%, $(SUB_DIRS))
dep-sub_dirs: dummy $(dep-subdir-list)

ifdef SUB_DIRS
$(dep-subdir-list) : dummy
	@ $(MAKE) -C $(patsubst dep_subdir_%,%,$@) dep
endif

SRC_FILES=$(shell ls *.c 2> /dev/null)

dep-files-list = $(patsubst %, dep_file_%, $(SRC_FILES))
dep-files: emptydep $(dep-files-list)

emptydep:
	@ > .depend

ifdef SRC_FILES
$(dep-files-list) : dummy
	@ $(CC) $(CFLAGS) -MM $(patsubst dep_file_%,%,$@) >> .depend
endif

dep: dep-sub_dirs dep-files

# all binary targets
all_targets: $(O_TARGET) $(SO_TARGET) $(A_TARGET) $(TARGET)

ifdef O_TARGET
ifdef obj-y
$(O_TARGET) : $(obj-y)
	rm -f $@
	$(LD) -r -o $@ $(filter $(obj-y), $^) $(LDFLAGS)
endif
endif

ifdef TARGET
ifdef obj-y
$(TARGET) : $(obj-y)
	rm -f $@
	$(CC) -o $@ $(filter $(obj-y), $^) $(LDFLAGS)
endif
endif

ifdef A_TARGET
ifdef obj-y
$(A_TARGET): $(obj-y)
	rm -f $@
	$(AR) rc $@ $(filter $(obj-y), $^)
	$(RANLIB) $@
endif
endif

ifdef SO_TARGET
ifdef obj-y
$(SO_TARGET): $(obj-y)
	rm -f $@
	$(CC) -shared -o $@ $(filter $(obj-y), $^) $(LDFLAGS)
endif
endif

# clean
clean:
	find . \( -name '*.[oas]' -o -name core -o -name '*~' -o -name '.*~' -o -name '.#*' -o -name '*.so' \) -type f -print | xargs rm -f

deep-clean: clean
	find . \( -name '.depend' \) -type f -print | xargs rm -f
	find . \( -name 'cm_logic' -o -name 'cm_pc' -o -name 'cm_monitor' -o -name 'cm_cli' \) -type f -perm +111 -print | xargs rm -f

dummy:

ifneq ($(wildcard .depend),)
include .depend
endif
