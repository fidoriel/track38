#ifndef preferences_h
#define preferences_h

#include "wx/wx.h"
#include "wx/preferences.h"

class track38PreferencePanel : public wxPanel
{
public:
    track38PreferencePanel(wxWindow *parent);
};

class track38PreferencePageGeneral : public wxStockPreferencesPage
{
public:
    track38PreferencePageGeneral() : wxStockPreferencesPage( Kind_General ) {}
    virtual wxWindow *CreateWindow( wxWindow *parent ) wxOVERRIDE
    { 
        return new track38PreferencePanel( parent );
    }
};
#endif