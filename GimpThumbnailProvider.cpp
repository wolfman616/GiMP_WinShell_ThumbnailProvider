// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include <combaseapi.h>
#include <thumbcache.h> // For IThumbnailProvider.
#include <Wincrypt.h>   // For CryptStringToBinary.
#include <wincodec.h>   // Windows Imaging Codecs
#include <shlwapi.h>
#include <stdexcept>
#include <AtlBase.h>
#include <atlconv.h>
#include <msxml6.h>
#include <iostream>
#include <ShlObj.h>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <locale>
#include <string>
#include <new>
#include <codecvt>        // std::codecvt_utf8_utf16
using namespace std;

std::wstring WStrGlobal;

template <class T> void SafeRelease(T** ppT) {
    if(*ppT) {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"msxml6.lib")

//    Using Process isolation override.
//    Implementing IInitializeWithFile to work toward being hosted in an un-isolated Explorer process *, promoting succesful Spaghetti-like nature, Thumbnail & Preview.
class CGimpThumbProvider : public IInitializeWithFile, public IThumbnailProvider {
  public:
    CGimpThumbProvider() : _cRef(1), _pStream(NULL) {
    }

//  Release sequence.
    virtual ~CGimpThumbProvider() {
        if(_pStream) {
            _pStream->Release();
        }
    }
//  IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv) {   
        static const QITAB qit[] = {
            QITABENT(CGimpThumbProvider, IInitializeWithFile),
            QITABENT(CGimpThumbProvider, IThumbnailProvider),
            { 0 },
        }; return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef() {
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release() {
        ULONG cRef = InterlockedDecrement(&_cRef);
        if(!cRef) {
            delete this;
        } return cRef;
    }

//  IInitializeWithfile
    IFACEMETHODIMP Initialize(LPCWSTR pszFilePath,DWORD grfMode);
//  IThumbnailProvider
    IFACEMETHODIMP GetThumbnail(UINT cx,HBITMAP* phbmp,WTS_ALPHATYPE* pdwAlpha);

private: //HRESULT _LoadXMLDocument( IXMLDOMDocument **ppXMLDoc);
    HRESULT _GetBase64EncodedImageString(UINT cx, PWSTR* ppszResult);
    HRESULT _GetStreamFromString(PCWSTR pszImageName, IStream** ppStream);
    long _cRef;
    IStream* _pStream;     // provided during initialization.
};

HRESULT CRecipeThumbProvider_CreateInstance(REFIID riid, void** ppv) {
    CGimpThumbProvider* pNew= new (std::nothrow) CGimpThumbProvider();
    HRESULT hr= pNew ? S_OK : E_OUTOFMEMORY;
    if(SUCCEEDED(hr)) {
        hr= pNew->QueryInterface(riid, ppv);
        pNew->Release();
    } return hr;
}

// IInitializeWithFile
IFACEMETHODIMP CGimpThumbProvider::Initialize(LPCWSTR pszFilePath, DWORD grfMode) { 
    HRESULT hr= E_UNEXPECTED;  // can only be inited once
    if (pszFilePath != NULL) {
        //  take a reference to the stream if we have not been inited yet
       //  hr= pszFilePath->QueryInterface(&_pStream);
      //  MessageBoxW(NULL, (LPCWSTR)pszFilePath, (LPCWSTR)L"SPIresult",MB_OK|MB_SETFOREGROUND);
        unsigned int4 = 2;
        std::stringstream ass;
        // string string1= WStrGlobal;
        // string string2= ("\"" + string1 + "\"") ;
      //  string string1="C:\\Script\\AHK\\__TESTS\\New folder\\0107.xcf";
       //   std::string narrow = string4.to_bytes(pszFilePath);
        std::wstring string4 = pszFilePath;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
        std::string string1 = conv1.to_bytes(string4);
        std::cout << "UTF-8 conversion produced " << string1.size() << " bytes:\n";
        // wide to UTF-16le
        std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>> conv2;
        //   std::string u16str = conv2.to_bytes(string4);
         // MessageBoxA(NULL, string1.c_str(), "u8",MB_OK|MB_SETFOREGROUND);
         // MessageBoxA(NULL, u16str.c_str(), "u16",MB_OK|MB_SETFOREGROUND);

         //  std::cout << "UTF-16le conversion produced " << u16str.size() << " bytes:\n";
        string string2 = "\"" + string1 + "\"";
        LPCSTR command1 = "open";
        //   LPCSTR LPFILE= "\"C:\\Script\\AHK\\__TESTS\\shell xcf to thumb (gimp.ahk\"";
        LPCSTR LPFILE = "\"C:\\Script\\AHK\\z_ConTxt\\GIMP_XCF_Shell_Thumb_Helper.ahk\"";

        LPCSTR lpParameters = string2.c_str();
        ShellExecuteA(NULL, command1, LPFILE, lpParameters, NULL, 0);
        int4 = strlen(string1.c_str());
        string string3 = ".txt";
        string1.replace(int4 - 4, 4, string3);
        // std::ifstream infile(string1.c_str());
        BOOL HAR = 0;
        unsigned stim = 1;
        void sleep(unsigned stime);
        //  while(HAR != 1) {
       //     void sleep(unsigned stime);
      //    HAR= (infile.good());
     //   MessageBoxA(NULL,(LPCSTR)string1.c_str(),"harrr",MB_OK|MB_SETFOREGROUND);
    //    }
   //   MessageBoxA(NULL, (LPCSTR)string1.c_str(), "harrr",MB_OK|MB_SETFOREGROUND);
        std::ifstream ifs(string1);
        std::string content;
        content.assign((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));
        IStream* _pStream;
        void sleep(unsigned stime);
        //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> content;
       // std::string narrow = content.to_bytes(wide_utf16_source_string);
      //  std
        CA2W ca2w(content.c_str());
        WStrGlobal = ca2w;
        std::wstring wFileName(string1.begin(), string1.end());
        BOOL res = NULL;
        //   MessageBoxW(NULL, WStrGlobal.c_str(), L"jizzy",MB_OK|MB_SETFOREGROUND);
     //  ; while (res == 0) {
            void sleep(unsigned stime);
            res = DeleteFileA(string1.c_str());
        if (res != 1) {
            void sleep(unsigned stime);
            res = DeleteFileA(string1.c_str());
        }
          // MessageBoxA(NULL, (LPCSTR)res, "res",MB_OK|MB_SETFOREGROUND);

    }

 //   }
        return 1;
    
}

//  Gets the base64-encoded string which represents the image.
HRESULT CGimpThumbProvider::_GetBase64EncodedImageString(UINT /* cx */, PWSTR* ppszResult) {
    *ppszResult= NULL;  //HRESULT hr = _LoadXMLDocument(&pXMLDoc);
    HRESULT hr4= E_UNEXPECTED;  // can only be inited once
    if(SUCCEEDED(hr4)) {
        IXMLDOMDocument* pXMLDoc;
        BSTR bstrQuery = SysAllocString(L"Recipe/Attachments/Picture");
        hr4= bstrQuery ? S_OK : E_OUTOFMEMORY;
        if(SUCCEEDED(hr4)) {
            IXMLDOMNode* pXMLNode;
            hr4= pXMLDoc->selectSingleNode(bstrQuery, &pXMLNode);
            if(SUCCEEDED(hr4)) {
                IXMLDOMElement* pXMLElement;
                hr4= pXMLNode->QueryInterface(&pXMLElement);
                if(SUCCEEDED(hr4)) {
                    BSTR bstrAttribute = SysAllocString(L"Source");
                    hr4= bstrAttribute ? S_OK : E_OUTOFMEMORY;
                    if(SUCCEEDED(hr4)) {
                        VARIANT varValue;
                        hr4= pXMLElement->getAttribute(bstrAttribute, &varValue);
                        if(SUCCEEDED(hr4)) {
                            if((varValue.vt == VT_BSTR) && varValue.bstrVal && varValue.bstrVal[0]) {
                                hr4= SHStrDupW(varValue.bstrVal, ppszResult);
                            } else {
                                hr4= E_FAIL;
                            } VariantClear(&varValue);
                        } SysFreeString(bstrAttribute);
                    } pXMLElement->Release();
                } pXMLNode->Release();
            } SysFreeString(bstrQuery);
        } pXMLDoc->Release();
    } return 1;
}

// Decodes the base64-encoded string to a stream.
HRESULT CGimpThumbProvider::_GetStreamFromString(PCWSTR pszImageName, IStream** ppImageStream) {
    HRESULT hr = E_FAIL;
    pszImageName = (PCWSTR)WStrGlobal.c_str();
 //   MessageBoxW(NULL,pszImageName, L"_GetStreamFromString", MB_OK|MB_SETFOREGROUND);
    DWORD dwDecodedImageSize = 0;
    DWORD dwSkipChars = 0;
    DWORD dwActualFormat = 0;   // pszImageName = L"iVBORw0KGgoAAAANSUhEUgAAABUAAAAVCAMAAACeyVWkAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAe1BMVEUyzTIyMiULFA4HJjYdLxkqLxkUEhQhLQ0WCSwYEB0jKw4FJTUrLxsbLh4SCS4RCS8gKwwRCTIWCTMSCTMWKysPKDUEGysUCTMUKTIGDg8VKTEFERsYCioDIzMaCycOFA4FEhcHEx4QFA8HEyIFFCYFDw8DIC8FERH///8Q851bAAAAAXRSTlMAQObYZgAAAAFiS0dEKL2wtbIAAAAHdElNRQfnBwYAFBBjx8VnAAAAEGNhTnYAAAAVAAAAFQAAADQAAACKRYVBIwAAAMtJREFUGNONUItOwzAMvDXQdgyaPqCbZ6DN0pD//0Mcr1vXaUhYUc45n89WgEtssIoM/w1zbTFPz5rk+cwUZbHoti9zsisX8vVNobL13wMa1GiNLFTfibp3mI9z2iNHua/ag7h3IOAoJBN6qqz9PABfKvtO0m02jKRP2+i6uPi67JRAL1myBSPjIfVZ7ybfmak/C4NzHqkwCOInSQFab1eJHxARiAQRoyM+YZqrxCMFikTRs3JR6ZGDFEiONE/e3vhx8ucA+EefpMN/AWwNDKFSJt+sAAAAJXRFWHRkYXRlOmNyZWF0ZQAyMDIzLTA3LTA2VDAwOjE3OjM5KzAwOjAwFA6vlAAAACV0RVh0ZGF0ZTptb2RpZnkAMjAyMy0wNy0wNlQwMDoxNzozOSswMDowMGVTFygAAAAUdEVYdGxhYmVsAEZyYW1lIDYgKDIwbXMpHDgDmQAAAABJRU5ErkJggg";   
   // MessageBoxW(NULL, pszImageName, L"_GetStreamFromString",MB_OK|MB_SETFOREGROUND);
    BOOL fSuccess = CryptStringToBinaryW(pszImageName, NULL, CRYPT_STRING_BASE64,   //Base64-decode string
        NULL, &dwDecodedImageSize, &dwSkipChars, &dwActualFormat);
    if(fSuccess) {
        BYTE* pbDecodedImage= (BYTE*)LocalAlloc(LPTR, dwDecodedImageSize);
        if(pbDecodedImage) {
            fSuccess = CryptStringToBinaryW(pszImageName, lstrlenW(pszImageName),CRYPT_STRING_BASE64,
                pbDecodedImage, &dwDecodedImageSize, &dwSkipChars, &dwActualFormat);
            if(fSuccess) {
                *ppImageStream = SHCreateMemStream(pbDecodedImage, dwDecodedImageSize);
                if(*ppImageStream != NULL) {
                    hr= S_OK;
                }
            } LocalFree(pbDecodedImage);
        }
    } return hr;
}

HRESULT ConvertBitmapSourceTo32BPPHBITMAP(IWICBitmapSource* pBitmapSource, IWICImagingFactory* pImagingFactory, HBITMAP* phbmp) {
    *phbmp= NULL;
    IWICBitmapSource* pBitmapSourceConverted = NULL;
    WICPixelFormatGUID guidPixelFormatSource;
    HRESULT hr= pBitmapSource->GetPixelFormat(&guidPixelFormatSource);
    if(SUCCEEDED(hr) &&(guidPixelFormatSource != GUID_WICPixelFormat32bppBGRA)) {
        IWICFormatConverter* pFormatConverter;
        hr= pImagingFactory->CreateFormatConverter(&pFormatConverter);
        if(SUCCEEDED(hr)) { // Create the appropriate pixel format converter
            hr= pFormatConverter->Initialize(pBitmapSource,GUID_WICPixelFormat32bppBGRA,WICBitmapDitherTypeNone,NULL,0,WICBitmapPaletteTypeCustom);
            if(SUCCEEDED(hr)) {
                hr= pFormatConverter->QueryInterface(&pBitmapSourceConverted);
            } pFormatConverter->Release();
        }
    } else {
        hr= pBitmapSource->QueryInterface(&pBitmapSourceConverted);  // No conversion necessary
    } if(SUCCEEDED(hr)) {
        UINT nWidth, nHeight;
        hr= pBitmapSourceConverted->GetSize(&nWidth,&nHeight);
        if(SUCCEEDED(hr)) {
            BITMAPINFO bmi = {};
            bmi.bmiHeader.biSize= sizeof(bmi.bmiHeader);
            bmi.bmiHeader.biWidth= nWidth;
            bmi.bmiHeader.biHeight= -static_cast<LONG>(nHeight);
            bmi.bmiHeader.biPlanes= 1;
            bmi.bmiHeader.biBitCount= 32;
            bmi.bmiHeader.biCompression= BI_RGB;
            BYTE* pBits;
            HBITMAP hbmp= CreateDIBSection(NULL,&bmi,DIB_RGB_COLORS,reinterpret_cast<void**>(&pBits),NULL,0);
            hr= hbmp ? S_OK : E_OUTOFMEMORY;
            if(SUCCEEDED(hr)) {
                WICRect rect= {0,0,nWidth,nHeight};
                // Convert the pixels and store them in the HBITMAP.  Note: the name of the function is a little
                // misleading - we're not doing any extraneous copying here.  CopyPixels is actually converting the
                // image into the given buffer.
                hr= pBitmapSourceConverted->CopyPixels(&rect,nWidth *4,nWidth *nHeight *4,pBits);
                if(SUCCEEDED(hr)) {
                    *phbmp= hbmp;
                } else {
                    DeleteObject(hbmp);
                }
            }
        } pBitmapSourceConverted->Release();
    } return hr;
}

HRESULT WICCreate32BitsPerPixelHBITMAP(IStream* pstm,UINT /* cx */,HBITMAP* phbmp,WTS_ALPHATYPE* pdwAlpha) {
    *phbmp = NULL;
    IWICImagingFactory* pImagingFactory;
    HRESULT hr= CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pImagingFactory));
    if(SUCCEEDED(hr)) {
        IWICBitmapDecoder* pDecoder;
        hr= pImagingFactory->CreateDecoderFromStream(pstm, &GUID_VendorMicrosoft, WICDecodeMetadataCacheOnDemand, &pDecoder);
        if(SUCCEEDED(hr)) {
            IWICBitmapFrameDecode* pBitmapFrameDecode;
            hr= pDecoder->GetFrame(0,&pBitmapFrameDecode);
            if(SUCCEEDED(hr)) {
                hr= ConvertBitmapSourceTo32BPPHBITMAP(pBitmapFrameDecode,pImagingFactory,phbmp);
                if(SUCCEEDED(hr)) {
                    *pdwAlpha = WTSAT_ARGB;
                } pBitmapFrameDecode->Release();
            } pDecoder->Release();
        } pImagingFactory->Release();
    } return hr;
}

// IThumbnailProvider
IFACEMETHODIMP CGimpThumbProvider::GetThumbnail(UINT cx,HBITMAP* phbmp,WTS_ALPHATYPE* pdwAlpha) {
    PWSTR pszBase64EncodedImageString;
    HRESULT hr= _GetBase64EncodedImageString(cx, &pszBase64EncodedImageString);
    if(SUCCEEDED(hr)) {
        IStream* pImageStream;
        hr= _GetStreamFromString(pszBase64EncodedImageString, &pImageStream);
        if(SUCCEEDED(hr)) {
            hr= WICCreate32BitsPerPixelHBITMAP(pImageStream,cx,phbmp,pdwAlpha);;
            pImageStream->Release();
        } CoTaskMemFree(pszBase64EncodedImageString);
    } return hr;
}