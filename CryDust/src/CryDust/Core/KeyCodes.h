#pragma once

namespace CryDust
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define CD_KEY_SPACE           ::CryDust::Key::Space
#define CD_KEY_APOSTROPHE      ::CryDust::Key::Apostrophe    /* ' */
#define CD_KEY_COMMA           ::CryDust::Key::Comma         /* , */
#define CD_KEY_MINUS           ::CryDust::Key::Minus         /* - */
#define CD_KEY_PERIOD          ::CryDust::Key::Period        /* . */
#define CD_KEY_SLASH           ::CryDust::Key::Slash         /* / */
#define CD_KEY_0               ::CryDust::Key::D0
#define CD_KEY_1               ::CryDust::Key::D1
#define CD_KEY_2               ::CryDust::Key::D2
#define CD_KEY_3               ::CryDust::Key::D3
#define CD_KEY_4               ::CryDust::Key::D4
#define CD_KEY_5               ::CryDust::Key::D5
#define CD_KEY_6               ::CryDust::Key::D6
#define CD_KEY_7               ::CryDust::Key::D7
#define CD_KEY_8               ::CryDust::Key::D8
#define CD_KEY_9               ::CryDust::Key::D9
#define CD_KEY_SEMICOLON       ::CryDust::Key::Semicolon     /* ; */
#define CD_KEY_EQUAL           ::CryDust::Key::Equal         /* = */
#define CD_KEY_A               ::CryDust::Key::A
#define CD_KEY_B               ::CryDust::Key::B
#define CD_KEY_C               ::CryDust::Key::C
#define CD_KEY_D               ::CryDust::Key::D
#define CD_KEY_E               ::CryDust::Key::E
#define CD_KEY_F               ::CryDust::Key::F
#define CD_KEY_G               ::CryDust::Key::G
#define CD_KEY_H               ::CryDust::Key::H
#define CD_KEY_I               ::CryDust::Key::I
#define CD_KEY_J               ::CryDust::Key::J
#define CD_KEY_K               ::CryDust::Key::K
#define CD_KEY_L               ::CryDust::Key::L
#define CD_KEY_M               ::CryDust::Key::M
#define CD_KEY_N               ::CryDust::Key::N
#define CD_KEY_O               ::CryDust::Key::O
#define CD_KEY_P               ::CryDust::Key::P
#define CD_KEY_Q               ::CryDust::Key::Q
#define CD_KEY_R               ::CryDust::Key::R
#define CD_KEY_S               ::CryDust::Key::S
#define CD_KEY_T               ::CryDust::Key::T
#define CD_KEY_U               ::CryDust::Key::U
#define CD_KEY_V               ::CryDust::Key::V
#define CD_KEY_W               ::CryDust::Key::W
#define CD_KEY_X               ::CryDust::Key::X
#define CD_KEY_Y               ::CryDust::Key::Y
#define CD_KEY_Z               ::CryDust::Key::Z
#define CD_KEY_LEFT_BRACKET    ::CryDust::Key::LeftBracket   /* [ */
#define CD_KEY_BACKSLASH       ::CryDust::Key::Backslash     /* \ */
#define CD_KEY_RIGHT_BRACKET   ::CryDust::Key::RightBracket  /* ] */
#define CD_KEY_GRAVE_ACCENT    ::CryDust::Key::GraveAccent   /* ` */
#define CD_KEY_WORLD_1         ::CryDust::Key::World1        /* non-US #1 */
#define CD_KEY_WORLD_2         ::CryDust::Key::World2        /* non-US #2 */

