/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef CHEWY_DETAIL_H
#define CHEWY_DETAIL_H

#include "chewy/ngshext.h"
#include "chewy/defines.h"

#define SAVE_ROOM 0
#define RESTORE_ROOM 1
#define MAX_SOUNDS 3

#define MAX_AUTO_MOV 20

typedef struct {
	uint8 RoomNr;
	uint8 BildNr;

	uint8 AutoMovAnz;
	uint8 TafLoad;

	char TafName[14];
	uint8 ZoomFak;
	uint8 Dummy;

} RaumInfo;

typedef struct {
	int16 X;
	int16 Y;
	uint8 SprNr;

	uint8 dummy;
} RaumAutoMov;

#define MAXTAF 10
#define MAXDETAILS 32
#define MAX_M_ITEMS 50
#define MAX_RDI_ROOM 100

typedef struct {

	char *Image;
	int16 ZEbene;
	int16 X;
	int16 Y;
	int16 X1;
	int16 Y1;
} SprInfo;

typedef struct {
	int16 sound_enable[MAX_SOUNDS];
	int16 sound_index [MAX_SOUNDS];
	int16 sound_start [MAX_SOUNDS];
	int16 kanal [MAX_SOUNDS];
	int16 volume [MAX_SOUNDS];
	int16 repeats [MAX_SOUNDS];
	int16 stereo [MAX_SOUNDS];
} sound_def_blk;

typedef struct {
	int16 x;
	int16 y;

	uint8 start_flag;
	uint8 repeat;
	int16 start_ani;
	int16 end_ani;
	int16 ani_count;
	int16 delay;
	int16 delay_count;
	int16 reverse;

	int16 timer_start;

	int16 z_ebene;

	uint8 load_flag;

	uint8 zoom;
	sound_def_blk sfx;

	int16 show_1_phase;

	int16 phase_nr;

} ani_detail_info;

typedef struct {
	int16 x;
	int16 y;
	int16 SprNr;

	short z_ebene;

	bool Hide;

	uint8 Dummy;
} static_detail_info;

typedef struct {
	int16 StaticDetailAnz;
	int16 AniDetailAnz;
	taf_info *dptr;
	ani_detail_info Ainfo [MAXDETAILS];
	static_detail_info Sinfo [MAXDETAILS];
	int16 mvect [MAX_M_ITEMS * 4];
	int16 mtxt [MAX_M_ITEMS];
	RaumInfo Ri;
	RaumAutoMov AutoMov [MAX_AUTO_MOV];
	int16 tvp_index[MAXDETAILS * MAX_SOUNDS];
	char *sample [MAXDETAILS * MAX_SOUNDS];
} room_detail_info;

typedef struct {
	char Id[4];
	int16 Anz;
} RdiDateiHeader;

#define MAXPATH 255
typedef struct {
	char Id[7];
	char IibFile[MAXPATH];
	char SibFile[MAXPATH];
	char RdiFile[MAXPATH];
	char TafFile[MAXPATH];
	char TgpFile[MAXPATH];
	char Workdir[MAXPATH];
	char InventFile[MAXPATH];
	char EibFile[MAXPATH];
	char AtsRoomFile[MAXPATH];
	char AadFile[MAXPATH];
	char AtsRoomSteuer[MAXPATH];
	char AdsFile[MAXPATH];
	char AtdsFont[MAXPATH];
	char AdhFile[MAXPATH];
	char TvpFile[MAXPATH];
	char DummyFile[MAXPATH];
} DeteditPrj;

class detail {
public:
	detail();
	~detail();

	void load_rdi(const char *fname, int16 room_nr);

	void load_rdi_taf(const char *fname, int16 load_flag);

#ifdef ICM
	void save_detail(char *fname, int16 room_nr);
#endif
	void set_static_spr(int16 nr, int16 spr_nr);
	char *get_static_image(int16 det_nr);

