//
// Created by karaler on 2020/12/24.
// Author: karaler
// Description: 
// Others: 
// Copyright (c) 2020 Bytelang . All rights reserved.
//

#ifndef KPLAYER_KPVIDEOSHOWPROCESSPLUGIN_H
#define KPLAYER_KPVIDEOSHOWPROCESSPLUGIN_H

#include "adapter/KPPluginAdapter.h"
#include "util/KPHelper.h"
#include "config.h"

#include <utility>

extern std::shared_ptr<KPPluginDriver> kplayer_plugin_driver;
extern KPEvent<bool>                   global_event_play_list_updated;

class KPVideoShowProcessPlugin : public KPPluginAdapter {
protected:
    std::string font_size  = "17";
    std::string font_color = "white";
    std::string x          = "10";
    std::string y          = "10";
    void Task() override;
    bool        stop       = false;
    int         ret        = 0;
public:
    explicit KPVideoShowProcessPlugin(const std::string &identify_name, const std::string &filter_name, const KPFilterType &filter_type, PluginParamsObject plugin_params_object);
    ~KPVideoShowProcessPlugin() override;
    void KillTask() override;
    void InitTask() override;
};

// slot
extern "C" {
KPLAYER_PLUGIN_FUNC(KPVideoShowProcessPlugin);
}


#endif //KPLAYER_KPVIDEOSHOWPROCESSPLUGIN_H
