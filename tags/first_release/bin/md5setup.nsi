; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "EasyMD5"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "Shineware, Inc."
!define PRODUCT_WEB_SITE "http://www.shineware.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\wmd5.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "..\wmd5\res\wmd5.ico"
!define MUI_UNICON "..\wmd5\res\wmd5.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "license.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "emsetup.exe"
InstallDir "$PROGRAMFILES\EasyMD5"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "wmd5.exe"
  CreateDirectory "$SMPROGRAMS\EasyMD5"
  CreateDirectory "$INSTDIR\Help"
  CreateShortCut "$SMPROGRAMS\EasyMD5\Uninstall.lnk" "$INSTDIR\uninst.exe"
  CreateShortCut "$SMPROGRAMS\EasyMD5\README.lnk" "$INSTDIR\Help\README.htm"
  File "ctxmenu.dll"
  SetOverwrite try
  
  SetOutPath "$INSTDIR\Help"
  File "Help\README.htm"
  File "Help\snap-check.jpg"
  File "Help\snap-gen.jpg"
  File "Help\snap-run.jpg"
  File "Help\ctxmenu.jpg"

SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\wmd5.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\wmd5.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  
  WriteRegStr HKLM "SOFTWARE\EasyMD5" "" "$INSTDIR"
  Exec 'regsvr32.exe /s "$INSTDIR\ctxmenu.dll"'
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) 已成功地从你的计算机移除。"
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "你确实要完全移除 $(^Name) ，其及所有的组件？" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\Help\ctxmenu.jpg"
  Delete "$INSTDIR\Help\snap-run.jpg"
  Delete "$INSTDIR\Help\snap-gen.jpg"
  Delete "$INSTDIR\Help\snap-check.jpg"
  Delete "$INSTDIR\Help\README.htm"
  Delete "$INSTDIR\ctxmenu.dll"
  Delete "$INSTDIR\wmd5.exe"

  Delete "$SMPROGRAMS\EasyMD5\Uninstall.lnk"
  Delete "$SMPROGRAMS\EasyMD5\README.lnk"
  
  RMDir "$SMPROGRAMS\EasyMD5"
  RMDir "$INSTDIR\Help"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  
  DeleteRegKey HKLM "SOFTWARE\EasyMD5"
  Exec 'regsvr32.exe /s /u "$INSTDIR\ctxmenu.dll"'

  SetAutoClose true
SectionEnd