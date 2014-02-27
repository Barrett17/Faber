#include "DefaultKeymap.h"

// Until we add support for XML in the API
// keymap support is disabled under gcc2,
// because it's unworth work.

#if __GNUC__ > 3

KeyBind kFileMenu[] {
	{ 0, "File", 0, 0, 0, 0, FABER_ITEM_START },
	{ 1,"New Project",         'N', B_COMMAND_KEY,                   0, 0,                      FABER_NEW_PROJECT },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 1,"Open",             'O', B_COMMAND_KEY,                 'O', 0,                      FABER_FILE_OPEN },
	{ 1,"Save",             'S', B_COMMAND_KEY,                   0, 0,                      	FABER_SAVE_PROJECT },
	{ 1,"Save As",             'S', B_COMMAND_KEY | B_SHIFT_KEY,  0, 0,                      FABER_SAVE_AS_PROJECT },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 0, "Recents", 0, 0, 0, 0, FABER_ITEM_START },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 1,"Export Project",           'E', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,              FABER_EXPORT_PROJECT },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 1,"Quit",                'Q', B_COMMAND_KEY,                   0, 0,                      B_QUIT_REQUESTED },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, NULL, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kEditMenu[] {
	{ 0, "Edit", 0, 0, 0, 0, FABER_ITEM_START },

	{ 1,"Undo",             'Z', B_COMMAND_KEY,                   0, 0,                         FABER_UNDO },
	{ 1,"Redo",             'Z', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,                         FABER_REDO },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 1,"Copy",             'C', B_COMMAND_KEY,                   0, 0,                         B_COPY },
	{ 1,"Copy & Silence",       'X', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,                     FABER_COPY_SILENCE },
	{ 1,"Cut",              'X', B_COMMAND_KEY,                   0, 0,                         B_CUT },
	{ 1,"Paste",                'V', B_COMMAND_KEY,                   0, 0,                     B_PASTE },
	//{ 1,"Paste to new project",         'V', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,             FABER_PASTE_NEW },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 1,"Select All",         'A', B_COMMAND_KEY,                   0, 0,                       B_SELECT_ALL },
	{ 1,"Unselect All",       'U', B_COMMAND_KEY,                   0, 0,                       FABER_UNSELECT_ALL },
    { 1,"Invert Selection",    0, 0,                           0, 0,                     0 },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 0, "Zero Cross", 0, 0, 0, 0, FABER_ITEM_START },

    { 1,"Zero In",            0, 0,                               0, 0,                         FABER_ZERO_IN },
    { 1,"Zero Out",           0, 0,                               0, 0,                         FABER_ZERO_OUT },
    { 1,"Zero LL",            0, 0,                               0, 0,                         FABER_ZERO_LL },
    { 1,"Zero LR",            0, 0,                               0, 0,                         FABER_ZERO_LR },
    { 1,"Zero RL",            0, 0,                               0, 0,                         FABER_ZERO_RL },
    { 1,"Zero RR",            0, 0,                               0, 0,                         FABER_ZERO_RR },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },
	//{ 1,"Set frequency",      0, 0,                               0, 0,                       FABER_SET_FREQUENCY },

	{ 1,"Clear",              'B', B_COMMAND_KEY,            B_DELETE, 0,                       FABER_CLEAR },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, NULL, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kTracksMenu[] {
	{ 0, "Tracks", 0, 0, 0, 0, FABER_ITEM_START },

    { 1, "New Track",                  0, 0,                           0, 0,                     FABER_ITEM_START },

    { 1, "Audio Track",             0, 0,                           0, 0,                     FABER_NEW_AUDIO_TRACK },
    { 1, "Label Track",           0, 0,                           0, 0,                     FABER_NEW_LABEL_TRACK },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

    { 1,"Reorder Tracks",                  0, 0,                           0, 0,                FABER_ITEM_START },

    { 1,"By Name",             0, 0,                           0, 0,                            FABER_SORT_BY_NAME },
    { 1,"By Duration",           0, 0,                           0, 0,                          FABER_SORT_BY_DURATION },
   // { 1,"By Type",           0, 0,                           0, 0,                            FABER_SORT_BY_TYPE },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

    { 1,"Mute All",             0, 0,                           0, 0,                           FABER_MUTE_ALL },
    { 1,"Unmute All",             0, 0,                           0, 0,                         FABER_UNMUTE_ALL },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, NULL, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kHelpMenu[] {
	{ 0, "Help", 0, 0, 0, 0, FABER_ITEM_START },

    { 1,"Homepage",                  0, 0,                           0, 0,                      FABER_OPEN_HOMEPAGE },
    { 1,"About",                  0, 0,                           0, 0,                         FABER_ABOUT },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, NULL, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kEngineMenu[] {
	{ 0, "Engine", 0, 0, 0, 0, FABER_ITEM_START },

    { 1,"Mixer",                  0, 0,                           0, 0,                      FABER_OPEN_MIXER },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 0, "Set Output", 0, 0, 0, 0, FABER_ITEM_START },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 0, "Transport", 0, 0, 0, 0, FABER_ITEM_START },

    { 0,"Play",     ' ', B_CONTROL_KEY,       0, 0,                                             FABER_TRANSPORT_PLAY },
    { 0,"Stop",       0, 0,                   0, 0,                                             FABER_TRANSPORT_STOP },
    { 0,"Rew",        0, 0,                   0, 0,                                             FABER_TRANSPORT_REW },
    { 0,"Rew All",B_HOME, B_SHIFT_KEY,        0, 0,                                             FABER_TRANSPORT_REW_ALL },
    { 0,"Forward",        0, 0,               0, 0,                                             FABER_TRANSPORT_FWD },
    { 0,"Forward All",B_END, B_SHIFT_KEY,     0, 0,                                             FABER_TRANSPORT_FWD_ALL },
    { 0,"Record",         0, 0,               0, 0,                                             FABER_TRANSPORT_REC },
    { 0,"Loop",     'L', 0,                   0, 0,                                             FABER_TRANSPORT_LOOP },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, NULL, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kProjectMenu[] {
	{ 0, "Project", 0, 0, 0, 0, FABER_ITEM_START },

	{ 0, "Tools", 0, 0, 0, 0, FABER_ITEM_START },

    { 0,"Selection Tool",                 0, 0,                 '1', 0,                        FABER_SELECTION_TOOL },
    //{ 0,"Draw tool",                      0, 0,                 '2', 0,                        FABER_DRAW_TOOL },
    { 0,"Play tool",                      0, 0,                 '3', 0,                        FABER_PLAY_TOOL },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER }, 

    { 0,"Zoom In",                    B_UP_ARROW, 0,                0,0,                       FABER_ZOOM_IN },
    { 0,"Zoom Out",                   B_DOWN_ARROW, 0,              0,0,                       FABER_ZOOM_OUT },
    { 0,"Zoom Full",                  B_UP_ARROW, B_SHIFT_KEY,      0,0,                       FABER_ZOOM_FULL },
    { 0,"Zoom to Selection",          B_DOWN_ARROW, B_SHIFT_KEY,    0,0,                       FABER_ZOOM_SELECTION },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 1,"Settings",      'P', B_COMMAND_KEY,                 'P', B_SHIFT_KEY,              FABER_SETTINGS },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, NULL, 0, 0, 0, 0, FABER_EOF }
};


