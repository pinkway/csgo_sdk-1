#pragma once

enum e_client_frame_stage {
	FRAME_UNDEFINED = -1,
	FRAME_START,

	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,

	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class c_view_setup {
public:
	__int32   x;
	__int32   x_old;
	__int32   y;
	__int32   y_old;
	__int32   width;
	__int32   width_old;
	__int32   height;
	__int32   height_old;
	char      pad0[144];
	float     fov;
	float     viewmodel_fov;
	vec3_t	  origin;
	qangle_t  angles;
	char      pad1[124];
};

class i_client_mode {
public:

};