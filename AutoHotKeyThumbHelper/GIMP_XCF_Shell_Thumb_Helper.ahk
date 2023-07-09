#NoEnv ; AHK1.1 M.Wolff 2023 GiMP Win-Shell Thumbnail Provider
#NoTrayIcon
#keyhistory,0
#include <imageput>
setbatchlines,-1
DetectHiddenText,On
DetectHiddenWindows,On
SetTitleMatchMode,2
SetTitleMatchMode,Slow
sleep,1500 ; give chance for gimp to save an active document ;

args1:= (!a_args[1]? "C:\Script\AHK\__TESTS\New folder\0107.xcf" : a_args[1])

TempDir:= a_temp . "\"

p:= splitpath(args1)

, PNGThumb:= TempDir . p.fn . ".png"
, PNGThumb2:= TempDir . p.fn . "-nq8.png"
, JFIFThumb:= TempDir . p.fn . "-nq8.JFIF"
, thumb64txtfile:= p.dir . "\" . p.fn . ".txt"
, cmdStr1:= (comspec " /c convert +repage -background none -thumbnail 256x256 -layers merge " chr(34) args1 chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers
, cmdStr105:= "C:\Apps\pngnq-s9-2.0.2\pngnq-s9.exe -f -s1-A " chr(34) PNGThumb chr(34)
, cmdStr2:= comspec " /c convert " chr(34) PNGThumb2 chr(34) " " chr(34)  JFIFThumb chr(34) 

loop,parse,% "thumb64txtfile,PNGThumb,JFIFThumb",`,
	if(FileExist(%a_loopfield%))
		FileDelete,% %a_loopfield%

if(!FileExist(PNGThumb))
	run,% cmdStr1,% p.dir,hide

loop,10 {
	sleep,400
	if(FileExist(PNGThumb))
		break,
}

run,% cmdStr105,% p.dir,hide

loop,10 {
	sleep,500
	if(FileExist(PNGThumb2))
		break,
} if(FileExist(PNGThumb))
	FileDelete,% PNGThumb

run,% cmdStr2,% p.dir,hide

loop,10 {
	sleep,50
	if(FileExist(JFIFThumb))
		break,
}

loop,10 {
	sleep,20
	if(!FileExist(PNGThumb2))
		break,
	else,FileDelete,% PNGThumb2
}

FileGetSize,nBytes,%JFIFThumb%
FileRead,Bin,*c %JFIFThumb%
thumb64:= Base64Enc(Bin,nBytes)

loop,10 {
	if(!FileExist(thumb64txtfile)) {
		FileApPend,% thumb64,% thumb64txtfile
		break,
	} else,sleep,500
	sleep,100
}

loop,10 {
	sleep,500
	if(FileExist(thumb64txtfile))
		break,
}

loop,10 {
	sleep,20
	if(!FileExist(JFIFThumb))
		break,
		else,FileDelete,% JFIFThumb
}

exitapp,

; other imagemagick commands
;,cmdStr1:= comspec " /c mogrify -format png -append " chr(34) args1 chr(34) ;produced vertical strip of layers
;,cmdStr1:= (comspec " /c convert  -alpha Set -background none -depth 8 -thumbnail 256x256 -reverse -composite -quality 0 " chr(34) args1 chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers
;,cmdStr1:= (comspec " /c convert +repage -alpha copy -background none -thumbnail 256x256 -reverse -composite " chr(34) args1 chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers


Base64Enc(ByRef Bin,nBytes,LineLength:=1280000,LeadingSpaces:=0) { ; By SKAN / 18-Aug-2017
	Local Rqd := 0, B64, B := "", N := 0 - LineLength + 1, CRYPT_STRING_BASE64:= 0x1,NULL:=0
	DllCall("Crypt32.dll\CryptBinaryToString","Ptr",&Bin,"UInt",nBytes,"UInt",CRYPT_STRING_BASE64,"Ptr",NULL,"UIntP",Rqd)
	VarSetCapacity(B64,Rqd *(A_Isunicode? 2 : 1 ),0)
	DllCall("Crypt32.dll\CryptBinaryToString","Ptr",&Bin,"UInt",nBytes,"UInt",CRYPT_STRING_BASE64,"Str",B64,"UIntP",Rqd)
	B64:= StrReplace(B64:= StrReplace(B64,"="),"`r`n")
	Loop,% Ceil(StrLen(B64) /LineLength)
		B.= Format("{1:" LeadingSpaces "s}","" ) . SubStr(B64,N+=LineLength,LineLength)
	Return,RTrim(B,"`n")
}