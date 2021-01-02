#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include <wx/statbox.h>
#include <wx/spinctrl.h>

class pfEditBox : public wxStaticBox
{ 
    public:
        pfEditBox(wxPanel* parent, int id, wxString title);

        enum
        {
            ID_ADD = 542,
            ID_REMOVE = 1666,
            ID_NEW = 512
        };
};

pfEditBox::pfEditBox(wxPanel* parent, int id, wxString title) : wxStaticBox(parent, id, title)
{   
    wxFlexGridSizer* topSizer = new wxFlexGridSizer(2, 10, 10);


    wxStaticText* nameTxt = new wxStaticText(this, wxID_ANY, "Train Name:");
    wxTextCtrl* trainName = new wxTextCtrl(this, wxID_ANY);

    wxStaticText* portTxt = new wxStaticText(this, wxID_ANY, "Arduino ComPort:");
    wxArrayString ports;
    ports.Add("/dev/abc");
    ports.Add("/dev/def");
    wxChoice* portPicker = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ports);

    wxStaticText* gpioTxt = new wxStaticText(this, wxID_ANY, "GPIO:");
    wxSpinCtrl* gpioPicker = new wxSpinCtrl(this, wxID_ANY, "13", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS);

    wxStaticText* channelTxt = new wxStaticText(this, wxID_ANY, "PowerFunctions Channel:");
    wxArrayString channel;
    channel.Add("1");
    channel.Add("2");
    channel.Add("3");
    channel.Add("4");
    wxChoice* channelPicker = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel);

    wxStaticText* subChannelTxt = new wxStaticText(this, wxID_ANY, "PowerFunctions SubCannel:");
    wxArrayString subChannel;
    subChannel.Add("Red");
    subChannel.Add("Blue");
    wxChoice* subChannelPicker = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, subChannel);

    wxStaticText* maxSpeedTxt = new wxStaticText(this, wxID_ANY, "Train Max Speed");
    wxSpinCtrl* maxSpeedPicker = new wxSpinCtrl(this, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 7);

    topSizer->Add(nameTxt, 0, wxALL | (wxALL & ~wxLEFT), 5);
    topSizer->Add(trainName, 0, wxALL | (wxALL & ~wxLEFT), 5);

    topSizer->Add(portTxt, 0, wxALL | (wxALL & ~wxLEFT), 5);
    topSizer->Add(portPicker, 0, wxALL | (wxALL & ~wxLEFT), 5);

    topSizer->Add(gpioTxt, 0, wxALL | (wxALL & ~wxLEFT), 5);
    topSizer->Add(gpioPicker, 0, wxALL | (wxALL & ~wxLEFT), 5);

    topSizer->Add(channelTxt, 0, wxALL | (wxALL & ~wxLEFT), 5);
    topSizer->Add(channelPicker, 0, wxALL | (wxALL & ~wxLEFT), 5);

    topSizer->Add(subChannelTxt, 0, wxALL | (wxALL & ~wxLEFT), 5);
    topSizer->Add(subChannelPicker, 0, wxALL | (wxALL & ~wxLEFT), 5);

    topSizer->Add(maxSpeedTxt, 0, wxALL | (wxALL & ~wxLEFT), 5);
    topSizer->Add(maxSpeedPicker, 0, wxALL | (wxALL & ~wxLEFT), 5);
    
    // Top Sizer Settings
    parent->SetSizer(topSizer);
    parent->Layout();
	topSizer->Fit(this);
    topSizer->SetSizeHints(this);
}