KeyBind kTrackContextualMenu[] {
	{ 0, "TrackMenu", 0, 0, 0, 0, FABER_ITEM_START },

	{ 1,"Set Name...",                  0, 0,                           0, 0,                  FABER_TRACK_SET_NAME },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

    { 1,"Get Informations",                  0, 0,                           0, 0,             FABER_TRACK_GET_INFO },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

    { 1,"Select All",           0, 0,                           0, 0,                          B_SELECT_ALL },
    { 1,"Unselect",           0, 0,                           0, 0,                     FABER_UNSELECT_ALL},
  //  { 1,"Invert Selection",           0, 0,                           0, 0,                     0 },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

    { 1,"Create two mono tracks",           0, 0,                           0, 0,                     0 },
    { 1,"Separate channels",           0, 0,                           0, 0,                     0 },
    { 1,"Merge channels",           0, 0,                           0, 0,                     0 },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 1, "Set Input Node", 0, 0, 0, 0, FABER_ITEM_START },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, "", 0, 0, 0, 0, FABER_SPLITTER },

	{ 1, "Move Up", 0, 0, 0, 0, 0 },
	{ 1, "Move Down", 0, 0, 0, 0, 0 },

	{ 0, NULL, 0, 0, 0, 0, FABER_ITEM_END },

	{ 0, NULL, 0, 0, 0, 0, FABER_EOF }
};

KeyBind* kDefaultKeymap[] {
	kFileMenu,
	kEditMenu,
	kTracksMenu,
	kHelpMenu,
	kEngineMenu,
	kTrackContextualMenu,
	NULL
};

#endif
