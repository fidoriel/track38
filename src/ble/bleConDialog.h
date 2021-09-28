#ifndef bleDialog_h
#define bleDialog_h

#include "wx/wx.h"
#include "wx/dialog.h"
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include "wx/panel.h"

#include <thread>
#include <future>

#include "bleSearch.h"

class bleConDialog : public wxDialog
{
public:
    bleConDialog(wxWindow *parent);
    bool Create();

    wxSizer* topSizer;
    wxSizer* buttonSizer;
    wxListBox* bleDeviceList;
    wxButton* selDevButton;
    wxButton* cancelButton;
};

#endif