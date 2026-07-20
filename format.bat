@echo off
REM Format all C++ files in src directory

for /r src %%f in (*.cpp *.h *.hpp) do (
    clang-format -i "%%f"
    echo Formatted: %%f
)

echo Format complete.
pause
