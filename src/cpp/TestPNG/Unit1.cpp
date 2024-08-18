//---------------------------------------------------------------------------

#include <vcl.h>

// #define STRICT
#include <windows.h>
#include <algorithm>
using std::min;
using std::max;
#include <gdiplus.h>

#pragma hdrstop

#include "Unit1.h"
#include "pngdib.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
//---------------------------------------------------------------------------

__fastcall TForm1::~TForm1()
{
    //shutdown GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	PNGDIB *pngdib;
    LPBITMAPINFOHEADER lpdib;
    int lpdib_size, lpdib_bits_offs;
	void *lpbits;

	pngdib = pngdib_p2d_init();

	if(!pngdib)
    {
	  return;
    }

	pngdib_p2d_set_png_filename(pngdib, _T("..\\..\\..\\..\\data\\testpng\\bkg_mission_select.png"));
	pngdib_p2d_set_use_file_bg(pngdib, 0);

	//pngdib_p2d_set_custom_bg(pngdib,GetRValue(0x909090),GetGValue(0x909090),GetBValue(0x909090));
    //pngdib_set_dibalpha32(pngdib, 1);

	//pngdib_p2d_set_gamma_correction(pngdib, 1, PNGDIB_DEFAULT_SCREEN_GAMMA);

	int ret = pngdib_p2d_run(pngdib);

    // returns 0 on success; yeah, that's real programming techniques! :)
	if (ret)
    {
		pngdib_done(pngdib);
        return;
	}

    int bpp = pngdib_p2d_get_bitsperpixel(pngdib);

	pngdib_p2d_get_dib(pngdib, &lpdib, &lpdib_size);
	pngdib_p2d_get_dibbits(pngdib, &lpbits, &lpdib_bits_offs, 0);

    /*
	unsigned char bg_r, bg_g, bg_b;

	if(pngdib_p2d_get_bgcolor(pngdib,&bg_r,&bg_g,&bg_b))
    {
		int image_bg = RGB(bg_r,bg_g,bg_b);
		int which_bg=1;
	}
	else
    {
		int which_bg=0;
	}
    */

    /*
    tmpBmp->Canvas->Brush->Color = 0x00000000;
    tmpBmp->Canvas->Brush->Style = bsClear;
    tmpBmp->Canvas->FillRect(TRect(0, 0, lpdib->biWidth, lpdib->biHeight));
    tmpBmp->TransparentMode = tmFixed;
    */

    int bitCount = lpdib->biBitCount;

    // set the bits
    //SetDIBits(GetDC(0), tmpBmp->Handle, 0, lpdib->biHeight, lpbits, (LPBITMAPINFO)lpdib, DIB_RGB_COLORS);

    StretchDIBits(PaintBox1->Canvas->Handle,
					0,0,lpdib->biWidth,lpdib->biHeight,
					0,0,lpdib->biWidth,lpdib->biHeight,
					lpbits,(LPBITMAPINFO)lpdib,
					DIB_RGB_COLORS,SRCCOPY);

	// get rid of any remaining data
	pngdib_done(pngdib);

	if(lpdib)
    {
      pngdib_p2d_free_dib(0, lpdib);
      lpdib = 0;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    Gdiplus::Graphics graphics(PaintBox1->Canvas->Handle); //Handle to the device context
    //Load the image from a file
    Gdiplus::Image image(L"..\\..\\..\\..\\data\\testpng\\char_penguin_shooter.png");
    graphics.DrawImage(&image, 0, 0, 1.5*image.GetWidth(), 1.5*image.GetHeight());
}
//---------------------------------------------------------------------------
