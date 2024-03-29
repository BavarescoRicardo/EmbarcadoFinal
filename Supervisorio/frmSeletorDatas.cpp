//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "frmSeletorDatas.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSeletorDatas *FormSeletorDatas;
//---------------------------------------------------------------------------
__fastcall TFormSeletorDatas::TFormSeletorDatas(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSeletorDatas::btnConfirmarClick(TObject *Sender)
{
	if (eDataInicial->Date > eDataFinal->Date)
	{
		ShowMessage("A Data Inicial deve ser maior do que a Data Final.");
		return;
	}

	this->ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormSeletorDatas::FormShow(TObject *Sender)
{
	AtualizarDataFinal();
}
//---------------------------------------------------------------------------
void __fastcall TFormSeletorDatas::eDataInicialChange(TObject *Sender)
{
	AtualizarDataFinal();
}
//---------------------------------------------------------------------------
void TFormSeletorDatas::AtualizarDataFinal()
{
   	// Incrementa a Data Final em dois dias a partir da Data Inicial
	eDataFinal->Date = IncDay(eDataInicial->Date, 2);
}
