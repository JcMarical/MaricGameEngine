using System;
using System.Runtime.CompilerServices;      //控制编译器的代码生成行为​

namespace CryDust
{


	//静态类​​，用于封装与底层（如C++引擎）交互的方法
	public static class InternalCalls
	{
		//判断是否含有组件
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

		//获取变换矩阵
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);

		//设置变换矩阵	
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);

		//添加线性冲量
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2DComponent_ApplyLinearImpulse(ulong entityID, ref Vector2 impulse, ref Vector2 point, bool wake);
		
		/// <summary>
		/// 这是？向中心的冲量？
		/// </summary>
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(ulong entityID, ref Vector2 impulse, bool wake);
		
		/// <summary>
		/// 输入按键
		/// </summary>
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyDown(KeyCode keycode);

	};


}
