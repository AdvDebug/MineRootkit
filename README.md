# MineRootkit
PoC Windows Usermode Rootkit made in C# and C++, made to show you how to protect your process from termination using hooking. (if you liked it please give me a start to continue on this and other projects)
# Usage
All you have to do is to enter the process id in the textbox which you want the program to be prevented from terminating MineRootkit, you can choose something like Task Manager or Process Explorer.

<img width="404" alt="MineRootkit" src="https://user-images.githubusercontent.com/90452585/141402279-bbdf9380-f6ae-45c8-b338-be5e3a4e9fd9.PNG">

# Tests (Tested only on windows 10 x64 machine)
Task Manager - Success

<img width="960" alt="TaskMgr" src="https://user-images.githubusercontent.com/90452585/141400756-3945746b-aa43-4cd3-9303-883ec93b3b88.png">

Process Explorer - Success

<img width="960" alt="ProcessExplorer" src="https://user-images.githubusercontent.com/90452585/141400804-e8c2144f-3d1d-43af-98bf-ffcc93397df5.PNG">

Process Hacker - Failed
# Video Showcase
Youtube Video Showcase: https://youtu.be/sVxOJApqGJM
# Note
Please Don't download the rootkit from any other source, i found a lot of people took the rootkit and injected it with a rat, ransomware, etc... on hacking forums. (always run programs from unknown sources in a VM or a sandbox with internet connection disabled)
