# Microsoft Developer Studio Project File - Name="Magical Broom Extreme" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Magical Broom Extreme - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Magical Broom Extreme.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Magical Broom Extreme.mak" CFG="Magical Broom Extreme - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Magical Broom Extreme - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Magical Broom Extreme - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Magical Broom Extreme - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W4 /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 luna.lib kernel32.lib uuid.lib libcmt.lib ole32.lib shell32.lib advapi32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib wsock32.lib imm32.lib winmm.lib dxguid.lib d3dx8.lib amstrmid.lib strmbase.lib d3dxof.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib /out:"bin/mbx.exe"

!ELSEIF  "$(CFG)" == "Magical Broom Extreme - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 luna.lib kernel32.lib uuid.lib libcmt.lib ole32.lib shell32.lib advapi32.lib comdlg32.lib gdi32.lib user32.lib comctl32.lib wsock32.lib imm32.lib winmm.lib dxguid.lib d3dx8.lib amstrmid.lib strmbase.lib d3dxof.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib /out:"bin/mbx_debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Magical Broom Extreme - Win32 Release"
# Name "Magical Broom Extreme - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\Application.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Io.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Voice.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Source\Application.h
# End Source File
# Begin Source File

SOURCE=.\Source\Base.h
# End Source File
# Begin Source File

SOURCE=.\Source\Event.h
# End Source File
# Begin Source File

SOURCE=.\Source\Io.h
# End Source File
# Begin Source File

SOURCE=.\Source\Luna.h
# End Source File
# Begin Source File

SOURCE=.\Source\Sound.h
# End Source File
# Begin Source File

SOURCE=.\Source\Voice.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ICON.ICO
# End Source File
# Begin Source File

SOURCE=.\Script.rc
# End Source File
# End Group
# Begin Group "Sprite"

# PROP Default_Filter ""
# Begin Group "Sprite_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\Bg.h
# End Source File
# Begin Source File

SOURCE=.\Source\Bit.h
# End Source File
# Begin Source File

SOURCE=.\Source\BitBlur.h
# End Source File
# Begin Source File

SOURCE=.\Source\Bomb.h
# End Source File
# Begin Source File

SOURCE=.\Source\Bonus.h
# End Source File
# Begin Source File

SOURCE=.\Source\BossLaser.h
# End Source File
# Begin Source File

SOURCE=.\Source\BossLife.h
# End Source File
# Begin Source File

SOURCE=.\Source\CharaBase.h
# End Source File
# Begin Source File

SOURCE=.\Source\Coin.h
# End Source File
# Begin Source File

SOURCE=.\Source\Combo.h
# End Source File
# Begin Source File

SOURCE=.\Source\Enemy.h
# End Source File
# Begin Source File

SOURCE=.\Source\EnemyFx.h
# End Source File
# Begin Source File

SOURCE=.\Source\EnemyShot.h
# End Source File
# Begin Source File

SOURCE=.\Source\Fairy.h
# End Source File
# Begin Source File

SOURCE=.\Source\Item.h
# End Source File
# Begin Source File

SOURCE=.\Source\ItemFx.h
# End Source File
# Begin Source File

SOURCE=.\Source\Laser.h
# End Source File
# Begin Source File

SOURCE=.\Source\LaserFx.h
# End Source File
# Begin Source File

SOURCE=.\Source\Life.h
# End Source File
# Begin Source File

SOURCE=.\Source\LifeParts.h
# End Source File
# Begin Source File

SOURCE=.\Source\Parts.h
# End Source File
# Begin Source File

SOURCE=.\Source\Player.h
# End Source File
# Begin Source File

SOURCE=.\Source\Scroll.h
# End Source File
# Begin Source File

SOURCE=.\Source\Shot.h
# End Source File
# Begin Source File

SOURCE=.\Source\Smoke.h
# End Source File
# Begin Source File

SOURCE=.\Source\Snow.h
# End Source File
# Begin Source File

SOURCE=.\Source\Sonic.h
# End Source File
# Begin Source File

SOURCE=.\Source\Tail.h
# End Source File
# Begin Source File

SOURCE=.\Source\Title.h
# End Source File
# Begin Source File

SOURCE=.\Source\Warning.h
# End Source File
# Begin Source File

SOURCE=.\Source\Window.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Source\BG.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Bit.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\BitBlur.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Bomb.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Bonus.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\BossLaser.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\BossLife.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\CharaBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Coin.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Combo.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\EnemyFx.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\EnemyShot.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Fairy.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\ItemFx.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Laser.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\LaserFx.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Life.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\LifeParts.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Parts.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Scroll.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Shot.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Smoke.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Snow.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Sonic.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Tail.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Title.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Warning.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Window.cpp
# End Source File
# End Group
# End Target
# End Project
