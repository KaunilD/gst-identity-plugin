/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2021 root <<user@hostname.org>>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-oclrgb2gray
 *
 * FIXME:Describe oclrgb2gray here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! oclrgb2gray ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>

#include "gstoclrgb2gray.h"

GST_DEBUG_CATEGORY_STATIC (gst_ocl_rgb2gray_debug);
#define GST_CAT_DEFAULT gst_ocl_rgb2gray_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

#define gst_ocl_rgb2gray_parent_class parent_class
G_DEFINE_TYPE (GstoclRGB2GRAY, gst_ocl_rgb2gray, GST_TYPE_ELEMENT);


static void gst_ocl_rgb2gray_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec);
static void gst_ocl_rgb2gray_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec);

static gboolean gst_ocl_rgb2gray_sink_event (GstPad * pad,
    GstObject * parent, GstEvent * event);
static GstFlowReturn gst_ocl_rgb2gray_chain (GstPad * pad,
    GstObject * parent, GstBuffer * buf);

#define MAX_BUFFER 4718592
//  read file to string
std::string convertToString(const char *filename) {
    using std::fstream;
    std::string str;
    char* filestr;
    fstream f(filename, (fstream::in | fstream::binary));
    if (f.is_open()) {
        size_t len;
        f.seekg(0, fstream::end);
        len = (size_t)f.tellg();
        f.seekg(0, fstream::beg);
        filestr = (char *) malloc(len + 1);
        f.read(filestr, len);
        filestr[len] = '\0';
        str = filestr;
        free(filestr);
        f.close();
        return str;
    }
    std::cout << "open file filed" << std::endl;
    return str;
}


static cl::Platform platform = OCL::get_platforms();
static cl::Device device = OCL::get_device(platform);
static cl::Context context = OCL::get_context(device);
static std::string kernel_code = convertToString(
    "/root/development/git/logitech/gst-plugin-example/src/plugins/rgb2gray.cl");
  
static cl::Program program = OCL::get_program(kernel_code, context, device);

static cl::Buffer src_buffer(context, CL_MEM_READ_WRITE, sizeof(char) * MAX_BUFFER);
static cl::Buffer dst_buffer(context, CL_MEM_READ_WRITE, sizeof(char) * MAX_BUFFER);
static cl::CommandQueue queue(context, device);
/* GObject vmethod implementations */

/* initialize the oclrgb2gray's class */
static void
gst_ocl_rgb2gray_class_init (GstoclRGB2GRAYClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_ocl_rgb2gray_set_property;
  gobject_class->get_property = gst_ocl_rgb2gray_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
          FALSE, G_PARAM_READWRITE));

  gst_element_class_set_details_simple (gstelement_class,
      "oclRGB2GRAY",
      "FIXME:Generic",
      "FIXME:Generic Template Element", "root <<user@hostname.org>>");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&src_factory));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_ocl_rgb2gray_init (GstoclRGB2GRAY * filter)
{
  filter->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");
  gst_pad_set_event_function (filter->sinkpad,
      GST_DEBUG_FUNCPTR (gst_ocl_rgb2gray_sink_event));
  gst_pad_set_chain_function (filter->sinkpad,
      GST_DEBUG_FUNCPTR (gst_ocl_rgb2gray_chain));
  GST_PAD_SET_PROXY_CAPS (filter->sinkpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->sinkpad);

  filter->srcpad = gst_pad_new_from_static_template (&src_factory, "src");
  GST_PAD_SET_PROXY_CAPS (filter->srcpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->srcpad);

  filter->silent = FALSE;

}

static void
gst_ocl_rgb2gray_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstoclRGB2GRAY *filter = GST_OCLRGB2GRAY (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_ocl_rgb2gray_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstoclRGB2GRAY *filter = GST_OCLRGB2GRAY (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, filter->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstElement vmethod implementations */

/* this function handles sink events */
static gboolean
gst_ocl_rgb2gray_sink_event (GstPad * pad, GstObject * parent,
    GstEvent * event)
{
  GstoclRGB2GRAY *filter;
  gboolean ret;

  filter = GST_OCLRGB2GRAY (parent);

  GST_LOG_OBJECT (filter, "Received %s event: %" GST_PTR_FORMAT,
      GST_EVENT_TYPE_NAME (event), event);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS:
    {
      GstCaps *caps;

      gst_event_parse_caps (event, &caps);
      /* do something with the caps */

      /* and forward */
      ret = gst_pad_event_default (pad, parent, event);
      break;
    }
    default:
      ret = gst_pad_event_default (pad, parent, event);
      break;
  }
  return ret;
}

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_ocl_rgb2gray_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
  GstoclRGB2GRAY *filter;

  filter = GST_OCLRGB2GRAY (parent);

  if (filter->silent == TRUE)
    g_print ("I'm plugged, therefore I'm in.\n");

  GstMapInfo src_map;
  char h_dst[MAX_BUFFER];
  char d_dst[MAX_BUFFER];
  
  if (gst_buffer_map (buf, &src_map, GST_MAP_READ)) {
    
    cl::Kernel h_grayscale = cl::Kernel(program, "d_rgb2gray");
    queue.enqueueWriteBuffer(src_buffer, CL_TRUE, 0, sizeof(char)*MAX_BUFFER, src_map.data);
    h_grayscale.setArg(0, 640);
    h_grayscale.setArg(1, 480);
    h_grayscale.setArg(2, 3);
    h_grayscale.setArg(3, src_buffer);
    h_grayscale.setArg(4, dst_buffer);
    
    queue.enqueueNDRangeKernel(h_grayscale, cl::NullRange, cl::NDRange(MAX_BUFFER), cl::NullRange);
    queue.finish();
    queue.enqueueReadBuffer(dst_buffer, CL_TRUE, 0, MAX_BUFFER, h_dst);

    gst_buffer_fill(buf, 0, h_dst, MAX_BUFFER);
    gst_buffer_unmap(buf, &src_map);
  } 
  /* just push out the incoming buffer without touching it */
  return gst_pad_push (filter->srcpad, buf);
}


/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
oclrgb2gray_init (GstPlugin * oclrgb2gray)
{
  /* debug category for filtering log messages
   *
   * exchange the string 'Template oclrgb2gray' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_ocl_rgb2gray_debug, "oclrgb2gray",
      0, "Template oclrgb2gray");

  if (!gst_element_register (oclrgb2gray, "oclrgb2gray", GST_RANK_NONE,
          GST_TYPE_OCLRGB2GRAY))
    return FALSE;

  return TRUE;
}

/* PACKAGE: this is usually set by meson depending on some _INIT macro
 * in meson.build and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use meson to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "oclrgb2gray"
#endif

/* gstreamer looks for this structure to register oclrgb2grays
 *
 * exchange the string 'Template oclrgb2gray' with your oclrgb2gray description
 */
GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    oclrgb2gray,
    "Template oclrgb2gray",
    oclrgb2gray_init,
    "1.0", "LGPL", "oclrgb2gray", "http://gstreamer.net")
