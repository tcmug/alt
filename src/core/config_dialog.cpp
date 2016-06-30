
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


	tree->AppendColumn("Configuration",
					   tree->WidthFor("Something"),
					   wxALIGN_LEFT,
					   wxCOL_RESIZABLE | wxCOL_SORTABLE);

	tree->AppendColumn("Value",
					   tree->WidthFor("The rest of the stuff and more than enough"),
					   wxALIGN_LEFT,
					   wxCOL_RESIZABLE | wxCOL_SORTABLE);

	// wxTreeListItem item, root = tree->GetRootItem();

	// wxTreeListItem parentitem = tree->AppendItem(
	// 	root, "Compiler", 0, 0
	// );

	// item = tree->AppendItem(
	// 	parentitem, "Compiler", 0, 0
	// );
	// tree->SetItemText(item, 1, "g++");

	// item = tree->AppendItem(
	// 	parentitem, "Parameters", 0, 0
	// );
	// tree->SetItemText(item, 1, "-o -Wall STUFF THERE IS IN THIS FIELD");
	// tree->Expand(parentitem);

	// parentitem = tree->AppendItem(
	// 	root, "GIT", 0, 0
	// );

	// item = tree->AppendItem(
	// 	parentitem, "remote", 0, 0
	// );
	// tree->SetItemText(item, 1, "git@ssh.example.com");
	// tree->Expand(parentitem);

	//parentitem->expand();
	//tree->ForceInitialize();  // <<< NEW LINE


	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(tree, wxSizerFlags(2).Expand());
	// .Border(wxALL, 10)
	wxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
	button_sizer->Add(new wxButton(this, wxID_CLOSE, wxT("&Cancel")), 0, wxALIGN_CENTER | wxALL, 5);
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

	lua_pushstring(L, "name");
	lua_gettable(L, -2);
	if (lua_isstring(L, -1)) {
		SetTitle(lua_tostring(L, -1));
	}
	lua_pop(L, 1);

	lua_pushstring(L,"config");
	lua_gettable(L,-2);
	if (lua_isnil(L, -1)) {
		printf("No alt.syntax\n");
		return;
	}

	this->loop_config_tree(tree->GetRootItem());

	tree->SetSortColumn(0);
}


class tree_item: public wxClientData {
	private:

		std::string name;
		std::string value;

	public:

		enum TYPE {
			PARENT,
			STRING,
			BOOLEAN,
			NUMBER
		};

		TYPE type;

		tree_item(TYPE type, const char *name, const char *value) {
			this->name = name;
			this->value = value;
			this->type = type;
		}
		~tree_item() {
			printf("Deleted\n");
		}

};


void config_dialog::loop_config_tree(wxTreeListItem parent) {

	lua_State *L = this->lua->get_state();
	lua_pushnil(L);
	wxTreeListItem item;

	while (lua_next(L, -2) != 0) {
		if (lua_istable(L, -1)) {

			if (lua_isnumber(L, -2)) {

				std::string name = this->lua->get_table_string("name");
				std::string value = this->lua->get_table_string("value");

				item = tree->AppendItem(
					parent,
					name.c_str(),
					-1, -1,
					new tree_item(
						tree_item::STRING,
						name.c_str(),
						value.c_str()
					)
				);

				tree->SetItemText(item, 1, value.c_str());

				// Loop child items.
				lua_pushstring(L, "kids");
				lua_gettable(L, -2);
				if (lua_istable(L, -1)) {
					loop_config_tree(item);
				}
				lua_pop(L, 1);


				//tree->Expand(parent);
			}
		}
		lua_pop(L, 1);
	}

/*

	while (lua_next(L, -2) != 0) {
		if (lua_isstring(L, -1)) {
			// String values.
			const char *key = lua_tostring(L, -2);
			const char *value = lua_tostring(L, -1);
			item = tree->AppendItem(
				parent, key, -1, -1, new tree_item(tree_item::STRING, key, value)
			);
			tree->SetItemText(item, 1, value);
		}
		if (lua_isboolean(L, -1)) {
			// Boolean values.
			const char *key = lua_tostring(L, -2);
			item = tree->AppendItem(
				parent, key, -1, -1, new tree_item(tree_item::BOOLEAN, key, "TRUE")
			);
			tree->SetItemText(item, 1, "TRUE");
		}
		if (lua_isnumber(L, -1)) {
			// Boolean values.
			const char *key = lua_tostring(L, -2);
			item = tree->AppendItem(
				parent, key, -1, -1, new tree_item(tree_item::NUMBER, key, "10")
			);
			tree->SetItemText(item, 1, "10");
		}
		else if (lua_istable(L, -1)) {
			// Table value, two alternatives:
			if (lua_isnumber(L, -2)) {
				// Ignore tables within tables.
				// e.g. { { key = "value "}}
				loop_config_tree(parent);
				tree->Expand(parent);
			}
			else {
				// Entry has child items.
				const char *key = lua_tostring(L, -2);
				const char *value = lua_tostring(L, -1);
				item = tree->AppendItem(
					parent, key, -1, -1, new tree_item(tree_item::PARENT, key, "BLAA")
				);
				tree->SetItemText(item, 1, value);
				loop_config_tree(item);

			}
		}
		lua_pop(L, 1);
	}

 */


	tree->Expand(parent);

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
	wxClientData *data = tree->GetItemData(item);

	tree_item *ti = dynamic_cast<tree_item*>(data);

	if (ti && ti->type != tree_item::PARENT) {
		wxString item_name = tree->GetItemText(item, 0) + ":";
		wxString existing = tree->GetItemText(item, 1);
		wxString hint = wxGetTextFromUser(item_name, "Edit item", existing, this);
		tree->SetItemText(item, 1, hint);
	}
//    wxLogMessage(msg, DumpItem(event.GetItem()));
}
