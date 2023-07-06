#NoEnv ; AHK1.1 M.Wolff 2023 GiMP Win-Shell Thumbnail Provider
#NoTrayIcon
#keyhistory,0
#include <imageput>
setbatchlines,-1
DetectHiddenText,On
DetectHiddenWindows,On
SetTitleMatchMode,2
SetTitleMatchMode,Slow
sleep,1000 ;if(!a_args[1])	;args1:= "C:\Script\AHK\__TESTS\New folder\0107.xcf" ;else 
args1:= a_args[1]
;tooltip,% args1
p:= splitpath(args1)
,PNGThumb:= p.dir . "\" . p.fn . ".png"
,JFIFThumb:= p.dir . "\" . p.fn . ".JFIF"
,thumb64txtfile:= p.dir . "\" . p.fn . ".txt"
;,cmdStr1:= comspec " /c mogrify -format png -append " chr(34) args1 chr(34) ;produced vertical strip of layers
,cmdStr1:= comspec " /c mogrify -format png -combine -compress JPEG -reverse -resize 256x256 -quality 0 -scale 99 " chr(34) args1 chr(34) ;produced final composite of layers
,cmdStr2:= comspec " /c convert " chr(34) PNGThumb chr(34) " " chr(34)  JFIFThumb chr(34) 
if(fileexist(thumb64txtfile))
	filedelete,% thumb64txtfile
if(fileexist(PNGThumb))
	filedelete,% PNGThumb
if(fileexist(JFIFThumb))
	filedelete,% JFIFThumb
if(!fileexist(PNGThumb)) {
	run,% cmdStr1,% p.dir,hide
loop,10 {
	sleep,500
	if(fileexist(PNGThumb))
		break,
}	run,% cmdStr2,% p.dir,hide
loop,10 {
		sleep,500
		if(fileexist(JFIFThumb))
		break,
}	FileGetSize,nBytes,%JFIFThumb%
FileRead,Bin,*c %JFIFThumb%
thumb64:= Base64Enc(Bin,nBytes)
loop,10 {
	if(!fileexist(thumb64txtfile)) {
		FileApPend,% thumb64,% thumb64txtfile
		break,
	} else,sleep,500
	sleep,100
}	loop,10 {
	sleep,500
	if(fileexist(thumb64txtfile))
			break,
}	if(fileexist(PNGThumb))
	filedelete,% PNGThumb
if(fileexist(JFIFThumb))
	filedelete,% JFIFThumb
} exitapp,

Base64Enc(ByRef Bin,nBytes,LineLength:=15000,LeadingSpaces:=0) { ; By SKAN / 18-Aug-2017
	Local Rqd := 0, B64, B := "", N := 0 - LineLength + 1, CRYPT_STRING_BASE64:= 0x1,NULL:=0
	DllCall("Crypt32.dll\CryptBinaryToString","Ptr",&Bin,"UInt",nBytes,"UInt",CRYPT_STRING_BASE64,"Ptr",NULL,"UIntP",Rqd)
	VarSetCapacity(B64,Rqd *(A_Isunicode? 2 : 1 ),0)
	DllCall("Crypt32.dll\CryptBinaryToString","Ptr",&Bin,"UInt",nBytes,"UInt",CRYPT_STRING_BASE64,"Str",B64,"UIntP",Rqd)
	;If ( LineLength = 64 and ! LeadingSpaces ) ;Return B64
	B64:= StrReplace(B64:= StrReplace(B64,"="),"`r`n")
	Loop,% Ceil(StrLen(B64) /LineLength)
		B.= Format("{1:" LeadingSpaces "s}","" ) . SubStr(B64,N+=LineLength,LineLength)
	Return,RTrim(B,"`n")
}