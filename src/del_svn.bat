rem 删除当前目录下，所有 cvs 文件，/r 为递归删除
for /r %%a in (.) do @if exist %%a\.svn @rmdir /Q /S "%%a\.svn"

