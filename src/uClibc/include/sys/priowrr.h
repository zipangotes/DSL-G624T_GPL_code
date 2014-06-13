#include <linux/linkage.h>
#include <linux/unistd.h>

_syscall4(int, config_voice_qos, char **, names, int, asymmtu, int, vpi, int, vci);

