#ifndef CONFIG_CONTROL_HPP
#define CONFIG_CONTROL_HPP

namespace alt {

/*

// A custom modal dialog
class MyModalDialog : public wxDialog
{
public:
    MyModalDialog(wxWindow *parent);

    void OnButton(wxCommandEvent& event);

private:
    wxButton *m_btnModal,
             *m_btnModeless,
             *m_btnDelete;

    wxDECLARE_EVENT_TABLE();
};


*/


class config_dialog: public wxDialog {

    private:

        wxTreeListCtrl *tree;
        wxSizer *sizer;
        lua *lua;
        const char *set_callback;

    public:


        config_dialog(wxWindow *parent);

        void init_by_function(const char *get_func, const char *set_func);

        void OnSelection(wxTreeListEvent& event);
        void OnSave(wxCommandEvent& WXUNUSED(event));

        wxDECLARE_EVENT_TABLE();
};


}

#endif
