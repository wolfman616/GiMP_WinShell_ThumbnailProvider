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

r_pid:= DllCall("GetCurrentProcessId")

if(ProcessClonesDetected()) {
	settimer,xit,-10000
	winget, hwnd_,id,% A_ScriptName . " - AutoHotkey ahk_pid " r_pid
	winsettitle,ahk_id %hwnd_%,,turds
	return,
}

Args1:= ( !A_Args[1]? "C:\Script\AHK\__TESTS\New folder\0107.xcf" : A_Args[1] )

TempDir:= A_Temp . "\"

p:= Splitpath(Args1)
if(p.ext!="xcf") {
	if(isdir(Args1))
		goto,FOLDERTHUMBFACTORY
} else,goto,XCFTHUMBFACTORY
return,

FOLDERTHUMBFACTORY:

return,

xit:
exitapp,
return,

HWNDlistGet(title) {
	local
	DetectHiddenCheck()

	ob:={}

	winget,li,list,% title
	if(li<2)
		return,

	loop,% ob.count:= li {
		winget,pid,pid,% "ahk_id " li%a_index%
		ob.push({	"hwnd"	:	li%a_index%
						,	"pid"		:	pid })
	}

	DetectHiddenReset()
	return,ob
}

DetectHiddenCheck() { ;modify detection settings if needed
	global
	if(A_DetectHiddenWindows="off") {
		oldDetectWin:= "off"
		DetectHiddenWindows,On
	}	if(A_DetectHiddenText="off") {
		oldDetectTxt:= "off"
		DetectHiddenText,	On
	} if(A_TitleMatchMode!=2){
		oldMatchMode:= A_TitleMatchMode
		SetTitleMatchMode,2
	} if(A_TitleMatchModeSpeed!="slow") {
		oldMatchSpeed:= A_TitleMatchModeSpeed
		SetTitleMatchMode,Slow
	}	return,byref oldMatchMode,byref oldMatchSpeed,byref oldDetectTxt,byref oldDetectWin
}

DetectHiddenReset() { ;reset any modified detection settings
	global
	if(oldMatchMode)
		SetTitleMatchMode,% oldMatchMode
	if(oldMatchSpeed)
		SetTitleMatchMode,% oldMatchSpeed
	if(oldDetectWin)
		DetectHiddenWindows,off
	if(oldDetectTxt)
		DetectHiddenText,off
}

ProcessClonesDetected(id="") { ; accepts hWnd or PID or leave blank to check own process
	local
	static pid_
	global oldMatchMode,oldMatchSpeed,oldDetectTxt,oldDetectWin

	if(id="")
		pid_:= DllCall("GetCurrentProcessId")
	else if !winexist("ahk_pid " id) {
		if winexist("ahk_id " id) 
			winget,pid_,pid,ahk_id %id%
	}

	object1:={}

	DetectHiddenCheck()
	loop,% count:=(object1:= HWNDlistGet(A_ScriptName . " - AutoHotkey ahk_class AutoHotkey")).count {
		winget,the_pid,pid,% "ahk_id " object1[a_index]
		(the_pid!=pid_? found_clone:= True)
	}

	DetectHiddenReset()

	return,% object1
}

XCFTHUMBFACTORY:
	PNGThumb := TempDir . p.fn . ".png"
, PNGThumb2:= TempDir . p.fn . "-nq8.png"  ; Pallete quantized and limited to 256.
, JFIFThumb:= TempDir . p.fn . "-nq8.JFIF" ; this was the only method to manifest a working alpha channel that i found .
;, Thumb64TxtFile:= p.dir . "\" . p.fn . ".txt"
, Thumb64TxtFile:= "C:\Users\ninj\AppData\Local\Temp\thumb64.txt"
, cmdStr1:= (ComSpec " /C convert -quality 0 -background none -layers merge " chr(34) Args1 chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers
, cmdStr102:= (ComSpec " /C convert -quality 0 -background none -thumbnail 256x256 " chr(34) PNGThumb chr(34) " " chr(34) PNGThumb chr(34)) ;produced final composite of layers
, cmdStr105:= "C:\Apps\pngnq-s9-2.0.2\pngnq-s9.exe -f -s1-A " chr(34) PNGThumb chr(34)
, cmdStr2:= ComSpec " /C convert -quality 0 -background none -thumbnail 256x256 " chr(34) PNGThumb2 chr(34) " " chr(34) JFIFThumb chr(34) 

loop,parse,% "Thumb64TxtFile,PNGThumb,JFIFThumb",`,
	if(FileExist(%a_Loopfield%))
		FileDelete,% %a_Loopfield%

if(FileExist(PNGThumb)) {
	msgbox png already exists
	exitapp,
}

runwait,% cmdStr1,% p.dir,hide

nbytesame:=0
loop,20 {
	FileGetSize,nBytes,%PNGThumb%

	if !nBytes {
		msgbox no nBytes PNGThumb
		exitapp
	}

	if(nBytes=nBytesold)
		nbytesame++
	if(nbytesame>3){
		break
	}	nBytesold:=nBytes
	sleep 150
}

if(!FileExist(PNGThumb)) {
	msgbox no PNGThumb
	exitapp
}

nbytesame:=0
loop,20 {
	FileGetSize,nBytes,%PNGThumb%

	if !nBytes {
		msgbox no nBytes PNGThumb
		exitapp
	}

	if(nBytes=nBytesold)
		nbytesame++
	if(nbytesame>3){
		break
	}	nBytesold:=nBytes
	sleep 150
}

runwait,% cmdStr102,% p.dir,hide

runwait,% cmdStr105,% p.dir,hide

loop,28 {
	sleep,150
	if(FileExist(PNGThumb2))
		break,
}

if(!FileExist(PNGThumb2)){
	msgbox,no PNGThumb2
	exitapp,
}

if(!FileExist(PNGThumb)){
	msgbox,no PNGThumb
		exitapp,
}

if(FileExist(PNGThumb))
FileDelete,% PNGThumb

if(!FileExist(PNGThumb2)){
	msgbox,no PNGThumb2
	exitapp,
}

sleep,100

runwait,% cmdStr2,% p.dir,hide

loop,20 {
	sleep,200
	if(FileExist(JFIFThumb))
		break,
}

if(!FileExist(JFIFThumb)) {
	msgbox,no jfif
	exitapp,
}

loop,20 {
	sleep,200
	if(!FileExist(PNGThumb2))
		break,
	else,FileDelete,% PNGThumb2
} sleep,100

FileGetSize,nBytes,%JFIFThumb%

if !nBytes {
	msgbox,no nBytes
	exitapp,
}

FileRead,Bin,*c %JFIFThumb%

if(!thumb64:= Base64Enc(Bin,nBytes)) {
	msgbox,no thumb64
	exitapp,
}

loop,10 {
	if(!FileExist(Thumb64TxtFile)) {
		FileApPend,% thumb64,% Thumb64TxtFile
		break,
	} else,sleep,100
	sleep,100
}

loop,10 {
	sleep,100
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