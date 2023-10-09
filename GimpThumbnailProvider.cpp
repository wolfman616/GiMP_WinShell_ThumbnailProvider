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
#include <chrono>
#include <thread>
#include <locale>
#include <string>
#include <new>
#include <codecvt>        // std::codecvt_utf8_utf16
#include <sys/stat.h>
#include <string>
#include <wincrypt.h>

#include <fstream>
#pragma comment(lib, "Crypt32.lib")
using namespace std;

std::wstring WStrGlobal;

#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"Crypt32.lib")

//    Using Process isolation override.
//    Implementing IInitializeWithFile to work toward being hosted in an un-isolated Explorer process *, promoting succesful Spaghetti-like nature, Thumbnail & Preview.
class CGimpThumbProvider : public IInitializeWithFile, public IThumbnailProvider
{
public:
    CGimpThumbProvider() : _cRef(1), _pStream(NULL) {

    }

    //  Release sequence.
    virtual ~CGimpThumbProvider() {
        (_pStream)? _pStream->Release(): static_cast<void>(0);
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
    IFACEMETHODIMP Initialize(LPCWSTR pszFilePath, DWORD grfMode);
    //  IThumbnailProvider
    IFACEMETHODIMP GetThumbnail(UINT cx, HBITMAP* phbmp, WTS_ALPHATYPE* pdwAlpha);

private: //HRESULT _LoadXMLDocument( IXMLDOMDocument **ppXMLDoc);
    HRESULT _GetBase64EncodedImageString(UINT cx, PWSTR* ppszResult);
    HRESULT _GetStreamFromString(PCWSTR pszImageName, IStream** ppStream);
    long _cRef;
    IStream* _pStream; // provided during initialization.
};

// In the event Gimp is saving a document which is in an open explorer window, Give a chance for the Gimp to save an active document as the Thumb Factory is invoked the instant the targets modified date is differing from the previous record held in the Windows thumbnail cache.
HRESULT CGimpThumbProvider_CreateInstance(REFIID riid, void** ppv) {

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    CGimpThumbProvider* pNew = new (std::nothrow) CGimpThumbProvider();
    HRESULT hr = pNew ? S_OK : E_OUTOFMEMORY;
    if (SUCCEEDED(hr)) {
        hr = pNew->QueryInterface(riid, ppv);
        pNew->Release();
    } return hr;
}

// IInitializeWithFile
IFACEMETHODIMP CGimpThumbProvider::Initialize(LPCWSTR pszFilePath, DWORD grfMode) {
    HRESULT hr = E_UNEXPECTED;  //can only be inited once
    if (pszFilePath != NULL) {

 


        unsigned int4 = 2;
        std::stringstream ass;
        std::wstring string4 = pszFilePath;
        std::wstring_convert<std::codecvt_utf8<wchar_t>>conv1;
        std::string string1 = conv1.to_bytes(string4);
   
        LPCSTR command1 = "open";
        LPCSTR LPFILE = "\"C:\\Script\\AHK\\z_ConTxt\\GIMP_XCF_Shell_Thumb_Helper.ahk\"";
        

       std::string input_string = string1;
        DWORD binary_length = input_string.length();
        BYTE* binary_data = (BYTE*)input_string.c_str();
        DWORD base64_length = 0;
        CryptBinaryToStringA(binary_data,
            binary_length,
            CRYPT_STRING_BASE64,
            NULL,
            &base64_length);

        char* encoded_data = new char[base64_length];

        CryptBinaryToStringA(binary_data,
            binary_length,
            CRYPT_STRING_BASE64,
            encoded_data,
            &base64_length);

        //std::cout << encoded_data << std::endl;

            std::string LPb64 = encoded_data;

            std::string LPb64textfile = "C:\\Users\\ninj\\AppData\\Local\\Temp\\" + LPb64 + ".txt";
            LPb64textfile.erase(std::remove(LPb64textfile.begin(), LPb64textfile.end(), '\n'), LPb64textfile.end());
            LPb64textfile.erase(std::remove(LPb64textfile.begin(), LPb64textfile.end(), '\r'), LPb64textfile.end());

            std::string arg2 = encoded_data;


        string string2 = "\"" + string1 + "\" \"" + arg2 + "\"";
         //MessageBoxA(NULL, string2.c_str(), string2.c_str(), MB_OK | MB_SETFOREGROUND);
        LPCSTR lpParameters = string2.c_str();



            delete[] encoded_data;
    CA2W ca3w(LPb64textfile.c_str());
        BOOL res = 0;
        ShellExecuteA(NULL,command1, LPFILE, lpParameters, NULL, 0);
        int4 = strlen(LPb64textfile.c_str());
        std::string string3 = ".txt";
        IStream* _pStream;
        bool existtxt = 0;
        struct stat buffer;
        int readattempts = 0;

        while (readattempts < 10) {
            readattempts++;
            existtxt = (stat(LPb64textfile.c_str(), &buffer) == 0);

            if (existtxt) {

                size_t initialSize = buffer.st_size;

                std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for 2 seconds.
               // MessageBoxA(NULL, "T.", "Theds.", MB_OK | MB_SETFOREGROUND);

                if (stat(LPb64textfile.c_str(), &buffer) == 0) {
                    size_t currentSize = buffer.st_size;

                    if (currentSize == initialSize) {
                        // The file size has not changed for 2 seconds.
                        // You can proceed with further operations here.
                       // MessageBoxA(NULL, "The file size has not changed for 2 seconds.", "The file size has not changed for 2 seconds.",MB_OK|MB_SETFOREGROUND);
                        break;
                    }
                }
            }

            // Wait for 1 second before the next attempt.
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        if (!existtxt || readattempts >= 10) {
            // The file didn't appear or stopped growing after 10 attempts.
            // Handle this case accordingly.
            return 1;
        }
        std::ifstream ifs(LPb64textfile);
        std::string content;
        content.assign((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));
        ifs.close();
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        CA2W ca2w(content.c_str());
        WStrGlobal = ca2w;
        res = 0;
        int deleteAttempts = 0;


        int bufferSize = MultiByteToWideChar(CP_UTF8, 0, LPb64textfile.c_str(), -1, nullptr, 0);

        // Allocate memory for the wide-character string
        wchar_t* wideString = nullptr; // Initialize to nullptr
    wideString = new wchar_t[bufferSize];

        // Convert the std::string to LPCWSTR
        MultiByteToWideChar(CP_UTF8, 0, LPb64textfile.c_str(), -1, wideString, bufferSize);

        // Now, wideString is an LPCWSTR containing the converted string

        // Use wideString as needed (e.g., pass it to Windows API functions)
        // ...

        // Clean up


        //res = DeleteFileW(wideString); crashes explorer below doesnt i dont know why i tried pre post delays

        if (DeleteFileW(wideString)) {
            // MessageBoxA(NULL, "File deleted successfully.", "Theds.", MB_OK | MB_SETFOREGROUND);

        }
        else {
            DWORD error = GetLastError();
            // Format the error message as a wide-character string
            wchar_t errorMessage[256];
            swprintf_s(errorMessage, L"File deletion failed with error code: %lu", error);

            // Display the error message in a message box
         //   MessageBoxW(NULL, errorMessage, L"Error", MB_ICONERROR | MB_SETFOREGROUND);
        }
             delete[] wideString;
   //MessageBoxA(NULL,(LPCSTR)res, "Theds.", MB_OK | MB_SETFOREGROUND);

      //    while (res == 0 && deleteAttempts < 5) {
      //        std::this_thread::sleep_for(std::chrono::milliseconds(800));
      //        res = DeleteFileW(ca3w);
      //   } //i(res==0) //return 1;
    } return 1;
}

// Gets the base64-encoded string which represents the image.
HRESULT CGimpThumbProvider::_GetBase64EncodedImageString(UINT /* cx */, PWSTR* ppszResult) {
    *ppszResult = NULL;  //HRESULT hr = _LoadXMLDocument(&pXMLDoc);
    HRESULT hr4 = E_UNEXPECTED;  // can only be inited once
    if (SUCCEEDED(hr4)) {
        IXMLDOMDocument* pXMLDoc;
        BSTR bstrQuery = SysAllocString(L"Gimp/Attachments/Picture");
        hr4 = bstrQuery ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr4)) {
            IXMLDOMNode* pXMLNode;
            hr4 = pXMLDoc->selectSingleNode(bstrQuery, &pXMLNode);
            if (SUCCEEDED(hr4)) {
                IXMLDOMElement* pXMLElement;
                hr4 = pXMLNode->QueryInterface(&pXMLElement);
                if (SUCCEEDED(hr4)) {
                    BSTR bstrAttribute = SysAllocString(L"Source");
                    hr4 = bstrAttribute ? S_OK : E_OUTOFMEMORY;
                    if (SUCCEEDED(hr4)) {
                        VARIANT varValue;
                        hr4 = pXMLElement->getAttribute(bstrAttribute, &varValue);
                        if (SUCCEEDED(hr4)) {
                            if ((varValue.vt == VT_BSTR) && varValue.bstrVal && varValue.bstrVal[0]) {
                                hr4 = SHStrDupW(varValue.bstrVal, ppszResult);
                            }
                            else {
                                hr4 = E_FAIL;
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
    DWORD dwDecodedImageSize = 0;
    DWORD dwSkipChars = 0;
    DWORD dwActualFormat = 0;   
    BOOL fSuccess = CryptStringToBinaryW(pszImageName, NULL, CRYPT_STRING_BASE64,   //Base64-decode string
        NULL, &dwDecodedImageSize, &dwSkipChars, &dwActualFormat);
    if (fSuccess) {
        BYTE* pbDecodedImage = (BYTE*)LocalAlloc(LPTR, dwDecodedImageSize);
        if (pbDecodedImage) {
            fSuccess = CryptStringToBinaryW(pszImageName, lstrlenW(pszImageName), CRYPT_STRING_BASE64,
                pbDecodedImage, &dwDecodedImageSize, &dwSkipChars, &dwActualFormat);
            if (fSuccess) {
                *ppImageStream = SHCreateMemStream(pbDecodedImage, dwDecodedImageSize);
                if (*ppImageStream != NULL) {
                    hr = S_OK;
                }
            } LocalFree(pbDecodedImage);
        }
    } return hr;
}

HRESULT ConvertBitmapSourceTo32BPPHBITMAP(IWICBitmapSource* pBitmapSource, IWICImagingFactory* pImagingFactory, HBITMAP* phbmp) {
    *phbmp = NULL;
    IWICBitmapSource* pBitmapSourceConverted = NULL;
    WICPixelFormatGUID guidPixelFormatSource;
    HRESULT hr = pBitmapSource->GetPixelFormat(&guidPixelFormatSource);
    if (SUCCEEDED(hr) && (guidPixelFormatSource != GUID_WICPixelFormat32bppBGRA)) {
        IWICFormatConverter* pFormatConverter;
        hr = pImagingFactory->CreateFormatConverter(&pFormatConverter);
        if (SUCCEEDED(hr)) { // Create the appropriate pixel format converter
            hr = pFormatConverter->Initialize(pBitmapSource, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom);
            if (SUCCEEDED(hr)) {
                hr = pFormatConverter->QueryInterface(&pBitmapSourceConverted);
            } pFormatConverter->Release();
        }
    }
    else {
        hr = pBitmapSource->QueryInterface(&pBitmapSourceConverted);  // No conversion necessary
    } if (SUCCEEDED(hr)) {
        UINT nWidth, nHeight;
        hr = pBitmapSourceConverted->GetSize(&nWidth, &nHeight);
        if (SUCCEEDED(hr)) {
            BITMAPINFO bmi = {};
            bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
            bmi.bmiHeader.biWidth = nWidth;
            bmi.bmiHeader.biHeight = -static_cast<LONG>(nHeight);
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;
            BYTE* pBits;
            HBITMAP hbmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, reinterpret_cast<void**>(&pBits), NULL, 0);
            hr = hbmp ? S_OK : E_OUTOFMEMORY;
            if (SUCCEEDED(hr)) {
                WICRect rect = { 0,0,nWidth,nHeight };
                // Convert the pixels and store them in the HBITMAP.  Note: the name of the function is a little
                // misleading - we're not doing any extraneous copying here.  CopyPixels is actually converting the
                // image into the given buffer.
                hr = pBitmapSourceConverted->CopyPixels(&rect, nWidth * 4, nWidth * nHeight * 4, pBits);
                if (SUCCEEDED(hr)) {
                    *phbmp = hbmp;
                } else {
                    DeleteObject(hbmp);
                }
            }
        } pBitmapSourceConverted->Release();
    } return hr;
}

HRESULT WICCreate32BitsPerPixelHBITMAP(IStream* pstm, UINT /* cx */, HBITMAP* phbmp, WTS_ALPHATYPE* pdwAlpha) {
    *phbmp = NULL;
    IWICImagingFactory* pImagingFactory;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pImagingFactory));
    if (SUCCEEDED(hr)) {
        IWICBitmapDecoder* pDecoder;
        hr = pImagingFactory->CreateDecoderFromStream(pstm, &GUID_VendorMicrosoft, WICDecodeMetadataCacheOnDemand, &pDecoder);
        if (SUCCEEDED(hr)) {
            IWICBitmapFrameDecode* pBitmapFrameDecode;
            hr = pDecoder->GetFrame(0, &pBitmapFrameDecode);
            if (SUCCEEDED(hr)) {
                hr = ConvertBitmapSourceTo32BPPHBITMAP(pBitmapFrameDecode, pImagingFactory, phbmp);
                if (SUCCEEDED(hr)) {
                    *pdwAlpha = WTSAT_ARGB;
                } pBitmapFrameDecode->Release();
            } pDecoder->Release();
        } pImagingFactory->Release();
    } return hr;
}

// IThumbnailProvider
IFACEMETHODIMP CGimpThumbProvider::GetThumbnail(UINT cx, HBITMAP* phbmp, WTS_ALPHATYPE* pdwAlpha) {
    PWSTR pszBase64EncodedImageString;
    HRESULT hr = _GetBase64EncodedImageString(cx, &pszBase64EncodedImageString);
    if (SUCCEEDED(hr)) {
        IStream* pImageStream;
        hr = _GetStreamFromString(pszBase64EncodedImageString, &pImageStream);
        if (SUCCEEDED(hr)) {
            hr = WICCreate32BitsPerPixelHBITMAP(pImageStream, cx, phbmp, pdwAlpha);;
            pImageStream->Release();
        } CoTaskMemFree(pszBase64EncodedImageString);
    } return hr;
}