#ifndef ALT_IDE_HPP
#define ALT_IDE_HPP

#include "../main.hpp"

class alt_ide: public wxFrame {

	public:

		alt_ide();
		virtual ~alt_ide();

	private:
		// wxWindow *m_left, *m_right;

		// wxSplitterWindow* m_splitter;
		// wxWindow *m_replacewindow;

		//wxDECLARE_EVENT_TABLE();
		wxDECLARE_NO_COPY_CLASS(alt_ide);
};

#endif
