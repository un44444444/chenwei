''===========================================================
''
'' NAME: <CreateMyShortCut.vbs>
''
'' AUTHOR: Ivan Chen , Zsu
'' AUTHOR: CHENWEI
'' DATE  : 2013-7-23
''
'' COMMENT: <comment>
''
''=============================================================
Const strDestination = "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\MyTools\"

Set objArgs = WScript.Arguments
Set WshShell = WScript.CreateObject("WScript.Shell")

For i = 0 to objArgs.Count - 1   '' Loop through all arguments.
  strShortCut = strDestination & "\" & GetAName(objArgs(i)) & ".lnk"
  set oShellLink = WshShell.CreateShortcut(strShortCut)
  oShellLink.TargetPath = objArgs(i)
  oShellLink.WindowStyle = 1
  ''oShellLink.Description = "Created By Chenwei"
  oShellLink.WorkingDirectory = Left(objArgs(i), InStrRev(objArgs(i), "\"))
  oShellLink.Save
Next

''Function GetAName(DriveSpec)
''   Dim fso
''   Set fso = CreateObject("Scripting.FileSystemObject")
''   GetAName = fso.GetFileName(DriveSpec)
''   GetAName = Left(GetAName,Len(GetAname)-4)
''End Function
Function GetAName(DriveSpec)
   Dim fso
   Dim tmp
   Set fso = CreateObject("Scripting.FileSystemObject")
      GetAName = fso.GetFileName(DriveSpec)
      GetAName = Left(GetAName,Len(GetAname)-4)
   strMessage = "将创建快捷方式：" & strDestination & "\" & GetAName & ".lnk" & vbCrLf & "重命名(可选)："
   strTitle = "确认创建快捷方式"
   tmp = InputBox(strMessage, strTitle)
   If tmp<>"" then
      GetAName = tmp
   End If
End Function 
''end