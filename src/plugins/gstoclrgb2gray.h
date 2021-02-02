#ifndef __OCL_RGB2GRAY_H__
#define __OCL_RGB2GRAY_H__

#include <gstreamermm.h>
#include <gstreamermm/private/basetransform_p.h>
#include <iostream>
#include "gst/gstplugin.h"

#define PACKAGE "OCLRGB2GRAY"


class OCLRGB2GRAY : public Gst::BaseTransform {
public:
    explicit          OCLRGB2GRAY(GstBaseTransform *gobj);

    Gst::FlowReturn transform_vfunc(
        const Glib::RefPtr<Gst::Buffer> &inbuf, 
        const Glib::RefPtr<Gst::Buffer> &outbuf) override;

    Gst::FlowReturn transform_ip_vfunc(
        const Glib::RefPtr<Gst::Buffer> &buf) override;
    
    Gst::FlowReturn prepare_output_buffer_vfunc(
        const Glib::RefPtr<Gst::Buffer> &input, 
        Glib::RefPtr<Gst::Buffer> &buffer) override;

    
    Glib::RefPtr<Gst::Caps> transform_caps_vfunc(
        Gst::PadDirection direction,
        const Glib::RefPtr<Gst::Caps> &caps,
        const Glib::RefPtr<Gst::Caps> &filter) override;

    static void class_init(Gst::ElementClass<OCLRGB2GRAY> *klass);
    
    static gboolean plugin_init(Glib::RefPtr<Gst::Plugin> plugin);
    
    static bool registerOCLRGB2GRAY(Glib::RefPtr<Gst::Plugin> plugin);

    bool    get_unit_size_vfunc(const Glib::RefPtr<Gst::Caps> &caps, gsize &size) const override;
    
    bool    start_vfunc() override;
   
    
private:

    static std::string m_srcCaps;
    static std::string m_sinkCaps;
    Glib::Property<bool> m_enabled;
};

static gboolean plugin_init_oclrgb2gray(GstPlugin * plugin){
    return gst_element_register(
        plugin, 
        "oclrgb2gray", 
        GST_RANK_NONE,
        Gst::register_mm_type<OCLRGB2GRAY> ("oclrgb2gray")
    );
}

GST_PLUGIN_DEFINE (
  GST_VERSION_MAJOR,
  GST_VERSION_MINOR,
  oclrgb2gray,
  "oclrgb2gray plugin",
  plugin_init_oclrgb2gray,
  PACKAGE,
  "LGPL",
  "GStreamer",
  "http://gstreamer.net/"
);

#endif