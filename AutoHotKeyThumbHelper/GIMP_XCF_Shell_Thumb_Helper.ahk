#NoEnv  /*  ; AHK1.1  GiMP Win-Shell Thumbnail Provider ;  M.Wolff 2023  ;  */
#NoTrayIcon
#keyhistory,0
#include <imageput>
setbatchlines,-1
DetectHiddenText,On
DetectHiddenWindows,On
SetTitleMatchMode,2
SetTitleMatchMode,Slow
Debug_Enabled:= False, Debug_Tooltip_Millisec:= 2000

TimeStart:= A_Tickcount

Args1:= ( !A_Args[1]? "C:\Script\AHK\__TESTS\New folder\0107.xcf" : A_Args[1] )

TempDir:= A_Temp . "\"

p:= Splitpath(Args1)

, PNGThumb:= TempDir . p.fn . ".png"
, PNGThumb2:= TempDir . p.fn . "-nq8.png" ; limits the pallete to 256 then quantizes the pallete.
, JFIFThumb:= TempDir . p.fn . "-nq8.JFIF" ; this was the only method to manifest a working alpha channel that i found .
, Thumb64TxtFile:= p.dir . "\" . p.fn . ".txt"
, cmdStr1:= (ComSpec " /C convert -background none -layers merge " chr(34) Args1 chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers
, cmdStr102:= (ComSpec " /C convert -thumbnail 256x256 " chr(34) PNGThumb chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers
, cmdStr105:= "C:\Apps\pngnq-s9-2.0.2\pngnq-s9.exe -f -s1-A " chr(34) PNGThumb chr(34)
, cmdStr2:= ComSpec " /C convert " chr(34) PNGThumb2 chr(34) " " chr(34)  JFIFThumb chr(34) 

loop,parse,% "Thumb64TxtFile,PNGThumb,JFIFThumb",`,
	if(FileExist(%a_Loopfield%))
		FileDelete,% %a_Loopfield%

if(!FileExist(PNGThumb))
	run,% cmdStr1,% p.dir,hide

loop,10 {
	sleep,400
	if(FileExist(PNGThumb))
		break,
}
run,% cmdStr102,% p.dir,hide
sleep,400
run,% cmdStr105,% p.dir,hide

loop,10 {
	sleep,500
	if(FileExist(PNGThumb2))
		break,
} if(FileExist(PNGThumb))
	FileDelete,% PNGThumb

run,% cmdStr2,% p.dir,hide

loop,10 {
	sleep,800
	if(FileExist(JFIFThumb))
		break,
}

loop,10 {
	sleep,800
	if(!FileExist(PNGThumb2))
		break,
	else,FileDelete,% PNGThumb2
}

FileGetSize,nBytes,%JFIFThumb%
FileRead,Bin,*c %JFIFThumb%
thumb64:= Base64Enc(Bin,nBytes)

loop,10 {
	if(!FileExist(Thumb64TxtFile)) {
		FileApPend,% thumb64,% Thumb64TxtFile
		break,
	} else,sleep,500
	sleep,100
}

loop,10 {
	sleep,500
	if(FileExist(Thumb64TxtFile))
		break,
}

loop,10 {
	sleep,80
	if(!FileExist(JFIFThumb))
		break,
		else,FileDelete,% JFIFThumb
}

if(Debug_Enabled) {
	tooltip,% "Thumbnail for: " Args1 "`nGenerated in: " TimeStart-A_Tickcount " Milliseconds"
	sleep,% Debug_Tooltip_Millisec
} exitapp,

Base64Enc(ByRef Bin,nBytes,LineLength:=5120000,LeadingSpaces:=0) { ; By SKAN / 18-Aug-2017
	Local Rqd := 0, B64, B := "", N := 0 - LineLength + 1, CRYPT_STRING_BASE64:= 0x1,NULL:=0
	DllCall("Crypt32.dll\CryptBinaryToString","Ptr",&Bin,"UInt",nBytes,"UInt",CRYPT_STRING_BASE64,"Ptr",NULL,"UIntP",Rqd)
	VarSetCapacity(B64,Rqd *(A_Isunicode? 2 : 1 ),0)
	DllCall("Crypt32.dll\CryptBinaryToString","Ptr",&Bin,"UInt",nBytes,"UInt",CRYPT_STRING_BASE64,"Str",B64,"UIntP",Rqd)
	B64:= StrReplace(B64:= StrReplace(B64,"="),"`r`n")
	Loop,% Ceil(StrLen(B64) /LineLength)
		B.= Format("{1:" LeadingSpaces "s}","" ) . SubStr(B64,N+=LineLength,LineLength)
	Return,RTrim(B,"`n")
}

SplitPath(Path="" ) {
	SplitPath,Path,,D,Ext,NameNoExt,Drive
	return,y:= ({"Dir" 	: D
	 ,			 		"Ext"		: Ext
	 ,					"Drv"		: Drive
	 ,					"FN"		: NameNoExt
	 ,					"Path"	: Path })
}

; other imagemagick commands
;,cmdStr1:= ComSpec " /c mogrify -format png -append " chr(34) Args1 chr(34) ;produced vertical strip of layers
;,cmdStr1:= (ComSpec " /c convert  -alpha Set -background none -depth 8 -thumbnail 256x256 -reverse -composite -quality 0 " chr(34) Args1 chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers
;,cmdStr1:= (ComSpec " /c convert +repage -alpha copy -background none -thumbnail 256x256 -reverse -composite " chr(34) Args1 chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers