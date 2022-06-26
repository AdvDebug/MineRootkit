# MineRootkit
PoC Windows Usermode Rootkit made in C# and C++, made to show you how to protect your process using hooking. (if you liked it please give me a start to continue on this and other projects)
# Usage
All you have to do is to enter the process id in the textbox which you want the program to be prevented from terminating MineRootkit, you can choose something like Task Manager or Process Explorer.

<img width="404" alt="MineRootkit" src="https://user-images.githubusercontent.com/90452585/141402279-bbdf9380-f6ae-45c8-b338-be5e3a4e9fd9.PNG">

# Tests (Tested only on windows 10 x64 machine)
Task Manager - Success

<img width="960" alt="TaskMgr" src="https://user-images.githubusercontent.com/90452585/141400756-3945746b-aa43-4cd3-9303-883ec93b3b88.png">

Process Explorer - Success

<img width="960" alt="ProcessExplorer" src="https://user-images.githubusercontent.com/90452585/141400804-e8c2144f-3d1d-43af-98bf-ffcc93397df5.PNG">

Process Hacker - Failed (Depends on a kernel-driver)
# Video Showcase
Youtube Video Showcase: https://youtu.be/sVxOJApqGJM
# Detection (x64 version)
The Rootkit are currently FUD, Note That these results may change as time passes. (uploaded at the date of 12/2/2021)
_____________________________
AntiScan.Me Results:

Executable: <a href="https://antiscan.me/scan/new/result?id=D885RxJFaIDw">AntiScan.Me Executable Detection</a> (0/26 at the time of uploading it.)

Dll: <a href="https://antiscan.me/scan/new/result?id=YqJLg3MrKfTJ">AntiScan.Me Dll Detection</a> (0/26 at the time of uploading it.)
_____________________________
KleenScan Results:

Executable: <a href="https://kleenscan.com/scan_result/e63b714b447cc69098bf73c896aa2cba991d88dd79e194702534c0aa41797eac">KleenScan Executable Detection</a> (0/40 at the time of uploading it.)

Dll: <a href="https://kleenscan.com/scan_result/0ba9119bb0b84f3c9242e855e68a72fdbbb97b21d49355b92f570916789fa16e">KleenScan Dll Detection</a> (1/40 at the time of uploading it.)
_____________________________
VirusTotal Results:

Executable: <a href="https://www.virustotal.com/gui/file/2f6ae10301a36b4e1ba47cba376fff7c7f1262c35d8cde1978a225ff4a68c11c?nocache=1">VirusTotal Executable Detection</a> (1/63 at the time of uploading it.)

Dll: <a href="https://www.virustotal.com/gui/file/7c3f296b8b01e48739447cb6131a6026b26d4138e2736d34fda74027d57b1534?nocache=1">VirusTotal Dll Detection</a> (4/64 at the time of uploading it.)
