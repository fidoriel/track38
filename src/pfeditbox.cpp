#include "pfeditbox.h"

pfEditBox::pfEditBox(wxPanel* parent, int id, wxString title) : wxStaticBox(parent, id, title)
{   
    topSizerPfEdit = new wxFlexGridSizer(2, 0, 0);


    nameTxt = new wxStaticText(this, wxID_ANY, "Train Name:");
    trainName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, -1));

    portTxt = new wxStaticText(this, wxID_ANY, "Arduino ComPort:");
    this->refreshSerial();
    portPicker = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300, -1), serialArray);

    gpioTxt = new wxStaticText(this, wxID_ANY, "GPIO:");
    gpioPicker = new wxSpinCtrl(this, wxID_ANY, "13", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS);

    channelTxt = new wxStaticText(this, wxID_ANY, "PowerFunctions Channel:");
    channel.Add("1");
    channel.Add("2");
    channel.Add("3");
    channel.Add("4");
    channelPicker = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel);

    subChannelTxt = new wxStaticText(this, wxID_ANY, "PowerFunctions SubCannel:");
    subChannel.Add("Red");
    subChannel.Add("Blue");
    subChannelPicker = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, subChannel);

    maxSpeedTxt = new wxStaticText(this, wxID_ANY, "Train Max Speed");
    maxSpeedPicker = new wxSpinCtrl(this, wxID_ANY, "10", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 7);

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

void pfEditBox::refreshSerial()
{
    #if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
        this->serialArray.Empty();
        int comportsElements;
        char directory_name[] = "/dev/";
        DIR *ptr;
        struct dirent *directory;
        ptr = opendir(directory_name);
        while((directory = readdir(ptr)) != NULL)
        {
            if ((strstr(directory->d_name, "tty.") != NULL) || (strstr(directory->d_name, "cu.") != NULL))
            {
                char tmp[100] = "/dev/";
                for (int i = 0; i < 95; i++)
                {
                    tmp[5+i] = directory->d_name[i];
                }
                this->serialArray.Add(tmp);
            }
        }
    #endif

    #if defined(WIN32)
        this->serialArray.Empty();
        char lpTargetPath[5000];

        for (int i = 0; i < 255; i++) // checking ports from COM0 to COM255
        {
            std::string str = "COM" + std::to_string(i); // converting to COM0, COM1, COM2
            DWORD test = QueryDosDevice(str.c_str(), lpTargetPath, 5000);

            // Test the return value and error if any
            if (test != 0) //QueryDosDevice returns zero if it didn't find an object
            {
                //std::cout << str << ": " << lpTargetPath << std::endl;

                std::string tmp = str + " | " + lpTargetPath;
                this->serialArray.Add(tmp);
            }

            if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
            }
        }
    #endif
}