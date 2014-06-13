#include <asm/avalanche/generic/pal.h>
#include <linux/module.h>
#include <linux/spinlock.h>


static unsigned int avalanche_vbus_freq;

void avalanche_set_vbus_freq(unsigned int new_vbus_freq)
{
    avalanche_vbus_freq = new_vbus_freq;
}

unsigned int avalanche_get_vbus_freq(void)
{
    return(avalanche_vbus_freq);
}

/* Reset API */
EXPORT_SYMBOL(PAL_sysResetCtrl);
EXPORT_SYMBOL(PAL_sysGetResetStatus);
EXPORT_SYMBOL(PAL_sysSystemReset);
EXPORT_SYMBOL(PAL_sysGetSysLastResetStatus);

/* Power API */
EXPORT_SYMBOL(PAL_sysPowerCtrl);
EXPORT_SYMBOL(PAL_sysGetPowerStatus);
EXPORT_SYMBOL(PAL_sysSetGlobalPowerMode);
EXPORT_SYMBOL(PAL_sysGetGlobalPowerMode);

/* Clock API */
EXPORT_SYMBOL(PAL_sysClkcSetFreq);
EXPORT_SYMBOL(PAL_sysClkcGetFreq);

/* Cache API */
EXPORT_SYMBOL(PAL_sysCacheInvalidate);
EXPORT_SYMBOL(PAL_sysCacheFlush);
EXPORT_SYMBOL(PAL_sysCacheFlushAndInvalidate);

/* Wakeup API */
EXPORT_SYMBOL(PAL_sysWakeupCtrl);

/* Misc API */
EXPORT_SYMBOL(PAL_sysGetChipVersionInfo);
EXPORT_SYMBOL(avalanche_get_cpu_type);
EXPORT_SYMBOL(avalanche_get_cpu_name);
EXPORT_SYMBOL(avalanche_set_mdix_on_chip);
EXPORT_SYMBOL(avalanche_is_mdix_on_chip);
EXPORT_SYMBOL(avalanche_set_vbus_freq);
EXPORT_SYMBOL(avalanche_get_vbus_freq);


#if defined(CONFIG_MIPS_AVALANCHE_TIMER16)
EXPORT_SYMBOL(PAL_sysTimer16SetParams);    
EXPORT_SYMBOL(PAL_sysTimer16GetFreqRange);
EXPORT_SYMBOL(PAL_sysTimer16Ctrl);
#endif 

#if defined(CONFIG_MIPS_AVALANCHE_WDTIMER)
EXPORT_SYMBOL(PAL_sysWdtimerInit);      
EXPORT_SYMBOL(PAL_sysWdtimerSetPeriod);
EXPORT_SYMBOL(PAL_sysWdtimerCtrl);      
EXPORT_SYMBOL(PAL_sysWdtimerKick);      
#endif 


#if defined (CONFIG_AVALANCHE_GENERIC_GPIO)
EXPORT_SYMBOL(PAL_sysGpioInit);
EXPORT_SYMBOL(PAL_sysGpioCtrl);
EXPORT_SYMBOL(PAL_sysGpioOutBit);
EXPORT_SYMBOL(PAL_sysGpioInBit);
EXPORT_SYMBOL(PAL_sysGpioOutValue);
EXPORT_SYMBOL(PAL_sysGpioInValue);
#endif



