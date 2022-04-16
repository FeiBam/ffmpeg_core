#include "aecho.h"

int create_aecho_fliter(int index, AVFilterGraph* graph, AVFilterContext* src, c_linked_list** list, float in_gain, float out_gain, c_linked_list** delays, c_linked_list** decays) {
	if (!graph || !in_gain || out_gain || delays || decays || !src || !list) return FFMPEG_CORE_ERR_NULLPTR;
	if (in_gain > 1.0) in_gain = 1.0;
	if (out_gain > 1.0) out_gain = 1.0;
	size_t delays_count = c_linked_list_count(&delays);
	size_t decays_count = c_linked_list_count(&decays);
	c_linked_list* delays_data = c_linked_list_head(&delays);
	c_linked_list* decays_data = c_linked_list_head(&decays);
	if (delays_count != decays_count) return FFMPEG_CORE_ERR_INVALID_PARAMS;
	char args[128];
	char name[32];
	for (size_t i = 0; i < decays_count; i++) {
		if (*(float*)delays_data->d < (float)0.1) *(float*)delays_data->d = 0.1;
		if (*(float*)delays_data->d > (float)90000.0) *(float*)delays_data->d = 90000.0;
		if (*(float*)decays_data->d < (float)0.1) *(float*)decays_data->d = 0.1;
		if (*(float*)delays_data->d > (float)1.0) *(float*)delays_data->d = 1.0;
		delays_data = delays_data->next;
		decays_data = decays_data->next;
	}
	const AVFilter* aecho = avfilter_get_by_name("aecho");
	AVFilterContext* context = NULL;
}

int create_args(c_linked_list** list, char* buf) {
	size_t list_count = c_linked_list_count(&list);
	c_linked_list* list_data = c_linked_list_head(&list);
	size_t temp_size = list_count * 32;
	char* temp;
	if (sizeof(buf) < temp_size) {
		temp = realloc(buf, temp_size);
		if (temp == 0) {
			return FFMPEG_CORE_ERR_OOM;
		}
		buf = temp;
		free(temp);
	}
	for (size_t i = 0; i < list_count; i++) {
		snprintf(buf, sizeof(buf), "%f|", *(float*)list_data->d);
	}
	return 0;
}