	void set_static_pos(int16 det_nr, int16 x, int16 y, bool hide, bool korr_flag);
	void set_detail_pos(int16 det_nr, int16 x, int16 y);
	void hide_static_spr(int16 nr);
	void show_static_spr(int16 nr);
	void freeze_ani();
	void unfreeze_ani();
	void get_ani_werte(int16 ani_nr, int16 *start, int16 *end);
	void set_ani(int16 ani_nr, int16 start, int16 end);
	char *get_image(int16 spr_nr);
	ani_detail_info *get_ani_detail(int16 ani_nr);
	int16 *get_korrektur_tbl();
	void init_taf(taf_info *dptr);
	taf_info *get_taf_info();
	room_detail_info *get_room_detail_info();

	void set_static_ani(int16 ani_nr, int16 static_nr);

	void del_static_ani(int16 ani_nr);
	void set_ani_delay(int16 nr, int16 del);
	void start_detail(int16 nr, int16 rep, int16 reverse);
	void stop_detail(int16 nr);
	void plot_ani_details(int16 scrx, int16 scry, int16 start, int16 end,
	                      int16 zoomx, int16 zoomy);
	void plot_ani_details(int16 scrx, int16 scry, int16 start, int16 end);
	void plot_static_details(int16 scrx, int16 scry, int16 start, int16 end);
	void init_list(int16 *mv);
	void get_list(int16 *mv);
	int16 maus_vector(int16 x, int16 y);
	int16 get_ani_status(int16 det_nr);

	SprInfo plot_detail_sprite(int16 scrx, int16 scry, int16 det_nr,
	                           int16 spr_nr, int16 mode);

	void set_global_delay(int16 delay);

	taf_info *init_taf_tbl(const char *fname);

	void del_taf_tbl(taf_info *Tt);

	void del_taf_tbl(int16 start, int16 anz, taf_info *Tt);

	// FIXME : was FILE*
	void load_taf_seq(void *handle, int16 spr_nr, int16 spr_anz,
	                  taf_info *Tt);

	void load_taf_seq(int16 spr_nr, int16 spr_anz, taf_info *Tt);
	void load_taf_tbl(taf_info *fti);

	void del_dptr();

	void calc_zoom_kor(int16 *kx, int16 *ky, int16 xzoom,
	                   int16 yzoom);

	int16 mouse_on_detail(int16 mouse_x, int16 mouse_y,
	                      int16 scrx, int16 scry);

	void set_sound_area(char *buffer, uint32 size);
	// FIXME : was FILE*
	void load_room_sounds(void *tvp_handle);
	void get_sound_para(int16 nr, sound_def_blk *sdb_dest);
	void set_sound_para(int16 nr, sound_def_blk *sdb_src);
	void map_tvp2sound(int16 nr, int16 sslot, int16 tvp_index);
	void disable_sound(int16 nr, int16 sslot);
	void enable_sound(int16 nr, int16 sslot);
	void play_sound(int16 nr, int16 sslot);
	void disable_detail_sound(int16 nr);
	void enable_detail_sound(int16 nr);
	void clear_detail_sound(int16 nr);
	void play_detail_sound(int16 nr);
	void disable_room_sound();
	void enable_room_sound();
	void clear_room_sound();
#ifdef DETEDIT
	void shadow_room(int16 mode);
#endif
	void set_taf_ani_mem(char *load_area);
	// FIXME : was FILE*
	void load_sprite_pointer(void *handle);

private:
	void load_taf_ani_sprite(int16 nr);

	void remove_unused_samples();
#ifdef KONVERT
	void konvert_format(room_detail_info *rdi_a,
	                    room_detail_info_new *rdi_n);
	room_detail_info_new rdi_new;
#endif
	room_detail_info rdi;
#ifdef DETEDIT
	room_detail_info rdi_shadow;
#endif
	RdiDateiHeader rdi_datei_header;
	SprInfo spr_info;

	int16 global_delay;
	int16 ani_freezeflag;
	int16 fulltaf;
	char tafname[80];
	char *taf_load_buffer;
	int16 direct_taf_ani;
	// FIXME : was FILE*
	void *CurrentTaf;
	uint32 SpritePos[MAXSPRITE];

	char *SoundBuffer;
	uint32 SoundBufferSize;
};

#endif
