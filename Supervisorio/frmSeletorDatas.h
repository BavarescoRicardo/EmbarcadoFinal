//---------------------------------------------------------------------------
#ifndef frmSeletorDatasH
#define frmSeletorDatasH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.DateUtils.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFormSeletorDatas : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *gpFaixaDatas;
	TLabel *lblDataInicial;
	TDateTimePicker *eDataInicial;
	TDateTimePicker *eDataFinal;
	TButton *btnConfirmar;
	TLabel *lblDataFinal;
	void __fastcall btnConfirmarClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall eDataInicialChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSeletorDatas(TComponent* Owner);
    void AtualizarDataFinal();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSeletorDatas *FormSeletorDatas;
//---------------------------------------------------------------------------
#endif
