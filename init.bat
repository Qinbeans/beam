@REM generate the nmake build for compile_commands.json
powershell -Command "& { Import-Module 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll'; Enter-VsDevShell -VsInstallPath 'C:\Program Files\Microsoft Visual Studio\2022\Community' -SkipAutomaticLocation; cmake -G 'NMake Makefiles' -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build-nmake }"
