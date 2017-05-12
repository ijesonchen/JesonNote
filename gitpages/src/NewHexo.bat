rem @echo off
echo input new hexo full path:
set /p hexopath= 
copy config.yml %hexopath%\_config.yml /y
mkdir %hexopath%\source\src
copy *.bat %hexopath%\.
xcopy . %hexopath%\source\src /s/e
pause