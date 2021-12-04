#include "editBox.h"

editBox::editBox( wxPanel* parent, int id, wxString title ) : wxStaticBox( parent, id, title )
{

}

void editBox::refreshSerial()
{
    #if defined( __linux__ ) || defined( __FreeBSD__ ) || defined( __APPLE__ )
        this->serialArray.Empty();
        int comportsElements;
        char directory_name[] = "/dev/";
        DIR *ptr;
        struct dirent *directory;
        ptr = opendir( directory_name );
        while( ( directory = readdir( ptr ) ) != NULL )
        {
            #if defined( __APPLE__ )
            if ( strstr( directory->d_name, "cu.usb" ) != NULL )
            #endif
            #if defined( __linux__ ) || defined( __FreeBSD__ )
            if ( strstr( directory->d_name, "ttyUSB" ) != NULL )
            #endif
            {
                char tmp[ 100 ] = "/dev/";
                for ( int i = 0; i < 95; i++ )
                {
                    tmp[ 5+i ] = directory->d_name[ i ];
                }
                this->serialArray.Add( tmp );
            }
        }
    #endif

    #if defined( WIN32 )
        this->serialArray.Empty();
        wchar_t lpTargetPath[ 5000 ]; // buffer to store the path of the COMPORTS

        for ( int i = 0; i < 255; i++ ) // checking ports from COM0 to COM255
        {
            std::wstring str = L"COM" + std::to_wstring( i ); // converting to COM0, COM1, COM2
            DWORD res = QueryDosDevice( str.c_str(), lpTargetPath, 5000 );

            // Test the return value and error if any
            if ( res != 0 ) //QueryDosDevice returns zero if it didn't find an object
            {
                //std::cout << str << ": " << lpTargetPath << std::endl;
                this->serialArray.Add( str );
            }
            if ( ::GetLastError() == ERROR_INSUFFICIENT_BUFFER )
            {
            }
        }
    #endif
}