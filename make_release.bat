@rem .\make_release.bat Win32 Debug 1.0.0

set base=Piron-GameToolkit

set base_folder=.releases

set platform=%1

set config=%2

set version=%3

set release_folder="%base_folder%\%base%-%1-%3"

echo %release_folder%

if not exist %release_folder% mkdir %release_folder%

@rem SprEd
call :DoCopy SprEd
call :DoCopy XSprite
call :DoCopy Sentry
call :DoCopy JadUpdate
call :DoCopy StringTool
call :DoCopy DataGrowth
call :DoCopy CurveEd

EXIT /B %ERRORLEVEL%

:DoCopy
copy /Y /B ".\bin\%~1\%platform%\%config%\%~1.exe" %release_folder%
EXIT /B 0