#ifndef _MUSICPLAYER2_AECHO_H
#define _MUSICPLAYER2_AECHO_H
#if __cplusplus
#include "core.h"
extern "C" {
#endif
#if !__cplusplus
#include "core.h"
#endif
	int create_aecho_fliter(AVFilterGraph* graph, AVFilterContext* src, c_linked_list** list, float in_gain, float out_gain, c_linked_list** delays, c_linked_list** decays);
#if __cplusplus
}
#endif
#endif // _MUSICPLAYER2_AECHO_H
