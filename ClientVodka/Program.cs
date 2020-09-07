/* ClientVodka Author C4ke#0002
*  Ты думаешь это магазин с водярой? 
*  Я скажу так, ди нахер
*/
using System;
using System.Diagnostics;

namespace ClientVodka
{
    class Program
    {

        const int offset_typeReq = 0x3058;//0x3058
        const int offset_memAddress = 0x3048;//0x3048
        const int offset_memSize = 0x3050;//0x3050
        const int offset_retAnswer = 0x3038;//0x3038
        const int offset_fRetAnswer = 0x3040;//0x3040
        const int offset_pid = 0x3060;//0x3060

        static VAMemory vam;
        static IntPtr bAddress;//base address VodkaHandler
        static void Main(string[] args)
        {
            vam = new VAMemory("VodkaHandler");
            bAddress = Process.GetProcessesByName("VodkaHandler")[0].MainModule.BaseAddress;
            Console.Write("-> Input PID: ");
            int pid = int.Parse(Console.ReadLine());
            Console.WriteLine(GetBaseAddress(pid));
            //Console.WriteLine(ReadMemoryFloat(pid, bb + 0x1B044)); this is example
            Console.ReadLine();
        }

        static UInt64 GetBaseAddress(int pid)
        {
            vam.WriteInt32(bAddress + offset_pid, pid);
            vam.WriteInt32(bAddress + offset_typeReq, 1);
            return vam.ReadUInt64(bAddress + offset_retAnswer);
        }

        static int ReadMemoryInt(int pid, UInt64 address)//example for read int
        {
            vam.WriteInt32(bAddress + offset_pid, pid);
            vam.WriteInt32(bAddress + offset_memSize, 4);
            vam.WriteUInt64(bAddress + offset_memAddress, address);
            vam.WriteInt32(bAddress + offset_typeReq, 2);
            return vam.ReadInt32(bAddress + offset_retAnswer);
        }

        static float ReadMemoryFloat(int pid, UInt64 address)//example for read float
        {
            vam.WriteInt32(bAddress + offset_pid, pid);
            vam.WriteUInt64(bAddress + offset_memAddress, address);
            vam.WriteInt32(bAddress + offset_typeReq, 3);
            return vam.ReadFloat(bAddress + offset_fRetAnswer);
        }
    }
}
