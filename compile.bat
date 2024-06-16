@echo off
REM CPP-Backend compiler
REM Compiling
g++ -o main.exe main.cpp socket.cpp -Iinclude -lws2_32

REM Check result
IF NOT EXIST main.exe (
    echo Compile Error
) ELSE (
    echo Compile Success
)

REM End of Script
pause
