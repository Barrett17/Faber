#include "DefaultKeymap.h"

KeyBind kFileMenu[] {
	{ 0, "File", 0, 0, 0, 0, ItemMessage(FABER_ITEM_START) },
	{ 1,"New Project",         'N', B_COMMAND_KEY,                   0, 0,                      GeneralMessage(FABER_NEW_PROJECT) },
	{ 1,"Open...",             'O', B_COMMAND_KEY,                 'O', 0,                      GeneralMessage(FABER_FILE_OPEN) },
	{ 1,"Save...",             'S', B_COMMAND_KEY,                   0, 0,                      GeneralMessage(FABER_SAVE_PROJECT) },

	{ 0, "", 0, 0, 0, 0, ItemMessage(FABER_SPLITTER) },

	{ 1,"Export Project...",           'S', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,              GeneralMessage(FABER_EXPORT_PROJECT) },
	{ 1,"Export Selection...",   'S', B_COMMAND_KEY | B_CONTROL_KEY, 'S', B_SHIFT_KEY,          GeneralMessage(FABER_EXPORT_SELECTION) },

	{ 0, "", 0, 0, 0, 0, ItemMessage(FABER_SPLITTER) },

	{ 1,"Insert",              'I', B_COMMAND_KEY,            B_INSERT, B_SHIFT_KEY,            GeneralMessage(FABER_INSERT) },
	{ 1,"Append",              0, 0,                                 0, 0,                      GeneralMessage(FABER_APPEND) },
 
	{ 0, "", 0, 0, 0, 0, ItemMessage(FABER_SPLITTER) },

	{ 1,"Quit",                'Q', B_COMMAND_KEY,                   0, 0,                      GeneralMessage(FABER_QUIT_REQUESTED) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_EOF) }
};

KeyBind kEditMenu[] {
	{ 0, "Edit", 0, 0, 0, 0, ItemMessage(FABER_ITEM_START) },

	{ 1,"Undo",             'Z', B_COMMAND_KEY,                   0, 0,                         GeneralMessage(FABER_UNDO) },
	{ 1,"Redo",             'Z', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,                         GeneralMessage(FABER_REDO) },

	{ 0, "", 0, 0, 0, 0, ItemMessage(FABER_SPLITTER) },

	{ 1,"Copy",             'C', B_COMMAND_KEY,                   0, 0,                         GeneralMessage(FABER_COPY) },
	{ 1,"Copy & Silence",       'X', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,                     GeneralMessage(FABER_COPY_SILENCE) },
	{ 1,"Cut",              'X', B_COMMAND_KEY,                   0, 0,                         GeneralMessage(FABER_CUT) },
	{ 1,"Paste",                'V', B_COMMAND_KEY,                   0, 0,                     GeneralMessage(FABER_PASTE) },
	{ 1,"Paste to new project",         'V', B_COMMAND_KEY | B_SHIFT_KEY,     0, 0,             GeneralMessage(FABER_PASTE_NEW) },

	{ 0, "", 0, 0, 0, 0, ItemMessage(FABER_SPLITTER) },

	{ 1,"Select All",         'A', B_COMMAND_KEY,                   0, 0,                       GeneralMessage(FABER_SELECT_ALL) },
	{ 1,"Unselect All",       'U', B_COMMAND_KEY,                   0, 0,                       GeneralMessage(FABER_UNSELECT_ALL) },

	{ 0, "", 0, 0, 0, 0, ItemMessage(FABER_SPLITTER) },

	{ 0, "Zero Cross", 0, 0, 0, 0, ItemMessage(FABER_ITEM_START) },

    { 1,"Zero In",            0, 0,                               0, 0,                         GeneralMessage(FABER_ZERO_IN) },
    { 1,"Zero Out",           0, 0,                               0, 0,                         GeneralMessage(FABER_ZERO_OUT) },
    { 1,"Zero LL",            0, 0,                               0, 0,                         GeneralMessage(FABER_ZERO_LL) },
    { 1,"Zero LR",            0, 0,                               0, 0,                         GeneralMessage(FABER_ZERO_LR) },
    { 1,"Zero RL",            0, 0,                               0, 0,                         GeneralMessage(FABER_ZERO_RL) },
    { 1,"Zero RR",            0, 0,                               0, 0,                         GeneralMessage(FABER_ZERO_RR) },



	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	//{ 1,"Set frequency",      0, 0,                               0, 0,                         FABER_SET_FREQUENCY) },
	//{ 1,"Resample",           0, 0,                               0, 0,                         FABER_RESAMPLE) },

    { 1,"Trim",               0, 0,                               0, 0,                         GeneralMessage(FABER_TRIM) },
	{ 1,"Clear",              'B', B_COMMAND_KEY,            B_DELETE, 0,                       GeneralMessage(FABER_CLEAR) },

	{ 0, "", 0, 0, 0, 0, ItemMessage(FABER_SPLITTER) },

	{ 1,"Settings...",      'P', B_COMMAND_KEY,                 'P', B_SHIFT_KEY,            GeneralMessage(FABER_SETTINGS) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_EOF) }
};

