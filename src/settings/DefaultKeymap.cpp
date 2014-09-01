#include "DefaultKeymap.h"

// Until we add support for XML in the API
// keymap support is disabled under gcc2,
// because it's unworth work.

#if __GNUC__ > 3

KeyBind kFileMenu[] {
	{ "File", 0, 0, 0, 0, FABER_FILE_MENU, FABER_ITEM_START },
	{ "New Project",         'N', B_COMMAND_KEY,                   0, 0,                FABER_NEW_PROJECT, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Open",             'O', B_COMMAND_KEY,                 'O', 0,                   FABER_FILE_OPEN, FABER_SUBITEM },
	{ "Recents...",           0,                 'O', 0,    0,                 			FABER_RECENTS_MENU, FABER_CUSTOM_ITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Save",             'S', B_COMMAND_KEY,                   0, 0,                   FABER_SAVE_PROJECT, FABER_SUBITEM },
	{ "Save As",             'S', B_COMMAND_KEY | B_SHIFT_KEY,  0, 0,                   FABER_SAVE_AS_PROJECT, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Export",           'E', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,             		FABER_EXPORT_PROJECT, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Quit",                'Q', B_COMMAND_KEY,                   0, 0,                B_QUIT_REQUESTED, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kEditMenu[] {
	{ "Edit", 0, 0, 0, 0, FABER_EDIT_MENU, FABER_ITEM_START },

	{ "Undo",             'Z', B_COMMAND_KEY,                   0, 0,                         FABER_UNDO, FABER_SUBITEM },
	{ "Redo",             'Z', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,                         FABER_REDO, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Copy",             'C', B_COMMAND_KEY,                   0, 0,                         B_COPY, FABER_SUBITEM },
	{ "Copy & Silence",       'X', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,                     FABER_COPY_SILENCE, FABER_SUBITEM },
	{ "Cut",              'X', B_COMMAND_KEY,                   0, 0,                         B_CUT, FABER_SUBITEM },
	{ "Paste",                'V', B_COMMAND_KEY,                   0, 0,                     B_PASTE, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Select All",         'A', B_COMMAND_KEY,                   0, 0,                       B_SELECT_ALL, FABER_SUBITEM },
	{ "Unselect All",       'U', B_COMMAND_KEY,                   0, 0,                       FABER_UNSELECT_ALL, FABER_SUBITEM },
	{ "Invert Selection",   'I', B_COMMAND_KEY,                   0, 0, 	                  FABER_INVERT_SELECTION, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Clear",              'B', B_COMMAND_KEY,            B_DELETE, 0,                       FABER_CLEAR, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kTracksMenu[] {
	{ "Tracks", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ "New Track", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ "Mono Track",             'M', B_COMMAND_KEY | B_CONTROL_KEY,  0, 0,                   FABER_NEW_MONO_TRACK, FABER_SUBITEM },
	{ "Stereo Track",           'S', B_COMMAND_KEY | B_CONTROL_KEY,  0, 0,                   FABER_NEW_STEREO_TRACK, FABER_SUBITEM },
	{ "Multichannel Track",     'P', B_COMMAND_KEY | B_CONTROL_KEY,  0, 0,                   FABER_NEW_MULTICHANNEL_TRACK, FABER_SUBITEM },
    //{ "Label Track",          'L', B_COMMAND_KEY | B_CONTROL_KEY,  0, 0,                   FABER_NEW_LABEL_TRACK, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ "Reorder Tracks",                  0, 0,                           0, 0, 0,               FABER_ITEM_START },

	{ "By Name",             0, 0,                           0, 0,                            FABER_SORT_BY_NAME, FABER_SUBITEM },
	{ "By Duration",           0, 0,                           0, 0,                          FABER_SORT_BY_DURATION, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Mute All",             'M', B_COMMAND_KEY | B_SHIFT_KEY,          0, 0,                FABER_MUTE_ALL, FABER_SUBITEM },
	{ "Unmute All",             'M', B_COMMAND_KEY,                      0, 0,                FABER_UNMUTE_ALL, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kHelpMenu[] {
	{ "Help", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ "Homepage",                  0, 0,                           0, 0,                      FABER_OPEN_HOMEPAGE, FABER_SUBITEM },
	{ "About",                  0, 0,                           0, 0,                         FABER_ABOUT, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kEngineMenu[] {
	{ "Engine", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ "Mixer", 'M', B_CONTROL_KEY | B_COMMAND_KEY,              0, 0,                      FABER_OPEN_MIXER, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Set Output", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Transport", 0, 0, 0, 0, 0, FABER_ITEM_START },

    { "Play",     '1', B_COMMAND_KEY,       0, 0,                             FABER_TRANSPORT_PLAY, FABER_SUBITEM },
    { "Stop",     '2', B_COMMAND_KEY,       0, 0,                             FABER_TRANSPORT_STOP, FABER_SUBITEM },
    { "Rew",   	  '3', B_COMMAND_KEY,       0, 0,                             FABER_TRANSPORT_REW, FABER_SUBITEM },
    { "Rew All",  '4', B_COMMAND_KEY,       0, 0,                             FABER_TRANSPORT_REW_ALL, FABER_SUBITEM },
    { "Forward",  '5', B_COMMAND_KEY,       0, 0,                             FABER_TRANSPORT_FWD, FABER_SUBITEM },
    { "Forward All", '6', B_COMMAND_KEY,    0, 0,                             FABER_TRANSPORT_FWD_ALL, FABER_SUBITEM },
    { "Record",      '7', B_COMMAND_KEY,    0, 0,                             FABER_TRANSPORT_REC, FABER_SUBITEM },
    { "Loop",        '8', B_COMMAND_KEY,    0, 0,                             FABER_TRANSPORT_LOOP, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_EOF }
};

KeyBind kProjectMenu[] {
	{ "Project", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ "Zoom", 0, 0, 0, 0, 0, FABER_ITEM_START },

    { "In",                    B_UP_ARROW, 0,                0,0,                    FABER_ZOOM_IN, FABER_SUBITEM },
    { "Out",                   B_DOWN_ARROW, 0,              0,0,                    FABER_ZOOM_OUT, FABER_SUBITEM },
    { "Full",                  B_UP_ARROW, B_SHIFT_KEY,      0,0,                    FABER_ZOOM_FULL, FABER_SUBITEM },
    { "Selection",          B_DOWN_ARROW, B_SHIFT_KEY,    0,0,                       FABER_ZOOM_SELECTION, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Settings", 'P', B_COMMAND_KEY, 'P', B_SHIFT_KEY, FABER_SETTINGS, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_EOF }
};


KeyBind kTrackContextualMenu[] {
	{ "TrackMenu", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ "Set Name...",                  0, 0,                           0, 0,                  FABER_TRACK_SET_NAME, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Get Informations",                  0, 0,                           0, 0,             FABER_TRACK_GET_INFO, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Select All",           0, 0,                           0, 0,                          B_SELECT_ALL, FABER_SUBITEM },
	{ "Unselect",           0, 0,                           0, 0,                     FABER_UNSELECT_ALL, FABER_SUBITEM },
	{ "Invert Selection",           0, 0,                           0, 0,             FABER_INVERT_SELECTION, FABER_SUBITEM },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Edit", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ "Add channel",           0, 0,                           0, 0,            		FABER_TRACK_ADD_CHAN, FABER_SUBITEM },

    { "Remove channel...",           0, 0,                     0, 0,					FABER_TRACK_RM_CHAN, FABER_ITEM_START},

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Split channels",           0, 0,                           0, 0,                 FABER_TRACK_SPLIT_CHAN, FABER_SUBITEM },

	{ "Merge channels with...",           0, 0,                           0, 0,		  FABER_ITEM_START, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ "Export", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ "To file...",           0, 0,                           0, 0,              FABER_TRACK_EXPORT_NFILE, FABER_SUBITEM },
	{ "To new project...",           0, 0,                           0, 0,       FABER_TRACK_EXPORT_NPROJ, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Set Input", 0, 0, 0, 0, 0, FABER_ITEM_START },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ "", 0, 0, 0, 0, 0, FABER_SPLITTER },

	{ "Move Up", 0, 0, 0, 0, FABER_TRACK_MOVE_UP, FABER_SUBITEM },
	{ "Move Down", 0, 0, 0, 0, FABER_TRACK_MOVE_DOWN, FABER_SUBITEM },

	{ NULL, 0, 0, 0, 0, 0, FABER_ITEM_END },

	{ NULL, 0, 0, 0, 0, 0, FABER_EOF }
};

KeyBind* kDefaultKeymap[] {
	kFileMenu,
	kEditMenu,
	kTracksMenu,
	kHelpMenu,
	kEngineMenu,
	kProjectMenu,
	kTrackContextualMenu,
	NULL
};

#endif
