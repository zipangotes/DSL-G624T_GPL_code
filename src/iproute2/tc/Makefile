include ../Config

ifeq ($(TC_CONFIG_ALL),y)
  TCOBJ=tc.o tc_qdisc.o tc_class.o tc_filter.o tc_util.o m_police.o m_estimator.o
else
  TCOBJ=tc.o tc_qdisc.o tc_filter.o tc_util.o
endif

TCMODULES :=
ifeq ($(TC_CONFIG_ALL),y)
  TCMODULES += q_fifo.o
  TCMODULES += q_sfq.o
  TCMODULES += q_red.o
  TCMODULES += q_prio.o
  TCMODULES += q_tbf.o
  TCMODULES += q_cbq.o
  TCMODULES += f_rsvp.o
  TCMODULES += f_route.o
  TCMODULES += f_fw.o
endif
TCMODULES += q_priowrr.o
TCMODULES += f_u32.o

ifeq ($(TC_CONFIG_DIFFSERV),y)
  TCMODULES += q_dsmark.o
  TCMODULES += q_gred.o
  TCMODULES += f_tcindex.o
  TCMODULES += q_ingress.o
endif
ifeq ($(TC_CONFIG_ATM),y)
  TCMODULES += q_atm.o
  LDLIBS += -latm
endif

#TCMODULES += q_csz.o
#TCMODULES += q_htb.o
#TCMODULES += q_hpfq.o
#TCMODULES += q_hfsc.o

TCOBJ += $(TCMODULES)

TCLIB := tc_core.o
ifeq ($(TC_CONFIG_ALL),y)
  TCLIB += tc_red.o
  TCLIB += tc_cbq.o
  TCLIB += tc_estimator.o
endif

LDLIBS += -L. -ltc -ldl
ifeq ($(TC_CONFIG_ALL),y)
  LDLIBS += -lm
endif
LDFLAGS += -Wl,-export-dynamic

ifeq ($(TC_CONFIG_ALL),y)
CFLAGS += -DTC_CONFIG_ALL
endif

all: libtc.a tc

tc: $(TCOBJ) $(LIBNETLINK) $(LIBUTIL) $(TCLIB)

libtc.a: $(TCLIB)
	ar rcs $@ $(TCLIB)

clean:
	rm -f $(TCOBJ) $(TCLIB) libtc.a tc

