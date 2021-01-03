#include "pupeditbox.h"

pupEditBox::pupEditBox(wxPanel* parent, int id, wxString title) : wxStaticBox(parent, id, title)
{
    topSizerPupEdit = new wxFlexGridSizer(2, 0, 0);

    nameTxt = new wxStaticText(this, wxID_ANY, "Train Name:");
    trainName = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, -1));

    portTxt = new wxStaticText(this, wxID_ANY, "Arduino ComPort:");
    this->refreshSerial();
    portPicker = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(300, -1), serialArray);

    hubAdressTxt = new wxStaticText(this, wxID_ANY, "Hub Adress:");
    hubAdress = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, -1));

    channelTxt = new wxStaticText(this, wxID_ANY, "PoweredUP Hub Channel:");
    channel.Add("A");
    channel.Add("B");
    channelPicker = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel);

    hasTwoMotorsTxt = new wxStaticText(this, wxID_ANY, "Two Motors used:");
    hasTwoMotors = new wxCheckBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);

    maxSpeedTxt = new wxStaticText(this, wxID_ANY, "Train Max Speed");
    maxSpeedPicker = new wxSpinCtrl(this, wxID_ANY, "7", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10);

    topSizerPupEdit->AddSpacer(10);
    topSizerPupEdit->AddSpacer(10);
    
    topSizerPupEdit->Add(nameTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPupEdit->Add(trainName, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPupEdit->Add(portTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPupEdit->Add(portPicker, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPupEdit->Add(hubAdressTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPupEdit->Add(hubAdress, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPupEdit->Add(channelTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPupEdit->Add(channelPicker, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPupEdit->Add(hasTwoMotorsTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPupEdit->Add(hasTwoMotors, 0, wxALL | (wxALL & ~wxLEFT), 10);

    topSizerPupEdit->Add(maxSpeedTxt, 0, wxALL | (wxALL & ~wxLEFT), 10);
    topSizerPupEdit->Add(maxSpeedPicker, 0, wxALL | (wxALL & ~wxLEFT), 10);
    
    // Top Sizer Settings
    parent->SetSizer(topSizerPupEdit);
    parent->Layout();
	topSizerPupEdit->Fit(this);
    topSizerPupEdit->SetSizeHints(this);
    SendSizeEvent();
}

void pupEditBox::refreshSerial()
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