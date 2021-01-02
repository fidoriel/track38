#include "pfeditbox.h"

pfEditBox::pfEditBox(wxPanel* parent, int id, wxString title) : wxStaticBox(parent, id, title)
{   
    wxFlexGridSizer* topSizerPfEdit = new wxFlexGridSizer(2, 0, 0);


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
    wxSpinCtrl* maxSpeedPicker = new wxSpinCtrl(this, wxID_ANY, "10", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 7);

    topSizerPfEdit->AddSpacer(10);
    topSizerPfEdit->AddSpacer(10);
    topSizerPfEdit->Add(nameTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPfEdit->Add(trainName, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPfEdit->Add(portTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPfEdit->Add(portPicker, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPfEdit->Add(gpioTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPfEdit->Add(gpioPicker, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPfEdit->Add(channelTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPfEdit->Add(channelPicker, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPfEdit->Add(subChannelTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPfEdit->Add(subChannelPicker, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPfEdit->Add(maxSpeedTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPfEdit->Add(maxSpeedPicker, 0, wxALL | (wxALL & ~wxLEFT), 10);
    
    // Top Sizer Settings
    parent->SetSizer(topSizerPfEdit);
    parent->Layout();
	topSizerPfEdit->Fit(this);
    topSizerPfEdit->SetSizeHints(this);
}