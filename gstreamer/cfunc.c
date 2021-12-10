#define GST_USE_UNSTABLE_API
#include <gst/webrtc/webrtc.h>
#include <gst/app/gstappsrc.h>
#include <glib.h>
#include <gst/gst.h>
#include <gst/gstbin.h>
#include <json-glib/json-glib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

gboolean print_field (GQuark field, const GValue * value, gpointer pfx) {
  gchar *str = gst_value_serialize (value);
  g_print ("%s  %15s: %s\n", (gchar *) pfx, g_quark_to_string (field), str);
  g_free (str);
  return TRUE;
}

#pragma GCC diagnostic ignored "-Wuninitialized"
GstWebRTCSessionDescription * make_web_session_description() {
    GstWebRTCSessionDescription * web;
    return web;
}

void print_caps (const GstCaps * caps, const gchar * pfx) {
  guint i;
  g_return_if_fail (caps != NULL);
  if (gst_caps_is_any (caps)) {
    g_print ("%sANY\n", pfx);
    return;
  }
  if (gst_caps_is_empty (caps)) {
    g_print ("%sEMPTY\n", pfx);
    return;
  }
  for (i = 0; i < gst_caps_get_size (caps); i++) {
    GstStructure *structure = gst_caps_get_structure (caps, i);
    g_print ("%s%s\n", pfx, gst_structure_get_name (structure));
    gst_structure_foreach (structure, print_field, (gpointer) pfx);
  }
}

void print_pad_capabilities (GstElement *element, gchar *pad_name) {
  	GstPad *pad = NULL;
 	GstCaps *caps = NULL;
	pad = gst_element_get_static_pad(element, pad_name);
	if (!pad) {
		g_printerr ("Could not retrieve pad '%s'\n", pad_name);
		return;
	}
	caps = gst_pad_get_current_caps (pad);
	if (!caps)
		caps = gst_pad_query_caps (pad, NULL);
	g_print ("Caps for the %s pad:\n", pad_name);
	print_caps(caps, "      ");
	gst_caps_unref (caps);
	gst_object_unref (pad);
}

GstFlowReturn gst_app_src_push_buffer_wrap(GstElement* element, void *buffer,int len) {
  gpointer p = g_memdup(buffer, len);
  GstBuffer *data = gst_buffer_new_wrapped(p, len);
  return gst_app_src_push_buffer(GST_APP_SRC(element), data);
}

extern void on_incoming_enough_data (GstElement * webrtc, GstPad * pad, GstElement * pipe);
void on_incoming_enough_data_wrap(GstElement * webrtc, GstPad * pad, GstElement * pipe)
{
    on_incoming_enough_data(webrtc, pad, pipe);
}

extern void send_ice_candidate_message (GstElement * webrtc G_GNUC_UNUSED, guint mlineindex, gchar * candidate, void *user_data);
void send_ice_candidate_message_wrap (GstElement * webrtc G_GNUC_UNUSED, guint mlineindex, gchar * candidate, void *user_data)
{
    send_ice_candidate_message(webrtc, mlineindex, candidate, user_data);
}

extern void on_offer_set (GstPromise * webrtc, void* user_data);
void on_offer_set_wrap(GstPromise * webrtc, void* user_data)
{
    on_offer_set(webrtc, user_data);
}

extern void on_incoming_stream (GstElement * webrtc, GstPad * pad, GstElement * pipe);
void on_incoming_stream_wrap (GstElement * webrtc, GstPad * pad, GstElement * pipe)
{
    on_incoming_stream(webrtc, pad, pipe);
}

extern gboolean bus_call (GstBus *bus, GstMessage *msg, void *data);
gboolean bus_call_wrap (GstBus *bus, GstMessage *msg, void *data)
{
  return bus_call(bus, msg, data);
}

GstWebRTCRTPTransceiver *g_array_index_wrap(GArray *a,int i) {
    return  g_array_index(a, GstWebRTCRTPTransceiver*, i);
}

void g_object_set_fec(GstWebRTCRTPTransceiver* trans) {
    g_object_set(trans, "fec-type", GST_WEBRTC_FEC_TYPE_ULP_RED, "do-nack", TRUE, NULL);
}

void gst_caps_set_simple_wrap(GstCaps *caps, char *field, int type, void *value) {
    gst_caps_set_simple (caps, field, type, value, NULL);
}

extern void on_answer_created (GstPromise * promise, void * user_data);
void on_answer_created_wrap (GstPromise * promise, gpointer user_data) {
    on_answer_created(promise, user_data);
}

void g_object_set_wrap(gpointer object_type, gchar *first_property_name, void *three) {
    g_object_set(object_type, first_property_name, three, NULL);
}

void g_object_set_int_wrap(gpointer object_type, gchar *first_property_name, int three) {
    g_object_set(object_type, first_property_name, three, NULL);
}

void g_object_set_bool_wrap(gpointer object_type, gchar *first_property_name, bool three) {
    g_object_set(object_type, first_property_name, three, NULL);
}
GstCaps *gst_caps_set_format() {
    return gst_caps_new_simple("video/x-h264", "stream-format", G_TYPE_STRING, "avc", NULL);
}

gchar* gst_pad_get_name_wrap(GstElement *element) {
    return gst_pad_get_name(element);
}

gchar* gst_element_get_name_wrap(GstElement *element) {
    return gst_element_get_name(element);
}

void sendKeyFrame(GstPad * pad) {
    gst_pad_send_event(pad, gst_event_new_custom( GST_EVENT_CUSTOM_UPSTREAM, gst_structure_new( "GstForceKeyUnit", "all-headers", G_TYPE_BOOLEAN, TRUE, NULL)));
}

void sendKeyFrameD(GstPad * pad) {
    gst_pad_send_event(pad, gst_event_new_custom( GST_EVENT_CUSTOM_DOWNSTREAM, gst_structure_new( "GstForceKeyUnit", "all-headers", G_TYPE_BOOLEAN, TRUE, NULL)));
}

GstElement * castObjectToElement(GstObject * object) {
    return GST_ELEMENT_CAST(object);
}