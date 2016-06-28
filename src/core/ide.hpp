#ifndef ALT_IDE_HPP
#define ALT_IDE_HPP

#include "../main.hpp"
#include "lua.hpp"

namespace alt {

class ide: public wxFrame {

	public:

		ide();
		virtual ~ide();

    	void OnPreferences(wxCommandEvent&);
  		lua &get_lua() { return this->lua; }

	private:
		// wxWindow *m_left, *m_right;

		alt::lua lua;
		wxTreeListCtrl *tree;
		wxSizer *sizer;
		// wxSplitterWindow* m_splitter;
		// wxWindow *m_replacewindow;

		//wxDECLARE_EVENT_TABLE();
		wxDECLARE_NO_COPY_CLASS(ide);
};

}

#endif
