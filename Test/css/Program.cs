using System;

namespace CS
{
    public static class Program
    {
        static void Main(string[] args)
        {
            rscv(999999);
        }

        static void rscv(int count)
        {
            Console.WriteLine(count);
            if (count == 0) return;
            
            rscv(count - 1);
        }
    }
}
