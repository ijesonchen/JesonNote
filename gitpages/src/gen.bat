copy _config.yml source\src\config.yml /y
copy autoGen.bat source\src\. /y
copy source\src\*.md source\_posts /y
hexo g
hexo s