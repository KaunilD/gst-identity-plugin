#include "gstoclrgb2gray.h"
#include "gst/gstelementfactory.h"
#include "gstreamermm/plugin.h"

std::string OCLRGB2GRAY::m_srcCaps = "video/x-raw,format=BGR,width=[1," +
                                         std::to_string(std::numeric_limits<int>::max()) + "],height=[1," +
                                         std::to_string(std::numeric_limits<int>::max()) + "]";
std::string OCLRGB2GRAY::m_sinkCaps = "video/x-raw,format=BGR,width=[1," +
                                          std::to_string(std::numeric_limits<int>::max()) + "],height=[1," +
                                          std::to_string(std::numeric_limits<int>::max()) + "]";

OCLRGB2GRAY::OCLRGB2GRAY(GstBaseTransform *gobj): Glib::ObjectBase(typeid(OCLRGB2GRAY)), 
        Gst::BaseTransform(gobj), m_enabled(*this, "enabled") {
            m_enabled.set_value(false);
}

void OCLRGB2GRAY::class_init(Gst::ElementClass<OCLRGB2GRAY> *klass) {

    klass->set_metadata(
        "OCLRGB2GRAY", "xavier/filters", "RGB2 Grayscale conversion using OpenCV", "Kaunil Dhruv <kdhruv@logitech.com");

    klass->add_pad_template(
        Gst::PadTemplate::create("sink", Gst::PAD_SINK, Gst::PAD_ALWAYS, Gst::Caps::create_from_string(m_sinkCaps)));
    klass->add_pad_template(
        Gst::PadTemplate::create("src", Gst::PAD_SRC, Gst::PAD_ALWAYS, Gst::Caps::create_from_string(m_srcCaps)));
}


Gst::FlowReturn OCLRGB2GRAY::transform_ip_vfunc(const Glib::RefPtr<Gst::Buffer> &buf) {
    if (!m_enabled.get_value()) {
        return Gst::FLOW_OK;
    }

    Gst::VideoInfo info;
    info.from_caps(this->get_sink_pad()->get_current_caps());

    return Gst::FLOW_OK;
}

Gst::FlowReturn OCLRGB2GRAY::transform_vfunc(
    const Glib::RefPtr<Gst::Buffer> &inbuf,
    const Glib::RefPtr<Gst::Buffer> &outbuf) {

    return Gst::FLOW_OK;
}

bool OCLRGB2GRAY::registerOCLRGB2GRAY(Glib::RefPtr<Gst::Plugin> plugin) {
    GType t = Gst::register_mm_type<OCLRGB2GRAY>("OCLRGB2GRAY");
    Gst::ElementFactory::register_element(plugin, "oclrgb2gray", GST_RANK_NONE, t);
    return true;
}

bool OCLRGB2GRAY::get_unit_size_vfunc(const Glib::RefPtr<Gst::Caps> &caps, gsize &size) const {
    Gst::VideoInfo in_info;
    
    if (!in_info.from_caps(caps)) {
        return false;
    }
    
    size = in_info.get_size();
    return true;
}

bool OCLRGB2GRAY::start_vfunc() {
    return true;
}

Glib::RefPtr<Gst::Caps> OCLRGB2GRAY::transform_caps_vfunc(
    Gst::PadDirection direction,
    const Glib::RefPtr<Gst::Caps> &caps,
    const Glib::RefPtr<Gst::Caps> &filter) {
        
    Glib::RefPtr<Gst::Caps> ret;
    if (direction == Gst::PAD_SRC)
        ret = Gst::Caps::create_from_string(m_sinkCaps);
    else
        ret = Gst::Caps::create_from_string(m_srcCaps);

    if (filter)
        ret = ret->get_intersect(filter, Gst::CAPS_INTERSECT_FIRST);
    
    return ret;
}

Gst::FlowReturn OCLRGB2GRAY::prepare_output_buffer_vfunc(
    const Glib::RefPtr<Gst::Buffer> &input,
    Glib::RefPtr<Gst::Buffer> &buffer) {
    
    auto caps = this->get_src_pad()->get_current_caps();
    gsize size;
    this->get_unit_size_vfunc(caps, size);
    buffer = buffer->create(size);
    buffer = buffer->create_writable();
    
    if (!copy_metadata_vfunc(input, buffer)) {
        std::cout << "unable to copy metadata\n";
        return Gst::FLOW_CUSTOM_ERROR;
    }

    return Gst::FLOW_OK;
}
