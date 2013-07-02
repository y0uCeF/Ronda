#--------------------------------
# Include Modern UI

    !include "MUI2.nsh"

#--------------------------------
# General

    # Program version
    !define RONDA_VERSION "0.1"
    !define RONDA_PRODVERSION "0.1.0.0"

    # Installer name and filename
    Name "Ronda"
    Caption "Ronda ${RONDA_VERSION} Setup"
    OutFile "Ronda-${RONDA_VERSION}.exe"

    # Icon to use for the installer
    !define MUI_ICON "../ronda.ico"

    # Default installation folder
    InstallDir "$PROGRAMFILES\Ronda"

    # Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\Ronda" ""

    # Request application privileges
    RequestExecutionLevel admin

#--------------------------------
# Version information

    VIProductVersion "${RONDA_PRODVERSION}"
    VIAddVersionKey "ProductName" "Ronda"
    VIAddVersionKey "FileDescription" "Ronda - an open source card game."
    VIAddVersionKey "FileVersion" "${RONDA_VERSION}"
    VIAddVersionKey "LegalCopyright" "GPL v.2"
    VIAddVersionKey "ProductVersion" "${RONDA_VERSION}"

#--------------------------------
# Settings

    # Show a warn on aborting installation
    !define MUI_ABORTWARNING

    # Defines the target start menu folder
    !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU"
    !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Ronda"
    !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

#--------------------------------
# Variables

    Var StartMenuFolder

#--------------------------------
# Pages

    # Installer pages
    !insertmacro MUI_PAGE_LICENSE "../../gpl-2.0.txt"
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
    !insertmacro MUI_PAGE_INSTFILES

    # Uninstaller pages
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES

#--------------------------------
# Languages

    !insertmacro MUI_LANGUAGE "English"

#--------------------------------
# Default installer section

Section
    SetShellVarContext all

    # Installation path
    SetOutPath "$INSTDIR"

    # Files to include in installer
    File ..\..\ronda.exe
    File ..\dlls\*.*
    File ..\ronda.ico
    File /r ..\..\data

    # Store installation folder in registry
    WriteRegStr HKCU "Software\Ronda" "" $INSTDIR

    # Create shortcuts
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
        CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
        CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Ronda.lnk" "$INSTDIR\ronda.exe"
        CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall Ronda.lnk" "$INSTDIR\Uninstall.exe"
        CreateShortCut "$DESKTOP\Ronda.lnk" "$INSTDIR\ronda.exe" "" "$INSTDIR\ronda.ico" 0
    !insertmacro MUI_STARTMENU_WRITE_END

    # Create the uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ronda" \
        "DisplayName" "Ronda"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ronda" \
        "DisplayIcon" "$INSTDIR\ronda.ico,0"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ronda" \
        "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ronda" \
        "DisplayVersion" ${RONDA_VERSION}

SectionEnd

#--------------------------------
# Uninstaller section

Section "Uninstall"
    SetShellVarContext all

    # Delete installed files
    Delete "$INSTDIR\*.dll"
    Delete "$INSTDIR\ronda.exe"
    Delete "$INSTDIR\ronda.ico"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir /r "$INSTDIR\data"
    RMDir "$INSTDIR"

    # Remove shortcuts
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    Delete "$SMPROGRAMS\$StartMenuFolder\Ronda.lnk"
    Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall Ronda.lnk"
    RMDir "$SMPROGRAMS\$StartMenuFolder"
    Delete "$DESKTOP\Ronda.lnk"

    # Remove registry entries
    DeleteRegKey /ifempty HKCU "Software\Ronda"
    DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\Ronda"

SectionEnd
