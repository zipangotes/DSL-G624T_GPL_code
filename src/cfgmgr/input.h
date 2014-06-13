#ifndef  __VOICEAPI_INPUT_H__
#define  __VOICEAPI_INPUT_H__

#include "cm_message.h"

void	setup_input(int fd, int timeout);
struct msg_base	*get_input( void );

#endif /*__VOICEAPI_INPUT_H__*/