/* Function keys */
#define CD_KEY_ESCAPE          ::CryDust::Key::Escape
#define CD_KEY_ENTER           ::CryDust::Key::Enter
#define CD_KEY_TAB             ::CryDust::Key::Tab
#define CD_KEY_BACKSPACE       ::CryDust::Key::Backspace
#define CD_KEY_INSERT          ::CryDust::Key::Insert
#define CD_KEY_DELETE          ::CryDust::Key::Delete
#define CD_KEY_RIGHT           ::CryDust::Key::Right
#define CD_KEY_LEFT            ::CryDust::Key::Left
#define CD_KEY_DOWN            ::CryDust::Key::Down
#define CD_KEY_UP              ::CryDust::Key::Up
#define CD_KEY_PAGE_UP         ::CryDust::Key::PageUp
#define CD_KEY_PAGE_DOWN       ::CryDust::Key::PageDown
#define CD_KEY_HOME            ::CryDust::Key::Home
#define CD_KEY_END             ::CryDust::Key::End
#define CD_KEY_CAPS_LOCK       ::CryDust::Key::CapsLock
#define CD_KEY_SCROLL_LOCK     ::CryDust::Key::ScrollLock
#define CD_KEY_NUM_LOCK        ::CryDust::Key::NumLock
#define CD_KEY_PRINT_SCREEN    ::CryDust::Key::PrintScreen
#define CD_KEY_PAUSE           ::CryDust::Key::Pause
#define CD_KEY_F1              ::CryDust::Key::F1
#define CD_KEY_F2              ::CryDust::Key::F2
#define CD_KEY_F3              ::CryDust::Key::F3
#define CD_KEY_F4              ::CryDust::Key::F4
#define CD_KEY_F5              ::CryDust::Key::F5
#define CD_KEY_F6              ::CryDust::Key::F6
#define CD_KEY_F7              ::CryDust::Key::F7
#define CD_KEY_F8              ::CryDust::Key::F8
#define CD_KEY_F9              ::CryDust::Key::F9
#define CD_KEY_F10             ::CryDust::Key::F10
#define CD_KEY_F11             ::CryDust::Key::F11
#define CD_KEY_F12             ::CryDust::Key::F12
#define CD_KEY_F13             ::CryDust::Key::F13
#define CD_KEY_F14             ::CryDust::Key::F14
#define CD_KEY_F15             ::CryDust::Key::F15
#define CD_KEY_F16             ::CryDust::Key::F16
#define CD_KEY_F17             ::CryDust::Key::F17
#define CD_KEY_F18             ::CryDust::Key::F18
#define CD_KEY_F19             ::CryDust::Key::F19
#define CD_KEY_F20             ::CryDust::Key::F20
#define CD_KEY_F21             ::CryDust::Key::F21
#define CD_KEY_F22             ::CryDust::Key::F22
#define CD_KEY_F23             ::CryDust::Key::F23
#define CD_KEY_F24             ::CryDust::Key::F24
#define CD_KEY_F25             ::CryDust::Key::F25

/* Keypad */
#define CD_KEY_KP_0            ::CryDust::Key::KP0
#define CD_KEY_KP_1            ::CryDust::Key::KP1
#define CD_KEY_KP_2            ::CryDust::Key::KP2
#define CD_KEY_KP_3            ::CryDust::Key::KP3
#define CD_KEY_KP_4            ::CryDust::Key::KP4
#define CD_KEY_KP_5            ::CryDust::Key::KP5
#define CD_KEY_KP_6            ::CryDust::Key::KP6
#define CD_KEY_KP_7            ::CryDust::Key::KP7
#define CD_KEY_KP_8            ::CryDust::Key::KP8
#define CD_KEY_KP_9            ::CryDust::Key::KP9
#define CD_KEY_KP_DECIMAL      ::CryDust::Key::KPDecimal
#define CD_KEY_KP_DIVIDE       ::CryDust::Key::KPDivide
#define CD_KEY_KP_MULTIPLY     ::CryDust::Key::KPMultiply
#define CD_KEY_KP_SUBTRACT     ::CryDust::Key::KPSubtract
#define CD_KEY_KP_ADD          ::CryDust::Key::KPAdd
#define CD_KEY_KP_ENTER        ::CryDust::Key::KPEnter
#define CD_KEY_KP_EQUAL        ::CryDust::Key::KPEqual

#define CD_KEY_LEFT_SHIFT      ::CryDust::Key::LeftShift
#define CD_KEY_LEFT_CONTROL    ::CryDust::Key::LeftControl
#define CD_KEY_LEFT_ALT        ::CryDust::Key::LeftAlt
#define CD_KEY_LEFT_SUPER      ::CryDust::Key::LeftSuper
#define CD_KEY_RIGHT_SHIFT     ::CryDust::Key::RightShift
#define CD_KEY_RIGHT_CONTROL   ::CryDust::Key::RightControl
#define CD_KEY_RIGHT_ALT       ::CryDust::Key::RightAlt
#define CD_KEY_RIGHT_SUPER     ::CryDust::Key::RightSuper
#define CD_KEY_MENU            ::CryDust::Key::Menu