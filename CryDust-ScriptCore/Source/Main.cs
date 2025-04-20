using System;
using System.Runtime.CompilerServices;      //控制编译器的代码生成行为​

namespace CryDust
{
	/// <summary>
	/// 向量类
	/// </summary>
	public struct Vector3
	{
		public float X, Y, Z;

		public Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	}

	//静态类​​，用于封装与底层（如C++引擎）交互的方法
	public static class InternalCalls
	{
		//使用extern 调用底层代码实现日志输出

		//MethodImplOptions.InternalCall​,标记该方法实现在CLR外部（如C++ DLL中），由运行时绑定到本地代码。
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void NativeLog(string text, int parameter);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void NativeLog_Vector(ref Vector3 parameter, out Vector3 result);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float NativeLog_VectorDot(ref Vector3 parameter);
	};


	public class Entity
	{
		public float FloatVar { get; set; }

		public Entity()
		{
			Console.WriteLine("Main constructor!主构造器！");
			Log("AAstroPhysiC", 8058);

			Vector3 pos = new Vector3(5, 2.5f, 1);
			Vector3 result = Log(pos);
			Console.WriteLine($"{result.X}, {result.Y}, {result.Z}");
			Console.WriteLine("{0}", InternalCalls.NativeLog_VectorDot(ref pos));
		}


		public void PrintMessage()
		{
			Console.WriteLine("Hello World from C#!");
		}

		public void PrintInt(int value)
		{
			Console.WriteLine($"C# says: {value}");
		}

		public void PrintInts(int value1, int value2)
		{
			Console.WriteLine($"C# says: {value1} and {value2}");
		}

		public void PrintCustomMessage(string message)
		{
			Console.WriteLine($"C# says: {message}");
		}

		private void Log(string text, int parameter)
		{
			InternalCalls.NativeLog(text, parameter);
		}

		private Vector3 Log(Vector3 parameter)
		{
			InternalCalls.NativeLog_Vector(ref parameter,out Vector3 result);
			return result;
		}
	}
}
