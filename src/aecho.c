#include "aecho.h"

int create_aecho_fliter(AVFilterGraph* graph, AVFilterContext* src, c_linked_list** list, float in_gain, float out_gain, c_linked_list** delays, c_linked_list** decays) {
	//if (!graph || !in_gain || !out_gain || !delays || !decays || !src || !list) return FFMPEG_CORE_ERR_NULLPTR;
	if (in_gain > 1.0) in_gain = 1.0;
	if (out_gain > 1.0) out_gain = 1.0;
	int delays_count = c_linked_list_count(*delays);
	int decays_count = c_linked_list_count(*decays);
	c_linked_list* delays_data = c_linked_list_head(*delays);
	c_linked_list* decays_data = c_linked_list_head(*decays);
	if (delays_count != decays_count) return FFMPEG_CORE_ERR_INVALID_PARAMS;
	char args[256]; //×î´ó256×Ö·û´®  
	char name[32];
	for (int i = 0; i < decays_count; i++) {
		if (*(float*)delays_data->d < (float)0.1) *(float*)delays_data->d = (float)0.1;
		if (*(float*)delays_data->d > (float)90000.0) *(float*)delays_data->d = (float)90000.0;
		if (*(float*)decays_data->d < (float)0.1) *(float*)decays_data->d = (float)0.1;
		if (*(float*)decays_data->d > (float)1.0) *(float*)decays_data->d = (float)1.0;
		delays_data = delays_data->next;
		decays_data = decays_data->next;
	}
	snprintf(args, sizeof(args), "aecho=%f:%f", in_gain, out_gain);
	create_args(delays, args);
	create_args(decays, args);
	snprintf(name, sizeof(args), "aecho%d", decays_count);
	int re = 0;
	const AVFilter* aecho = avfilter_get_by_name("aecho");
	AVFilterContext* context = NULL;
	if ((re = avfilter_graph_create_filter(&context,aecho, name, args, NULL, graph)) < 0) {
		av_log(NULL, AV_LOG_FATAL, "Failed to create aecho filter \"%s\": %s (%i)\n", name, av_err2str(re), re);
		return re;
	}
	if (!c_linked_list_append(list, (void*)context)) {
		av_log(NULL, AV_LOG_FATAL, "Failed to append filter \"%s\" to list.\n", context->name);
		return FFMPEG_CORE_ERR_OOM;
	}
	if (c_linked_list_count(*list) > 1) {
		AVFilterContext* last = c_linked_list_tail(*list)->prev->d;
		if ((re = avfilter_link(last, 0, context, 0)) < 0) {
			av_log(NULL, AV_LOG_FATAL, "Failed to link %s:%i -> %s:%i: %s (%i)\n", last->name, 0, context->name, 0, av_err2str(re), re);
			return re;
		}
	}
	else {
		if ((re = avfilter_link(src, 0, context, 0)) < 0) {
			av_log(NULL, AV_LOG_FATAL, "Failed to link %s:%i -> %s:%i: %s (%i)\n", src->name, 0, context->name, 0, av_err2str(re), re);
			return re;
		}
	}
	return FFMPEG_CORE_ERR_OK;
}

int create_args(c_linked_list** list, char* buf) {
	char temp[64];
	int list_count = c_linked_list_count(*list);
	c_linked_list* list_data = c_linked_list_head(*list);
	for (int i = 0; i < list_count; i++) {
		if (i == list_count) snprintf(temp, sizeof(temp), "%f:", *(float*)list_data->d);
		else snprintf(temp, sizeof(temp), "%f|", *(float*)list_data->d);
	}
	snprintf(buf, sizeof(buf), "%s%s", buf,temp);
	return FFMPEG_CORE_ERR_OK;
}

