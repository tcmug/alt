
#include "ide.hpp"
#include "lua.hpp"
#include "config_dialog.hpp"

#include <wx/button.h>

using alt::config_dialog;


enum {
	wxID_SavePreferences,
};


wxBEGIN_EVENT_TABLE(config_dialog, wxDialog)
    EVT_TREELIST_ITEM_ACTIVATED(wxID_ANY, config_dialog::OnSelection)
    EVT_BUTTON(wxID_SavePreferences, config_dialog::OnSave)
wxEND_EVENT_TABLE()





config_dialog::config_dialog(wxWindow *parent)
	:wxDialog(parent, wxID_ANY, wxString(wxT("...")),
	wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {

	SetEscapeId(wxID_CLOSE);

	tree = new wxTreeListCtrl(this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxTL_DEFAULT_STYLE
	);

	//tree->SetImageList(m_imageList);

/*
	wxTreeListItem item = tree->AppendItem(m_isFlat ? root : parent, \
										   #item, \
										   Icon_FolderClosed, \
										   Icon_FolderOpened); \
   tree->SetItemText(item, Col_Files, files);
   tree->SetItemText(item, Col_Size, size)
*/

   	this->lua = &dynamic_cast <ide*>(parent)->get_lua();
	SetTitle(wxT("Configuration"));

	tree->AppendColumn("Configuration",
					   tree->WidthFor("Something"),
					   wxALIGN_LEFT,
					   wxCOL_RESIZABLE | wxCOL_SORTABLE);

	tree->AppendColumn("Value",
					   tree->WidthFor("The rest of the stuff and more than enough"),
					   wxALIGN_LEFT,
					   wxCOL_RESIZABLE | wxCOL_SORTABLE);

	wxTreeListItem item, root = tree->GetRootItem();

	wxTreeListItem parentitem = tree->AppendItem(
		root, "Compiler", 0, 0
	);

	item = tree->AppendItem(
		parentitem, "Compiler", 0, 0
	);
	tree->SetItemText(item, 1, "g++");

	item = tree->AppendItem(
		parentitem, "Parameters", 0, 0
	);
	tree->SetItemText(item, 1, "-o -Wall STUFF THERE IS IN THIS FIELD");
	tree->Expand(parentitem);

	parentitem = tree->AppendItem(
		root, "GIT", 0, 0
	);

	item = tree->AppendItem(
		parentitem, "remote", 0, 0
	);
	tree->SetItemText(item, 1, "git@ssh.example.com");
	tree->Expand(parentitem);

	//parentitem->expand();
	//tree->ForceInitialize();  // <<< NEW LINE
	tree->Expand(parentitem);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(tree, wxSizerFlags(2).Expand());
	// .Border(wxALL, 10)
	wxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
	button_sizer->Add(new wxButton(this, wxID_ANY, wxT("&Cancel")), 0, wxALIGN_CENTER | wxALL, 5);
	button_sizer->Add(new wxButton(this, wxID_SavePreferences, wxT("&Save")), 0, wxALIGN_CENTER | wxALL, 5);
	sizer->Add(button_sizer, 0, wxALIGN_RIGHT | wxALL, 5);
	SetSizer(sizer);


	// m_btnModal->SetFocus();
	// m_btnModal->SetDefault();

}


void config_dialog::init_by_function(const char *get_func, const char *set_func) {

	lua_State *L = this->lua->get_state();

	this->set_callback = set_func;

	lua_getglobal(L, get_func);
	if (!lua_isfunction(L, -1)) {
		lua_remove(L, -1);
		printf("No such function: ");
		printf(get_func);
		printf("\n");
		return;
	}

	lua_pcall(L, 0, 1, 0);
	if (lua_isnil(L, -1)) {
		printf("No alt\n");
		return;
	}

	lua_pushstring(L,"save");
	lua_gettable(L,-2);
	if (lua_isnil(L, -1)) {
		printf("No alt.save\n");
		return;
	}

	lua_pcall(L, 0, 0, 0);

	lua_pushstring(L,"syntax");
	lua_gettable(L,-2);
	if (lua_isnil(L, -1)) {
		printf("No alt.syntax\n");
		return;
	}

	lua_remove(L,-2);
	lua_pushstring(L,"highlighting");
	lua_gettable(L,-2);
	lua_remove(L,-2);

	for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
		const char *c = lua_tostring(L, -2);
		printf("%s\n", c);
		for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
			const char *c = lua_tostring(L, -2);
			printf(" - %s\n", c);
			if (lua_istable(L, -1)) {
				for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
					const char *c = lua_tostring(L, -2);
					printf("   - %s\n", c);
					if (lua_istable(L, -1)) {
						for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
							int i = lua_tointeger(L, -2);
							printf("   - %u\n", i);
							if (lua_istable(L, -1)) {
								for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
									const char *c = lua_tostring(L, -2);
									const char *p = lua_tostring(L, -1);
									printf("       - %s %s\n", c, p);
								}
							}
						}
					}
				}
			}
		}
	}

}


void config_dialog::OnSave(wxCommandEvent& WXUNUSED(event)) {
	lua_State *L = this->lua->get_state();

	lua_getglobal(L, this->set_callback);
	if (!lua_isfunction(L, -1)) {
		lua_remove(L, -1);
		printf("No such function: ");
		printf(this->set_callback);
		printf("\n");
		return;
	}

	lua_pushstring(L,"highlighting");
	lua_pcall(L, 1, 1, 0);
	if (lua_isboolean(L, -1) && lua_toboolean(L, -1)) {
		lua_pop(L, 1);
		Close();
		return;
	}
	lua_pop(L, 1);
}



void config_dialog::OnSelection(wxTreeListEvent& event)
{
    const char* msg;

    wxTreeListItem item = event.GetItem();
    wxString item_name = tree->GetItemText(item, 0) + ":";
    wxString existing = tree->GetItemText(item, 1);
    wxString hint = wxGetTextFromUser(item_name, "Edit item", existing, this);

    tree->SetItemText(item, 1, hint);

//    wxLogMessage(msg, DumpItem(event.GetItem()));
}
