#include "headers/mainmenu.h" 

using namespace System;
using namespace System::Windows::Forms;
using namespace WindowsForm;

[STAThread]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew mainMenu());
    return 0;
}
