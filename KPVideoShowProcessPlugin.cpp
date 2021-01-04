//
// Created by karaler on 2020/12/24.
// Author: karaler
// Description: 
// Others: 
// Copyright (c) 2020 Bytelang . All rights reserved.
//

#include "KPVideoShowProcessPlugin.h"

KPVideoShowProcessPlugin::KPVideoShowProcessPlugin(const std::string &identify_name, const std::string &filter_name, const KPFilterType &filter_type, PluginParamsObject plugin_params_object) : KPPluginAdapter(identify_name, filter_name, filter_type, plugin_params_object) {
    if (plugin_params_object.params.find("font_size") != plugin_params_object.params.end()) {
        font_size = plugin_params_object.params["font_size"];
    }
    if (plugin_params_object.params.find("font_color") != plugin_params_object.params.end()) {
        font_color = plugin_params_object.params["font_color"];
    }
    if (plugin_params_object.params.find("x") != plugin_params_object.params.end()) {
        x = plugin_params_object.params["x"];
    }
    if (plugin_params_object.params.find("y") != plugin_params_object.params.end()) {
        y = plugin_params_object.params["y"];
    }

    // 赋值described
    std::stringstream filter_desc_stream;
    filter_desc_stream << "x=" << x << ":y=" << y << ":fontfile=res/font.ttf:fontsize=" << font_size << ":fontcolor=" << font_color << ":text='" << "[无]" << "'";
    filter_desc = filter_desc_stream.str();

    // 查找过滤器
    filter = avfilter_get_by_name("drawtext");
    if (filter == nullptr)
        throw KPFilterException("未知过滤器; filter name: drawtext");
}

KPVideoShowProcessPlugin::~KPVideoShowProcessPlugin() {
}


/**
 * 秒级定时器，一秒延迟之后更新plugin text内容
 * 在plugin可用时会使用单独的子线程开启task，需要主要判断filter_context当前是否可用
 * 在对plugin reload时 filter_context会短暂不可用
 *
 */
void KPVideoShowProcessPlugin::Task() {
    do {
        std::this_thread::sleep_for(std::chrono::microseconds(1000));

        std::stringstream time_str_stream;
        auto              current_seek     = kplayer_plugin_driver->PluginDriverGetCurrentPlayMediaSeekTime();
        auto              current_duration = kplayer_plugin_driver->PluginDriverGetCurrentPlayMediaDuration();

        int seek_hour   = floor(current_seek / 3600);
        int seek_minute = floor(int(current_seek) % 3600 / 60);
        int seek_second = int(current_seek) % 60;

        int duration_hour   = floor(current_duration / 3600);
        int duration_minute = floor(int(current_duration) % 3600 / 60);
        int duration_second = int(current_duration) % 60;

        char time[200];
        sprintf(time, "%02d:%02d:%02d / %02d:%02d:%02d", seek_hour, seek_minute, seek_second, duration_hour, duration_minute, duration_second);

        SetPluginValue(std::map<std::string, std::string>{{"text", time}});
    } while (!stop);
}

void KPVideoShowProcessPlugin::KillTask() {
    stop = true;
}

void KPVideoShowProcessPlugin::InitTask() {
    stop = false;
}


// slot
KPLAYER_PLUGIN_FUNC(KPVideoShowProcessPlugin) {
    return new KPVideoShowProcessPlugin("kplayer", "video_plugin_show_process", KP_FILTER_TYPE_VIDEO, std::move(plugin_params));
}