KeyBind kTracksMenu[] {
	{ 0, "Track", 0, 0, 0, 0, ItemMessage(FABER_ITEM_START) },

    { 1,"New Track",                  0, 0,                           0, 0,                     GeneralMessage(FABER_ITEM_START) },

    { 1,"New Mono Track",             0, 0,                           0, 0,                     GeneralMessage(FABER_NEW_EMPTY_TRACK) },
    { 1,"New Stereo Track",           0, 0,                           0, 0,                     GeneralMessage(FABER_NEW_EMPTY_TRACK) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_EOF) }
};

KeyBind kHelpMenu[] {
	{ 0, "Help", 0, 0, 0, 0, ItemMessage(FABER_ITEM_START) },

    { 1,"Homepage",                  0, 0,                           0, 0,                      GeneralMessage(FABER_OPEN_HOMEPAGE) },
    { 1,"About",                  0, 0,                           0, 0,                         GeneralMessage(FABER_ABOUT) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_EOF) }
};

KeyBind kPlayButtons[] {
	{ 0, "Transport Widget", 0, 0, 0, 0, ItemMessage(FABER_ITEM_START) },

    { 0,"Play",     ' ', B_CONTROL_KEY,       0, 0,                                             GeneralMessage(FABER_TRANSPORT_PLAY) },
    { 0,"Stop",       0, 0,                   0, 0,                                             GeneralMessage(FABER_TRANSPORT_STOP) },
    { 0,"Rew",        0, 0,                   0, 0,                                             GeneralMessage(FABER_TRANSPORT_REW) },
    { 0,"Rew All",B_HOME, B_SHIFT_KEY,        0, 0,                                             GeneralMessage(FABER_TRANSPORT_REW_ALL) },
    { 0,"Forward",        0, 0,               0, 0,                                             GeneralMessage(FABER_TRANSPORT_FWD) },
    { 0,"Forward All",B_END, B_SHIFT_KEY,     0, 0,                                             GeneralMessage(FABER_TRANSPORT_FWD_ALL) },
    { 0,"Record",         0, 0,               0, 0,                                             GeneralMessage(FABER_TRANSPORT_REC) },
    { 0,"Loop",     'L', 0,                   0, 0,                                             GeneralMessage(FABER_TRANSPORT_LOOP) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_EOF) }
};

KeyBind kZoomBar[] {
	{ 0, "Zoom Widget", 0, 0, 0, 0, ItemMessage(FABER_ITEM_START) },

    { 0,"Zoom In",                    B_UP_ARROW, 0,                0,0,                       GeneralMessage(FABER_ZOOM_IN) },
    { 0,"Zoom Out",                   B_DOWN_ARROW, 0,              0,0,                       GeneralMessage(FABER_ZOOM_OUT) },
    { 0,"Zoom Full",                  B_UP_ARROW, B_SHIFT_KEY,      0,0,                       GeneralMessage(FABER_ZOOM_FULL) },
    { 0,"Zoom to Selection",          B_DOWN_ARROW, B_SHIFT_KEY,    0,0,                       GeneralMessage(FABER_ZOOM_SELECTION) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_EOF) }
};

KeyBind kToolsBar[] {
	{ 0, "Tools Widget", 0, 0, 0, 0, ItemMessage(FABER_ITEM_START) },

    { 0,"Selection Tool",                 0, 0,                 '1', 0,                        GeneralMessage(FABER_SELECTION_TOOL) },
    { 0,"Draw tool",                      0, 0,                 '2', 0,                        GeneralMessage(FABER_DRAW_TOOL) },
    { 0,"Play tool",                      0, 0,                 '3', 0,                        GeneralMessage(FABER_PLAY_TOOL) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_ITEM_END) },

	{ 0, NULL, 0, 0, 0, 0, ItemMessage(FABER_EOF) }
};

KeyBind* kDefaultKeymap[] {
	kFileMenu,
	kEditMenu,
	kTracksMenu,
	kHelpMenu,
	kPlayButtons,
	kToolsBar,
	kZoomBar,
	NULL
};
