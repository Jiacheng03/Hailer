@echo off
if exist ./Hailer.exe (
    start Hailer.exe 33333 127.0.0.1 44444
    pause
    start Hailer.exe 44444 127.0.0.1 33333
) else (
    echo "./Hailer.exe does not exist"
    pause
)
