package main

/*
#cgo pkg-config: gstreamer-1.0
#include <gst/gst.h>
*/
import "C"

func main() {
	C.gst_init(nil, nil)
	C.gst_debug_set_default_threshold(C.GST_LEVEL_WARNING)
	pipeString := "videotestsrc ! tee name=tee_video audiotestsrc ! audioconvert qos=true ! avenc_aac ! tee name=tee_audio  tee_video. ! cudaupload ! nvh264enc max-bitrate=1536 bitrate=1536 preset=3 rc-mode=2 zerolatency=true ! queue2 ! mux_1920. tee_audio. ! queue2 ! mux_1920. mpegtsmux  name=mux_1920 latency=500000000 ! hlssink max-files=5 location=hd/segment%05d.ts playlist-location=hd/playlist.m3u8 playlist-length=5 target-duration=3 tee_video. ! cudaupload ! nvh264enc max-bitrate=1024 bitrate=1024 preset=3 rc-mode=2 zerolatency=true ! queue2 ! mux_1280. tee_audio. ! queue2 ! mux_1280. mpegtsmux  name=mux_1280 latency=500000000 ! hlssink max-files=5 location=mid/segment%05d.ts playlist-location=mid/playlist.m3u8 playlist-length=5 target-duration=3 tee_video. ! cudaupload ! nvh264enc max-bitrate=798 bitrate=798 preset=3 rc-mode=2 zerolatency=true ! queue2 ! mux_854. tee_audio. ! queue2 ! mux_854. mpegtsmux  name=mux_854 latency=500000000 ! hlssink max-files=5 location=hi/segment%05d.ts playlist-location=hi/playlist.m3u8 playlist-length=5 target-duration=3 tee_video. ! cudaupload ! nvh264enc max-bitrate=512 bitrate=512 preset=3 rc-mode=2 zerolatency=true ! queue2 ! mux_428. tee_audio. ! queue2 ! mux_428. mpegtsmux  name=mux_428 latency=500000000 ! hlssink max-files=5 location=low/segment%05d.ts playlist-location=low/playlist.m3u8 playlist-length=5 target-duration=3"
	pipeline := C.gst_parse_launch(C.CString(pipeString), nil)
	C.gst_element_set_state(pipeline, C.GST_STATE_PLAYING)
	main_loop := C.g_main_loop_new(nil, 0)
	C.g_main_loop_run(main_loop)
}
