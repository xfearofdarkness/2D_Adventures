;--------------------------------
; 2D Adventures NSIS Installer Script
;--------------------------------

!include "MUI2.nsh"
!include "x64.nsh"

;--------------------------------
; General Attributes
Name "2D Adventures"
OutFile "2D_Adventures_Installer.exe"
InstallDir "$PROGRAMFILES\2D Adventures"
InstallDirRegKey HKLM "Software\2D Adventures" "Install_Dir"
RequestExecutionLevel admin

;--------------------------------
; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

;--------------------------------
; Languages
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "German"

;--------------------------------
; Installer Sections

Section "Install Files" SEC01
    ; Set installation folder:
    SetOutPath "$INSTDIR"
    File "cmake-build-release\2D_Adventures.exe"
    
    ; Copy resources:
    SetOutPath "$INSTDIR\res"
    File /r "res\*.*"
    
    ; Return to installation folder:
    SetOutPath "$INSTDIR"
    
    ; Save the installation directory for future upgrades:
    WriteRegStr HKLM "Software\2D Adventures" "Install_Dir" "$INSTDIR"
    
    ; Write the uninstaller executable:
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    ; Create a desktop shortcut for the main app:
    CreateShortCut "$DESKTOP\2D Adventures.lnk" "$INSTDIR\2D_Adventures.exe" "" "$INSTDIR\res\Icon.ico" 0
    
    ; Create a Start Menu folder and shortcuts:
    CreateDirectory "$SMPROGRAMS\2D Adventures"
    CreateShortCut "$SMPROGRAMS\2D Adventures\2D Adventures.lnk" "$INSTDIR\2D_Adventures.exe" "" "$INSTDIR\res\Icon.ico" 0
    CreateShortCut "$SMPROGRAMS\2D Adventures\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\res\Icon.ico" 0
    
    ; Register the application for Add/Remove Programs (and Windows 11 Apps & features):
    ${If} ${RunningX64}
        SetRegView 64
    ${EndIf}
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\2D Adventures" "DisplayName" "2D Adventures"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\2D Adventures" "UninstallString" '"$INSTDIR\Uninstall.exe"'
    
    ; Additional keys for proper display in Windows settings:
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\2D Adventures" "DisplayIcon" "$INSTDIR\res\Icon.ico"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\2D Adventures" "DisplayVersion" "1.0.0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\2D Adventures" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\2D Adventures" "Publisher" "Jamie Huta"
SectionEnd

Section "Uninstall"
    ${If} ${RunningX64}
        SetRegView 64
    ${EndIf}
    ; Delete the main executable and uninstaller:
    Delete "$INSTDIR\2D_Adventures.exe"
    Delete "$INSTDIR\Uninstall.exe"
    
    ; Remove resources folder:
    RMDir /r "$INSTDIR\res"
    
    ; Remove desktop shortcut:
    Delete "$DESKTOP\2D Adventures.lnk"
    
    ; Remove Start Menu shortcuts:
    Delete "$SMPROGRAMS\2D Adventures\2D Adventures.lnk"
    Delete "$SMPROGRAMS\2D Adventures\Uninstall.lnk"
    RMDir "$SMPROGRAMS\2D Adventures"
    
    ; Clean up registry entries:
    DeleteRegKey HKLM "Software\2D Adventures"
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\2D Adventures"
    
    ; Finally, remove the installation folder:
    RMDir "$INSTDIR"
SectionEnd