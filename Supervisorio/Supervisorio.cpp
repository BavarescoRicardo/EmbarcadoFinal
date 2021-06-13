//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <tchar.h>
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("frmMonitorarDados.cpp", FormMonitorarDados);
USEFORM("frmPrincipal.cpp", FormPrincipal);
USEFORM("frmSeletorDatas.cpp", FormSeletorDatas);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormPrincipal), &FormPrincipal);
		Application->CreateForm(__classid(TFormMonitorarDados), &FormMonitorarDados);
		Application->CreateForm(__classid(TFormSeletorDatas), &FormSeletorDatas);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
